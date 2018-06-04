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

    return true;
  }

  void LEEPreCut::configure( const fcllite::PSet& ps ) {
    fOpHitProducer = ps.get< std::string >("OpHitProducer");
    fBinTickWidth  = ps.get< int >("BinTickWidth",     6);
    fWinStartTick  = ps.get< int >("WinStartTick",   190);
    fWinEndTick    = ps.get< int >("WinEndTick",     320);
    fPEThreshold   = ps.get< float >("PEThreshold", 20.0);
    fVetoStartTick = ps.get< int >("VetoStartTick",  60);
    fVetoEndTick   = ps.get< int >("VetoEndTick",    190);
    fPMTMaxFrac    = ps.get< float > ("PMTMaxFrac",  0.6);
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

    event_ophit* ophit_v = (event_ophit*)(storage->get_data( data::kOpHit, fOpHitProducer ));
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
    std::vector<float> vetoPEinfo = m_algo.GetTotalPE( fPEThreshold , vetobins );

    float maxfrac     = m_algo.PMTMaxFrac( ophit_peaktime_v, ophit_pe_v, ophit_femch_v, beamPEinfo, fBinTickWidth,  fWinStartTick);

    _beamPE = beamPEinfo[0];
    _vetoPE = vetoPEinfo[0];
    _maxFrac = maxfrac;
    
    
    if ( beamPEinfo[0]>fPEThreshold && vetoPEinfo[0]<fPEThreshold && maxfrac < fPMTMaxFrac ) {
      _result = 1;
      return true;
    }

    _result = 0;
    return false;
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
  
    return true;
  }

  void LEEPreCut::bindOutputVariablesToTree( TTree* ttree ) {
    ttree->Branch( "precut_result", &_result, "precut_result/I");
    ttree->Branch( "precut_beamPE", &_beamPE, "precut_beamPE/F" );
    ttree->Branch( "precut_vetoPE", &_vetoPE, "precut_vetoPE/F" );
    ttree->Branch( "precut_maxFrac", &_maxFrac, "precut_maxFrac/F" );
  }
  
}
#endif
