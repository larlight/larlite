#ifndef ENERGYBIRKSDMODULE_CXX
#define ENERGYBIRKSDMODULE_CXX

#include <iomanip>

#include "EnergyBirksModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco{

  EnergyBirksModule::EnergyBirksModule(){

    _name = "EnergyBirksModule";
    _useArea = true;
    _caloAlg->setUseModBox(false);

  }

  void EnergyBirksModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    
    // This function takes the shower cluster set and calculates an energy in MeV for each plane

    // prepare a vector that holds plane-by-plane energy
    // entries 0,1,2 correspond to planes U,V,Y
    std::vector<double> showerEnergy = {0.,0.,0.};

    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();

    // get the 3D direction reconstructed hopefully in a previous step
    TVector3 dir3D = resultShower.fDCosStart;

    // we want an energy for each plane
    for (size_t n=0; n < inputShowers.size(); n++){

      // get the hits associated with this cluster
      auto const& hits = inputShowers.at(n).hit_vector;

      // get the plane associated with this cluster
      auto pl = inputShowers.at(n).plane_id.Plane;

      // store calculated energy
      double E = 0.;
      // store hit-by-hit dEdx
      double dEdx = 0.;

      //*** we need to calculate a pitch for this plane
      // get the 2D angle on this plane from a 3D angle
      float angle2D = geomHelper->Slope_3Dto2D(dir3D,pl);
      double pitch = 1;

      // loop over hits
      for (auto const &h : hits){
	
	if (_useArea)
	  dEdx = _caloAlg->dEdx_AMP(h.charge / pitch, h.t / geomHelper->TimeToCm(), h.plane);
	else
	  dEdx = _caloAlg->dEdx_AMP(h.peak / pitch, h.t / geomHelper->TimeToCm(), h.plane);

	E += dEdx;

      }// loop over all hits

    }
    
    return;

  }

} //showerreco

#endif
