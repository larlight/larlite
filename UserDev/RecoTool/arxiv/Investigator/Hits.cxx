#ifndef LARLITE_HITS_CXX
#define LARLITE_HITS_CXX

#include "Hits.h"

namespace larlite {

  bool Hits::initialize() {

    if ( fInputProducer.empty() ) {
      print( msg::kERROR, __FUNCTION__, "Hit producer's name is not set!" );
      return false;
    }

    InitializeAnaTrees();

    return true;
  }
  
  bool Hits::analyze( storage_manager* storage ) {
  
    ResetEventTreeParams();

    _event  = storage->event_id();
    _run    = storage->run_id();
    _subrun = storage->subrun_id();

    // Retrieve hits
    auto ev_hit = storage->get_data<event_hit>( fInputProducer );
    if ( !ev_hit || !( ev_hit->size() ) ) {
      print( msg::kERROR, __FUNCTION__, 
             Form( "Hit data product from %s not found!", fInputProducer.c_str() ) );
      return false;
    }

    fEventTreeParams.run = _run;
    fEventTreeParams.subrun = _subrun;
    fEventTreeParams.event = _event;

    for ( auto & hit : *ev_hit ) {
      fEventTreeParams.nhits[hit.View()] += 1;
      fEventTreeParams.charges[hit.View()] += hit.SummedADC();
    }

    fEventTree->Fill();

    return true;
  }

  bool Hits::finalize() {

    if( _fout ) {
      if ( fEventTree ) fEventTree->Write();
      else print( msg::kERROR, __FUNCTION__, "Did not find an output tree pointer!!!" );
    } else print( msg::kERROR, __FUNCTION__, "Did not find an output file pointer!!! File not opened?" );
  
    return true;
  }

  void Hits::InitializeAnaTrees() {

    if ( fEventTree ) delete fEventTree;
    fEventTree = new TTree( "EventTree", "" );

    fEventTree->Branch( "run", &fEventTreeParams.run, "_run/I" );
    fEventTree->Branch( "subrun", &fEventTreeParams.subrun, "_subrun/I" );
    fEventTree->Branch( "event", &fEventTreeParams.event, "_event/I" );
    fEventTree->Branch( "nhits[3]", &fEventTreeParams.nhits, "nhits[3]/I" );
    fEventTree->Branch( "charges[3]", &fEventTreeParams.charges, "charges[3]/D" );

  }

  void Hits::ResetEventTreeParams() {
    fEventTreeParams.run = 0;
    fEventTreeParams.subrun = 0;
    fEventTreeParams.event = 0;
    for ( int i = 0; i < 3; ++i ) {
      fEventTreeParams.nhits[i] = 0;
      fEventTreeParams.charges[i] = 0.;
    }
  }

}
#endif
