#ifndef LINEARENERGY_CXX
#define LINEARENERGY_CXX

#include <iomanip>

#include "LinearEnergy.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"
#include "LArUtil/LArProperties.h"
#include "AnalysisAlg/AnalysisAlgConstants.h"

namespace showerreco {

LinearEnergy::LinearEnergy()
{

  _name = "LinearEnergy";
  _fill_tree   = false;
  _fC_to_e = 6250.; // e- / fC
  _e_to_eV = 23.6;  // eV / e-
  _eV_to_MeV = 1e-6;// eV / MeV 
  _ADC_to_mV = 0.5; // ADC -> mV conversion from gain measurements
    
  // ELECTRON LIFETIME
  _tau = larutil::LArProperties::GetME()->ElectronLifetime();

  // RECOMBINATION FACTOR
  _recomb_factor = 1.;

  return;
}

void LinearEnergy::initialize()
{

  if (_fill_tree) {
    if (_tree) delete _tree;
    _tree = new TTree(_name.c_str(), "dQdx Info Tree");
    _tree->Branch("_dE", &_dE, "dE/D");
    _tree->Branch("_dQ", &_dQ, "dQ/D");
    _tree->Branch("_lifetime_corr", &_lifetime_corr, "lifetime_corr/D");
    _tree->Branch("_electrons",&_electrons,"electrons/D");
    _tree->Branch("_tick",&_tick,"tick/D");
  }

  std::cout << "Electron lifetime : " << _tau << " [ms]" << std::endl
	    << "Recomb factor     : " << _recomb_factor << std::endl
	    << "ADC -> fC         : " << _ADC_to_fC << std::endl
	    << "fC  -> e-         : " << _fC_to_e << std::endl
	    << "e-  -> eV         : " << _e_to_eV << std::endl
	    << "eV  -> MeV        : " << _eV_to_MeV << std::endl
	    << "ADC -> MeV        : " << _ADC_to_MeV << std::endl;

  _clocktick = larutil::DetectorProperties::GetME()->SamplingRate() * 1.e-3; //time sample in usec

  return;
}

void LinearEnergy::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                     Shower_t& resultShower) {

  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
    throw ShowerRecoException(ss.str());
  }

  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and calculates an energy in MeV for each plane

  // auto geom       = larutil::Geometry::GetME();
  auto geomHelper = larutil::GeometryHelper::GetME();

  // check if plane 2 has been used.
  // if so, we will fill the global energy with that from plane 2
  // otherwise, average the other two planes
  bool hasPl2 = false;
  
  auto t2cm = geomHelper->TimeToCm();

  // we want an energy for each plane
  for (size_t n = 0; n < clusters.size(); n++) {

    // get the hits associated with this cluster
    auto const& hits = clusters.at(n).hit_vector;

    // get the plane associated with this cluster
    auto const& pl = clusters.at(n).plane_id.Plane;

    if (pl == 2)
      hasPl2 = true;

    // store calculated energy
    double E  = 0.;

    // loop over hits
    for (auto const &h : hits) {

      // lifetime correction
      _tick = h.t / t2cm;

      _lifetime_corr = exp( _tick * _clocktick / _tau );

      _electrons = h.charge * _elec_gain;

      _dQ = _electrons * _lifetime_corr * _e_to_eV * _eV_to_MeV;

      _dE = _dQ / _recomb_factor;

      _tree->Fill();

      E += _dE;

    }// loop over all hits

    if (_verbose)
      std::cout << "energy on plane " << pl << " is : " << E << std::endl;

    // set the energy for this plane
    resultShower.fTotalEnergy_v[pl] = E;

  }// for all input clusters

  if (hasPl2)
    resultShower.fTotalEnergy = resultShower.fTotalEnergy_v[2];
  else
    resultShower.fTotalEnergy = ( resultShower.fTotalEnergy_v[0] + resultShower.fTotalEnergy_v[1] ) / 2.;

  return;

}

} //showerreco

#endif
