#ifndef __TAGGER_DUMP_CXX__
#define __TAGGER_DUMP_CXX__

#include "TaggerDump.h"

#include "DataFormat/user_info.h"
#include <cassert>
#include <exception>


namespace larlite {

  TaggerDump::TaggerDump() {
    _name = "TaggerDump";

    fEventTree = nullptr;
  }

  bool TaggerDump::initialize() {
    
    fEventTree = new TTree("EventTaggerDump", "");
 
    AttachEvent(fEventTree);

    _pass_str    = "pass";
    _vetoPE_str  = "vetoPE";
    _beamPE_str  = "beamPE";
    _maxFrac_str = "maxFrac";
    
    return true;
  }

  bool TaggerDump::analyze(storage_manager* storage) {
    std::cout << "@entry=" << storage->get_index() << std::endl;
    
    ClearEvent();
 
    FillEvent(storage);
    
    _run    = (int)storage->run_id();
    _subrun = (int)storage->subrun_id();
    _event  = (int)storage->event_id();
    _entry  = (int)storage->get_index();
    
    fEventTree->Fill();
        
    return true;
  }

  bool TaggerDump::finalize() {
    
    fEventTree->Write();
    
    return true;
  }

  void TaggerDump::AttachEvent(TTree* tree) {

    assert(tree);
    tree->Branch("event"  , &_event  , "event/I");
    tree->Branch("subrun" , &_subrun , "subrun/I");
    tree->Branch("run"    , &_run    , "run/I");
    tree->Branch("entry"  , &_entry  , "entry/I");

    tree->Branch("pass"    , &_pass    , "pass/I");
    tree->Branch("vetoPE"  , &_vetoPE   , "vetoPE/D");
    tree->Branch("beamPE"  , &_beamPE   , "beamPE/D");
    tree->Branch("maxFrac" , &_maxFrac  , "maxFrac/D");

  }

  void TaggerDump::FillEvent(storage_manager *sto) {

    const auto ev_user = (larlite::event_user*)sto->get_data(larlite::data::kUserInfo,"precutresults");

    if (!ev_user) return;

    if (ev_user->empty()) return;

    const auto& user = ev_user->front();
    
    _pass    = user.get_int(_pass_str);
    _vetoPE  = user.get_double(_vetoPE_str);
    _beamPE  = user.get_double(_beamPE_str);
    _maxFrac = user.get_double(_maxFrac_str);

    return;
  }


  void TaggerDump::ClearEvent() {
    
    _event  = -1.0*data::kINVALID_INT;
    _subrun = -1.0*data::kINVALID_INT;
    _run    = -1.0*data::kINVALID_INT;
    _entry  = -1.0*data::kINVALID_INT;

    _pass    = -1.0*data::kINVALID_INT;
    _vetoPE  = -1.0*data::kINVALID_DOUBLE;
    _beamPE  = -1.0*data::kINVALID_DOUBLE;
    _maxFrac = -1.0*data::kINVALID_DOUBLE;
    
    return;
  }
  

}

#endif
