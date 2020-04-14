#ifndef LARLITE_CRTVETO_CXX
#define LARLITE_CRTVETO_CXX

#include "CRTVeto.h"

#include "DataFormat/opflash.h"
#include "DataFormat/crthit.h"

namespace larlite {

  bool CRTVeto::initialize() {

    if (_fout)
      _fout->cd();
    
    // if we make our own tree
    _tree = new TTree("crtveto_tree","");
    _tree->Branch("run"    , &_run    , "run/I");
    _tree->Branch("subrun" , &_subrun , "subrun/I");
    _tree->Branch("event"  , &_event  , "event/I");    
    bindOutputVariablesToTree(_tree);
    return true;
  }

  void CRTVeto::configure( const fcllite::PSet& ps ) {
    fOpFlashProducer = ps.get< std::string >("OpFlashProducer");
    fCRTHitProducer = ps.get< std::string >("CRTHitProducer");
    fWinStart  = ps.get< float >("WinStart",   3.19);
    fWinEnd    = ps.get< float >("WinEnd",     4.87);
    fDeltaT    = ps.get< float >("DeltaT",     1.0);
    fCRTHitPE  = ps.get< float >("CRTHitPE",   100.);
  }

  /**
   * set default parameters
   *
   * the timing window is set for BNB events
   *
   */
  void CRTVeto::setDefaults( CRTVeto::FileType_t ftype ) {

    // start and end tick
    switch (ftype) {
    case kBNB:
    case kMC:
      fWinStart  = 3.19;
      fWinEnd    = 4.87;
      break;
    case kEXTBNB:
      fWinStart  = 3.57;
      fWinEnd    = 5.25;
      break;
    case kOVERLAY:
      fWinStart  = 3.57;
      fWinEnd    = 5.25;
      break;
    default:
      throw std::runtime_error("CRTVeto::setDefaults. Unknown file type.");
      break;
    }

    fDeltaT = 1.0;
    fCRTHitPE = 100.;
  }
  
  bool CRTVeto::analyze(storage_manager* storage) {
  
    _run = (int)storage->run_id();
    _subrun = (int)storage->subrun_id();
    _event = (int)storage->event_id();

    _DT.clear();
    _opflashT.clear();
    _crthitT.clear();
    _crthitPE.clear();
    
    event_opflash* opflash_v = (event_opflash*)(storage->get_data( data::kOpFlash, fOpFlashProducer ));
    event_crthit* crthit_v = (event_crthit*)(storage->get_data( data::kCRTHit, fCRTHitProducer ));

    bool passed = apply( *opflash_v, *crthit_v );

    if (_tree )
      _tree->Fill(); 
    
    return passed;
  }

  bool CRTVeto::apply( const larlite::event_opflash& opflash_v, const larlite::event_crthit& crthit_v ) {

    std::vector<float> opflash_time_v;
    opflash_time_v.clear();

    for ( size_t i=0; i<opflash_v.size(); i++ ) {
      auto const& opflash   = opflash_v.at(i);
      // only fill flashes within beam spill window
      _opflashT.push_back(opflash.Time());
      if( opflash.Time()>= fWinStart && opflash.Time()<= fWinEnd ) opflash_time_v.push_back(opflash.Time());
    }
    //if we don't have any flashes do not tag
    if(opflash_time_v.size()<1) {
      std::cout << "[CRTVeto] No flashes in the beam window. "
		<< "window=[" << fWinStart << "," << fWinEnd << "] "
		<< "In input container=" << opflash_v.size()
		<< std::endl;
      _passed = 1;
      return true;
    }
      
    bool passed = true;
    
    for( size_t i =0; i < crthit_v.size(); i++){
      auto const& crthit = crthit_v.at(i);
      _crthitPE.push_back(crthit.peshit);
      _crthitT.push_back(crthit.ts2_ns/1000.);

      if ( crthit.peshit < fCRTHitPE ) continue;

      for (size_t j = 0; j< opflash_time_v.size(); j++){
	_DT.push_back(crthit.ts2_ns/1000. - opflash_time_v[j]);
	if ( fabs(crthit.ts2_ns/1000. - opflash_time_v[j]) <= fDeltaT ){
	  std::cout << "[CRTVeto] Found coincident CRT Hit: "
		    << " hit_time=" << crthit.ts2_ns/1000. << " usec; "
		    << " dT=" << fabs(crthit.ts2_ns/1000. - opflash_time_v[j]) 
		    << std::endl;
	  passed = false;
	  break;
	}
      }
      if( passed == false ) break;
    }
    _passed = (int)passed;

    //return true;
    return passed;
  }
  
  bool CRTVeto::finalize() {

    if (_tree ) {
      if (_fout){
	std::cout << "Finalizing output" << std::endl;
	_fout->cd();
	_tree->Write();
	_fout->Close();
      }
    }
    
    return true;
  }
  
  void CRTVeto::bindOutputVariablesToTree( TTree* ttree ) {
    // bind variables of interest to an external tree
    // we do not own it
    ttree->Branch("passed" , &_passed , "passed/I");
    ttree->Branch("opflashT" , &_opflashT );
    ttree->Branch("crthitT" , &_crthitT );
    ttree->Branch("crthitPE" , &_crthitPE );
    ttree->Branch("DT", &_DT);
  }
  
}
#endif
