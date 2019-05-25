#ifndef LARLITE_LEEPRECUT_CXX
#define LARLITE_LEEPRECUT_CXX

#include "LEEPreCut.h"

#include "DataFormat/ophit.h"

namespace larlite {

  bool LEEPreCut::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //
    if (_fout)
      _fout->cd();
    // if we make our own tree
    _tree = new TTree("pmt_precut_tree","");
    _tree->Branch("run"    , &_run    , "run/I");
    _tree->Branch("subrun" , &_subrun , "subrun/I");
    _tree->Branch("event"  , &_event  , "event/I");    
    bindOutputVariablesToTree(_tree);
    return true;
  }

  void LEEPreCut::configure( const fcllite::PSet& ps ) {
    fOpHitProducer = ps.get< std::string >("OpHitProducer");
    fBinTickWidth  = ps.get< int >("BinTickWidth",     6);
    fWinStartTick  = ps.get< int >("WinStartTick",   190);
    fWinEndTick    = ps.get< int >("WinEndTick",     320);
    fPEThreshold   = ps.get< float >("PEThreshold",  20.0);
    fVetoPEThresh  = ps.get< float >("VetoPEThreshold", 20.0);    
    fMaxVetoPE     = ps.get< float >("MaxVetoPE",    20.0);
    fVetoStartTick = ps.get< int >("VetoStartTick",  60);
    fVetoEndTick   = ps.get< int >("VetoEndTick",    190);
    fPMTMaxFrac    = ps.get< float > ("PMTMaxFrac",  0.6);
    fUseVetoWin    = ps.get< bool >("UseVetoWindow", true );
  }

  /**
   * set default parameters
   *
   * the timing window is set for BNB events
   *
   */
  void LEEPreCut::setDefaults( LEEPreCut::FileType_t ftype ) {

    // start and end tick
    switch (ftype) {
    case kBNB:
    case kMC:
      fWinStartTick  = 190;
      fWinEndTick    = 320;
      fVetoStartTick = 60;
      fVetoEndTick   = 190;
      break;
    case kEXTBNB:
      fWinStartTick  = 210;
      fWinEndTick    = 340;
      fVetoStartTick = 80;
      fVetoEndTick   = 210;
      break;
    case kOVERLAY:
      fWinStartTick  = 215;
      fWinEndTick    = 345;
      fVetoStartTick = 85;
      fVetoEndTick   = 215;      
      break;
    default:
      throw std::runtime_error("LEEPreCut::setDefaults. Unknown file type.");
      break;
    }

    fBinTickWidth  = 6;
    fPEThreshold   = 20.0;
    fVetoPEThresh  = 20.0;
    fMaxVetoPE     = 20.0;
    fPMTMaxFrac    = 0.6;
    fUseVetoWin    = true;
  }
  
  bool LEEPreCut::analyze(storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //


    _run = (int)storage->run_id();
    _subrun = (int)storage->subrun_id();
    _event = (int)storage->event_id();

    event_ophit* ophit_v = (event_ophit*)(storage->get_data( data::kOpHit, fOpHitProducer ));

    /* encapsulated by taritree in apply(..)
    std::vector<float> ophit_peaktime_v( ophit_v->size(), 0.0);
    std::vector<float> ophit_pe_v( ophit_v->size(), 0.0);
    std::vector<int>   ophit_femch_v( ophit_v->size(), 0);

    for ( size_t i=0; i<ophit_v->size(); i++ ) {
      auto const& ophit = ophit_v->at(i);
      ophit_peaktime_v[i] = ophit.PeakTime();
      ophit_pe_v[i] = ophit.PE();
      ophit_femch_v[i] = ophit.OpChannel();
    }

    std::vector<float> flashbins = m_algo.MakeTimeBin( ophit_peaktime_v, ophit_pe_v, fBinTickWidth, fWinStartTick, fWinEndTick );
    std::vector<float> vetobins  = m_algo.MakeTimeBin( ophit_peaktime_v, ophit_pe_v, fBinTickWidth, fVetoStartTick, fVetoEndTick );

    std::vector<float> beamPEinfo = m_algo.GetTotalPE( fPEThreshold , flashbins );

    std::vector<float> vetoPEinfo = m_algo.GetTotalPE( fVetoPEThresh, vetobins );

    float maxfrac = m_algo.PMTMaxFrac( ophit_peaktime_v, ophit_pe_v, ophit_femch_v, beamPEinfo, fBinTickWidth,  fWinStartTick);
	
    _beamPE = beamPEinfo[0]; // first bin is always the totalPE in the window
    _vetoPE = vetoPEinfo[0]; // first bin is always the totalPE in the window
    _maxfrac = maxfrac;
    _beamFirstBin = -1;
    if ( beamPEinfo.size()>1 )
      _beamFirstBin = fWinStartTick + beamPEinfo[1];
    _vetoFirstBin = -1;
    if ( vetoPEinfo.size()>1 )
      _vetoFirstBin = fVetoStartTick + vetoPEinfo[1];

    bool passed = false;
    
    if ( beamPEinfo[0]>fPEThreshold && (vetoPEinfo[0]<fMaxVetoPE || !fUseVetoWin) && maxfrac < fPMTMaxFrac )
      passed = true;
     
    _passed = (int)passed;

    */

    bool passed = apply( *ophit_v );

    if (_tree )
      _tree->Fill(); 
    
    return passed;
  }

  bool LEEPreCut::apply( const larlite::event_ophit& ophit_v ) {

    std::vector<float> ophit_peaktime_v( ophit_v.size(), 0.0);
    std::vector<float> ophit_pe_v( ophit_v.size(), 0.0);
    std::vector<int>   ophit_femch_v( ophit_v.size(), 0);

    for ( size_t i=0; i<ophit_v.size(); i++ ) {
      auto const& ophit   = ophit_v.at(i);
      ophit_peaktime_v[i] = ophit.PeakTime();
      ophit_pe_v[i]       = ophit.PE();
      ophit_femch_v[i]    = ophit.OpChannel();
    }

    std::vector<float> flashbins  = m_algo.MakeTimeBin( ophit_peaktime_v, ophit_pe_v, fBinTickWidth, fWinStartTick, fWinEndTick );
    std::vector<float> vetobins   = m_algo.MakeTimeBin( ophit_peaktime_v, ophit_pe_v, fBinTickWidth, fVetoStartTick, fVetoEndTick );

    std::vector<float> beamPEinfo = m_algo.GetTotalPE( fPEThreshold , flashbins );

    std::vector<float> vetoPEinfo = m_algo.GetTotalPE( fVetoPEThresh, vetobins );

    float maxfrac = m_algo.PMTMaxFrac( ophit_peaktime_v, ophit_pe_v, ophit_femch_v, beamPEinfo, fBinTickWidth,  fWinStartTick);
	
    _beamPE = beamPEinfo[0]; // first bin is always the totalPE in the window
    _vetoPE = vetoPEinfo[0]; // first bin is always the totalPE in the window
    _maxfrac = maxfrac;
    _beamFirstBin = -1;
    if ( beamPEinfo.size()>1 )
      _beamFirstBin = fWinStartTick + beamPEinfo[1];
    _vetoFirstBin = -1;
    if ( vetoPEinfo.size()>1 )
      _vetoFirstBin = fVetoStartTick + vetoPEinfo[1];

    bool passed = false;
    
    if ( beamPEinfo[0]>fPEThreshold && (vetoPEinfo[0]<fMaxVetoPE || !fUseVetoWin) && maxfrac < fPMTMaxFrac )
      passed = true;
     
    _passed = (int)passed;

    return true;

  }
  
  bool LEEPreCut::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //

    if (_tree ) {
      if (_fout)
	_fout->cd();
      _tree->Write();
    }
    
    return true;
  }
  
  void LEEPreCut::bindOutputVariablesToTree( TTree* ttree ) {
    // bind variables of interest to an external tree
    // we do not own it
    ttree->Branch("passed" , &_passed , "passed/I");
    ttree->Branch("beamPE" , &_beamPE , "beamPE/F");
    ttree->Branch("vetoPE" , &_vetoPE , "vetoPE/F");
    ttree->Branch("maxfrac", &_maxfrac, "maxfrac/F");
    ttree->Branch("beamFirstBin",&_beamFirstBin,"beamFirstBin/I");
    ttree->Branch("vetoFirstBin",&_vetoFirstBin,"vetoFirstBin/I");    
  }
  
}
#endif
