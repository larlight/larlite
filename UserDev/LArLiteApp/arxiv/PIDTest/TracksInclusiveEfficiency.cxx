#ifndef LARLITE_TRACKSINCLUSIVEEFFICIENCY_CXX
#define LARLITE_TRACKSINCLUSIVEEFFICIENCY_CXX

#include "TracksInclusiveEfficiency.h"
#include "DataFormat/track.h"
#include "DataFormat/mctrack.h"

namespace larlite {

  bool TracksInclusiveEfficiency::initialize() {
    efficiency = 1.;
    purity = 1.;
    numbMCTrackEff   = 0;
    numbMCTrackPur   = 0;
    numbRecoTrackEff = 0;
    numbRecoTrackPur = 0;

    
    if(fTrackProducer.empty()) {
      
      print(msg::kERROR,__FUNCTION__,
            "Track producer's name is not set!");
      return false;
      
    }

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool TracksInclusiveEfficiency::analyze(storage_manager* storage) {
  
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
  
    //Grab the MCTracks
    
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");    
    if(!ev_mctrack) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }


    auto const ev_track = storage->get_data<event_track>(fTrackProducer);
    if(!ev_track) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }
    

    auto const numberOfMCTracks   =   ev_mctrack->size();
    auto const numberOfRecoTracks =   ev_track->size();
 

    if (numberOfMCTracks>numberOfRecoTracks) 
      {
	numbMCTrackEff   += numberOfMCTracks;
	numbRecoTrackEff += numberOfRecoTracks;
	return false;
      }else if (numberOfMCTracks<numberOfRecoTracks) 
      {
	numbMCTrackPur   += numberOfMCTracks;
	numbRecoTrackPur += numberOfRecoTracks;
	return true;
      }else 
      {
	numbMCTrackPur   += numberOfMCTracks;
	numbRecoTrackPur += numberOfRecoTracks;
	numbMCTrackEff   += numberOfMCTracks;
	numbRecoTrackEff += numberOfRecoTracks;
	return true; 
      }

    return true;
  }

  bool TracksInclusiveEfficiency::finalize() {

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
  
 
    efficiency = (double)numbRecoTrackEff/(double)numbMCTrackEff;
    purity     = (double)numbRecoTrackPur/(double)numbMCTrackPur;


    std::cout<<"Tracks Purity    : "<<purity<<std::endl;
    std::cout<<"Tracks Efficiency: "<<efficiency<<std::endl;
    return true;
  }

}
#endif
