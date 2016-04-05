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
  _useArea = true;
  _useModBox = true;
  _fill_tree = false;
  _caloAlg = ::calo::CalorimetryAlg();
  _caloAlg.setUseModBox(false);
  _fC_to_e = 6250.; // a fC in units of the electron charge
  _e_to_eV = 23.6;  // ionization energy of Ar in eV
  _eV_to_MeV = 1e-6; // eV -> MeV conversion
  _ADC_to_mV = 0.5; // ADC -> mV conversion from gain measurements
  // to go from mV to fC the ASIC gain and Shaping time
  // must be considered
  // fC -> mV *= ( shaping time * ASIC gain )
  _shp_time  = 2.; // in usec
  _asic_gain = 7.8; // in mV/fC

  return;
}

void LinearEnergy::initialize()
{

  if (_fill_tree) {
    if (_tree) delete _tree;
    _tree = new TTree(_name.c_str(), "dQdx Info Tree");
    _tree->Branch("_dE_v", "std::vector<double>", &_dE_v);
    _tree->Branch("_dEdx_v", "std::vector<double>", &_dEdx_v);
    _tree->Branch("_dQ_v", "std::vector<double>", &_dQ_v);
    _tree->Branch("_pl", &_pl, "_pl/I");
  }

  _charge_conversion = _ADC_to_mV / ( _shp_time * _asic_gain ) ;
  //_charge_conversion = _ADC_to_mV / _asic_gain;

  _energy_conversion = _charge_conversion * _fC_to_e * _e_to_eV * _eV_to_MeV;

  _tau = larutil::LArProperties::GetME()->ElectronLifetime();        // electron lifetime in usec
  _recomb_factor = 1.;
  double MeV_to_fC = 1. / ( _e_to_eV * _eV_to_MeV );
  double MIP = 2.3; // MeV
  if (_useModBox)
    _recomb_factor = larutil::LArProperties::GetME()->ModBoxInverse(MIP) / ( MIP * MeV_to_fC );
  else
    _recomb_factor = larutil::LArProperties::GetME()->BirksInverse(MIP) / ( MIP * MeV_to_fC );

  if (_verbose)
    std::cout << "Recombination factor is : " << _recomb_factor << std::endl;

  if (_verbose) {
    std::cout << "do inverse functions work?" << std::endl;
    std::cout << "ModBox: " << std::endl;
    double dqdx = larutil::LArProperties::GetME()->ModBoxInverse(MIP);
    double dedx = larutil::LArProperties::GetME()->ModBoxCorrection(dqdx);
    std::cout << "MIP : " << MIP << " -> dQdx : " << dqdx << " -> dEdx : " << dedx << std::endl;
    std::cout << "Birks: " << std::endl;
    dqdx = larutil::LArProperties::GetME()->BirksInverse(MIP);
    dedx = larutil::LArProperties::GetME()->BirksCorrection(dqdx);
    std::cout << "MIP : " << MIP << " -> dQdx : " << dqdx << " -> dEdx : " << dedx << std::endl;
  }

  _timetick = larutil::DetectorProperties::GetME()->SamplingRate() * 1.e-3; //time sample in usec

  return;
}

void LinearEnergy::do_reconstruction(const ProtoShower & proto_shower,
                                     Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and calculates an energy in MeV for each plane

  _dQ_v.clear();
  _dE_v.clear();
  _dEdx_v.clear();

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
    _pl = pl;

    if (pl == 2)
      hasPl2 = true;

    auto const& dir3D = resultShower.fDCosStart;
    double pitch = geomHelper->GetPitch(dir3D, (int)pl);

    // store calculated energy
    double E = 0.;
    double dQ = 0.;
    double dE = 0.;

    // loop over hits
    for (auto const &h : hits) {

      // lifetime correction
      double hit_tick = h.t / t2cm ;
      double lifetimeCorr = exp( hit_tick * _timetick / _tau );

      if (_useArea) {
        dQ = _caloAlg.ElectronsFromADCArea(h.charge, pl);
        dE = dQ * lifetimeCorr * _e_to_eV * _eV_to_MeV;
      }
      else {
        dQ = _caloAlg.ElectronsFromADCPeak(h.peak, pl);
        dE = dQ * lifetimeCorr * _e_to_eV * _eV_to_MeV;
      }

      if (_fill_tree) {
        _dQ_v.push_back(dQ);
        _dE_v.push_back(dE);
        _dEdx_v.push_back(dE / pitch / _recomb_factor);
      }

      E += dE;

    }// loop over all hits

    E /= _recomb_factor;

    if (_fill_tree)
      _tree->Fill();

    // if (_verbose)
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
