#ifndef LARLITE_NC_DUMP_CXX
#define LARLITE_NC_DUMP_CXX

#include "nc_dump.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctruth.h"

namespace larlite {

  bool nc_dump::initialize() {

    _outtree = new TTree("nc_tree","");
    _outtree->Branch("run"   , &_run    , "run/I");
    _outtree->Branch("subrun", &_subrun , "subrun/I");
    _outtree->Branch("event" , &_event  , "event/I");
    _outtree->Branch("shower_pdg_v"  , &_shower_pdg_v);
    _outtree->Branch("shower_trackid_v"  , &_shower_trackid_v);
    _outtree->Branch("is_primary_v"  , &_is_primary_v);
    _outtree->Branch("is_pi0_v"      , &_is_pi0_v);
    _outtree->Branch("shower_edep_v" , &_shower_edep_v);
    _outtree->Branch("pi0_energy", &_pi0_energy,"pi0_energy/F");

    return true;
  }
  
  bool nc_dump::analyze(storage_manager* storage) {
    ClearEvent();
    
    const auto ev_shr = (event_mcshower*)storage->get_data(data::kMCShower,"mcreco");
    const auto ev_tru = (event_mctruth*)storage->get_data(data::kMCTruth,"generator");
    
    const auto& tru = ev_tru->front();
    for(const auto& par : tru.GetParticles()) {
      if (par.PdgCode() == 111) {
	_pi0_energy = par.Trajectory().front().E()*1e3;
	break;
      }
    }
    _run    = (int)storage->run_id();
    _subrun = (int)storage->subrun_id();
    _event  = (int)storage->event_id();
    
    for(const auto& shr : *ev_shr) {

      // add primary pdgs
      if (shr.TrackID() == shr.MotherTrackID()) {
	_shower_pdg_v.push_back(shr.PdgCode());
	_shower_trackid_v.push_back(shr.TrackID());
	_is_primary_v.push_back(1);
	_is_pi0_v.push_back(0);
	_shower_edep_v.push_back(shr.DetProfile().E());
	continue;
      }
      
      // add pi0
      if (shr.MotherPdgCode() == 111) {
	if (shr.MotherTrackID() != shr.AncestorTrackID()) continue; // is this needed?
	_shower_pdg_v.push_back(shr.PdgCode());
	_shower_trackid_v.push_back(shr.TrackID());
	_is_primary_v.push_back(0);
	_is_pi0_v.push_back(1);
	_shower_edep_v.push_back(shr.DetProfile().E());	
      }


    }

    _outtree->Fill();

    return true;
  }


  void nc_dump::ClearEvent() {

    _run    = -1*data::kINVALID_INT;
    _subrun = -1*data::kINVALID_INT;
    _event  = -1*data::kINVALID_INT;

    _shower_pdg_v.clear();
    _shower_trackid_v.clear();
    _is_primary_v.clear();
    _is_pi0_v.clear();
    _shower_edep_v.clear();

    _pi0_energy = -1*data::kINVALID_FLOAT;
    
    return;
  }

  bool nc_dump::finalize() {
  

    _outtree->Write();
    return true;
  }

}
#endif
