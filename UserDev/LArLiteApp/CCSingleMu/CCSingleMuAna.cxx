#ifndef LARLITE_MYANA_CCSingleMuAna_CXX
#define LARLITE_MYANA_CCSingleMuAna_CXX

#include "CCSingleMuAna.h"
#include <sstream>
#include "DataFormat/cosmictag.h"
#include "DataFormat/track.h"
#include "DataFormat/cosmictag.h"
namespace larlite {

  bool CCSingleMuAna::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    h = new TH1D("h","My Histogram; X-axis; Y-axis",100,0,1000);
    hTrackCosmicScore = new TH1D("hTrackCosmicScore","My Histogram; X-axis; Y-axis",100,-1.5,1.5);
    hLongestTrackCosmicScore = new TH1D("hLongestTrackCosmicScore","My Histogram; X-axis; Y-axis",100,-1.5,1.5);
    
    return true;
  }
  
  bool CCSingleMuAna::analyze(storage_manager* storage) {
  
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

    //fTrackProducer = "pandoraNuKHit";
    //fTrackProducer = "trackkalmanhit";

    double fLongestTrackCosmicScore = -999999;
    double fLongestTrackLength = 0;

    auto ev_track = storage->get_data<event_track>("pandoraNuKHit");
    auto track_tag = storage->get_data<event_cosmictag>("pandoraNuKHittag");

    if(!ev_track || ev_track->size() < 1)
      return false;

    auto assoc_infor = track_tag->association(ev_track->id());


    if(!ev_track || !track_tag) {

      std::ostringstream msg;
      if(!ev_track)  msg << "Track object not found!"       << std::endl;
      if(!track_tag)  msg << "Track tag object not found!"       << std::endl;
      throw DataFormatException(msg.str());

    }


    for (size_t i=0; i < ev_track->size(); i++){

      if(ev_track->at(i).n_point() > fLongestTrackLength){
        fLongestTrackLength = ev_track->at(i).n_point();
        fLongestTrackCosmicScore = track_tag->at(i).CosmicScore(); 
      }

      //time-selection (only if in-frame)
      //if( (evt_mctracks->at(m).Start().T() > -0.8E6) and (evt_mctracks->at(m).Start().T() < 0.8E6) ){
		 //if( evt_mctracks->at(m).size() > 1) 
		   //addTrack(evt_mctracks->at(m));
         //}
    
      h->Fill(ev_track->at(i).n_point());
      hTrackCosmicScore->Fill(track_tag->at(i).CosmicScore()); 

    }

    hLongestTrackCosmicScore->Fill(fLongestTrackCosmicScore); 



    return true;
  }

  bool CCSingleMuAna::finalize() {

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

    h->Write();
    hTrackCosmicScore->Write();
    hLongestTrackCosmicScore->Write();
    

    return true;
  }
}
#endif
