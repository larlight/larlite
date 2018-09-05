#ifndef LARLITE_MICHEL_DUMP_CXX
#define LARLITE_MICHEL_DUMP_CXX

#include "michel_dump.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"

#include <map>
namespace larlite {


  bool michel_dump::initialize() {

    _outtree = new TTree("MichelTree","");
    _outtree->Branch("run"        , &_run       , "run/I");
    _outtree->Branch("subrun"     , &_subrun    , "subrun/I");
    _outtree->Branch("event"      , &_event     , "event/I");
    _outtree->Branch("has_michel" , &_has_michel, "has_michel/I");

    _outtree->Branch("michel_x" , &_michel_x, "michel_x/F");
    _outtree->Branch("michel_y" , &_michel_y, "michel_y/F");
    _outtree->Branch("michel_z" , &_michel_z, "michel_z/F");

    _outtree->Branch("michel_sx" , &_michel_sx, "michel_sx/F");
    _outtree->Branch("michel_sy" , &_michel_sy, "michel_sy/F");
    _outtree->Branch("michel_sz" , &_michel_sz, "michel_sz/F");

    _outtree->Branch("muon_e"   , &_muon_e  , "muon_e/F");
    _outtree->Branch("michel_e" , &_michel_e, "michel_e/F");

    return true;
  }
  
  bool michel_dump::analyze(storage_manager* storage) {

    _run    = (int) storage->run_id();
    _subrun = (int) storage->subrun_id();
    _event  = (int) storage->event_id();

    auto ev_mctrack  = (event_mctrack*)storage->get_data<event_mctrack>("mcreco");
    auto ev_mcshower = (event_mcshower*)storage->get_data<event_mcshower>("mcreco"); 

    // build ID -> position map for MCTracks
    std::map<int,int> trackIDMap;
    for (size_t i=0; i < ev_mctrack->size(); i++)
      trackIDMap[ ev_mctrack->at(i).TrackID() ] = i;

    static std::string decay   = "Decay";
    static std::string process = "muMinusCaptureAtRest";

    _has_michel = 0;

    _muon_e = -1*data::kINVALID_FLOAT;
    _michel_e = -1*data::kINVALID_FLOAT;
    
    _michel_x = -1*data::kINVALID_FLOAT;
    _michel_y = -1*data::kINVALID_FLOAT;
    _michel_z = -1*data::kINVALID_FLOAT;

    _michel_sx = -1*data::kINVALID_FLOAT;
    _michel_sy = -1*data::kINVALID_FLOAT;
    _michel_sz = -1*data::kINVALID_FLOAT;
    
    
    for(const auto& mcsh : *(ev_mcshower)) {
      
      if ( (mcsh.Process() != decay ) and (mcsh.Process() != process) )
	continue;

      if (mcsh.PdgCode() == 22)
	continue;

      if ( (mcsh.MotherPdgCode() != 13) and (mcsh.MotherPdgCode() != -13) )
	continue;
      
      auto const& e_strt = mcsh.Start();

      if (e_strt.E() < 1)
	continue;

      auto const& muon = ev_mctrack->at( trackIDMap.at(mcsh.MotherTrackID()) );

      if (muon.size() < 2)
	continue;

      if ( muon.at(muon.size() - 2).E() > 106)
	continue;
      
      _has_michel = 1;
      _muon_e = muon.Start().E();
      _michel_e = e_strt.E();
      
      _michel_x = e_strt.X();
      _michel_y = e_strt.Y();
      _michel_z = e_strt.Z();
      
      const auto offset = _sce.GetPosOffsets(_michel_x, _michel_y, _michel_z);
      
      _michel_sx = _michel_x - offset[0] + 0.7;
      _michel_sy = _michel_y + offset[1];
      _michel_sz = _michel_z + offset[2];            
      
      break;
    }
    
  
    _outtree->Fill();

    return true;
  }

  bool michel_dump::finalize() {
    _outtree->Write();
    return true;
  }

}
#endif
