#ifndef ENERGYMODULE_CXX
#define ENERGYMODULE_CXX

#include <iomanip>

#include "EnergyModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

EnergyModule::EnergyModule()
{

  _name = "EnergyModule";
  _useArea = true;
  _caloAlg = ::calo::CalorimetryAlg();
  _caloAlg.setUseModBox(false);

}

void EnergyModule::initialize()
{

  /*
  if (_tree) delete _tree;
  _tree = new TTree(_name.c_str(),"dQdx Info Tree");
  _tree->Branch("_dEdx_v","std::vector<double>",&_dEdx_v);
  _tree->Branch("_timetick_v","std::vector<double>",&_timetick_v);
  _tree->Branch("_pl",&_pl,"_pl/I");
  */

  return;
}

void EnergyModule::do_reconstruction(const ProtoShower & proto_shower,
                                     Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and calculates an energy in MeV for each plane

  // prepare a vector that holds plane-by-plane energy
  // entries 0,1,2 correspond to planes U,V,Y
  std::vector<double> showerEnergy = {0., 0., 0.};

  //_dEdx_v.clear();
  //_timetick_v.clear();

  auto geom       = larutil::Geometry::GetME();
  auto geomHelper = larutil::GeometryHelper::GetME();

  auto t2cm = geomHelper->TimeToCm();

  // get the 3D direction reconstructed hopefully in a previous step
  auto const& dir3D = resultShower.fDCosStart;

  if (_verbose)
    std::cout << "dir3D: [" << dir3D[0] << ", " << dir3D[1] << ", " << dir3D[2] << "]" << std::endl;

  // we want an energy for each plane
  for (size_t n = 0; n < clusters.size(); n++) {

    // get the hits associated with this cluster
    auto const& hits = clusters.at(n).hit_vector;

    // get the plane associated with this cluster
    auto const& pl = clusters.at(n).plane_id.Plane;
    //_pl = pl;

    // store calculated energy
    double E = 0.;
    // store hit-by-hit dEdx
    double dEdx = 0.;

    //*** we need to calculate a pitch for this plane
    double pitch = geomHelper->GetPitch(dir3D, (int)pl);

    if (_verbose) std::cout << "pitch is: " << pitch << std::endl;

    // loop over hits
    for (auto const &h : hits) {
      double time_tick = (h.t / t2cm);
      //_timetick_v.push_back(time_tick);

      if (_useArea)
        dEdx = _caloAlg.dEdx_AREA(h.charge / pitch, time_tick, pl);
      else
        dEdx = _caloAlg.dEdx_AMP(h.peak / pitch, time_tick, pl);

      // make sure we aren't adding a crazy amount
      // this is totally possible due to non-linear
      // birks / box model formulas

      if (dEdx > 10.) { dEdx = 10.; }
      //_dEdx_v.push_back(dEdx);

      E += dEdx * pitch;

    }// loop over all hits

    //_tree->Fill();

    if (_verbose)
      std::cout << "energy on plane " << pl << " is : " << E << std::endl;

    // set the energy for this plane
    resultShower.fTotalEnergy_v[pl] = E;

  }// for all input clusters

  return;

}

} //showerreco

#endif
