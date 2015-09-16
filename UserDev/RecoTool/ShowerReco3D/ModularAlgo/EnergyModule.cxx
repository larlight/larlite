#ifndef ENERGYMODULE_CXX
#define ENERGYMODULE_CXX

#include <iomanip>

#include "EnergyModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco{

  EnergyModule::EnergyModule()
  {

    _name = "EnergyModule";
    _useArea = true;
    _caloAlg = ::calo::CalorimetryAlg();
    _caloAlg.setUseModBox(false);

  }

  void EnergyModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    
    // This function takes the shower cluster set and calculates an energy in MeV for each plane

    // prepare a vector that holds plane-by-plane energy
    // entries 0,1,2 correspond to planes U,V,Y
    std::vector<double> showerEnergy = {0.,0.,0.};

    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();

    // get the 3D direction reconstructed hopefully in a previous step
    auto const& dir3D = resultShower.fDCosStart;

    std::cout << "dir3D: [" << dir3D[0] << ", " << dir3D[1] << ", " << dir3D[2] << "]" << std::endl;

    // we want an energy for each plane
    for (size_t n=0; n < inputShowers.size(); n++){

      // get the hits associated with this cluster
      auto const& hits = inputShowers.at(n).hit_vector;

      // get the plane associated with this cluster
      auto const& pl = inputShowers.at(n).plane_id.Plane;

      // store calculated energy
      double E = 0.;
      // store hit-by-hit dEdx
      double dEdx = 0.;

      //*** we need to calculate a pitch for this plane
      double pitch = geomHelper->GetPitch(dir3D,(int)pl);

      if (_verbose) std::cout << "pitch is: " << pitch << std::endl;

      // loop over hits
      for (auto const &h : hits){
	
	if (_useArea)
	  dEdx = _caloAlg.dEdx_AMP(h.charge / pitch, h.t / geomHelper->TimeToCm(), h.plane);
	else
	  dEdx = _caloAlg.dEdx_AMP(h.peak / pitch, h.t / geomHelper->TimeToCm(), h.plane);

	// make sure we aren't adding a crazy amount
	// this is totally possible due to non-linear
	// birks / box model formulas

	if (dEdx > 10.) { dEdx = 10.; }
	E += dEdx*pitch;

      }// loop over all hits

      if (_verbose)
	std::cout << "energy on plane " << pl << " is : " << E << std::endl;
						
      // set the energy for this plane
      resultShower.fTotalEnergy[pl] = E;

    }// for all input clusters
    
    return;

  }

} //showerreco

#endif
