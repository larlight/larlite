#include <TSystem.h>
#include <TVector3.h>
#include "DataFormat/storage_manager.h"
#include "DataFormat/track.h"
#include "DataFormat/wrapper.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"

#include <iostream>
#include <vector>

int main(){

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for storage_manager class which interacts
  // decoder output root file. 
  //

  larlite::storage_manager my_storage;

  // If you wish, change the message level to DEBUG.
  // Commented out by default.
  //my_storage.set_verbosity(larlight::MSG::DEBUG);

  // Step 0: Set I/O mode: we are reading in, so "READ"
  my_storage.set_io_mode(larlite::storage_manager::kWRITE);

  // Step 1: Set output file 
  my_storage.set_out_filename("trial.root");

  // Step 2: Open a file.
  my_storage.open();

  // Step 3: Check if a file is opened.
  if(!my_storage.is_open()) {
    std::cerr << "File open failed!" << std::endl;
    return 0;
  }
  
  // Step 4: Check if it's ready to perform I/O
  if(!my_storage.is_ready_io()) {
    std::cerr << "I/O preparation failed!" << std::endl;
  }

  // Let's fill event_track for 100 events.
  //auto my_event_track = my_storage.get_data<larlite::event_track>("test");
  auto my_event_track = (::larlite::event_track*)(my_storage.get_data(larlite::data::kTrack,"test"));
  auto my_larsofthits = (::larlite::wrapper<std::vector<recob::Hit> >*)(my_storage.get_data(larlite::data::kLarSoftHit,"test"));
  auto my_larsoftclusters = (::larlite::wrapper<std::vector<recob::Cluster> >*)(my_storage.get_data(larlite::data::kLarSoftCluster,"test"));

  int run_id = 1;
  int subrun_id = 1;
  for( int i=0; i<100; i++){
    std::cout<<my_storage.get_entries_written()<<std::endl;
    int event_id = i;
    my_storage.set_id(run_id, subrun_id, event_id);
//    my_event_track->set_run(run_id); THESE ARE NOW PRIVATE
//    my_event_track->set_subrun(subrun_id);
//    my_event_track->set_event_id(event_id);

    // Let's make 2 tracks!
    for( int j=0; j<2; j++){
      larlite::track t;
      t.set_track_id(j); 

      // Let's make a track with 20 fake space points
      for(int k=0; k<20; k++){
	t.add_vertex    ( TVector3( (double)k,(double)k,(double)k ) );
	t.add_momentum  ( 1.);
	t.add_direction ( TVector3( (double)k,(double)k,(double)k ) );
      }
    
      // Append to the event track array
      my_event_track->push_back(t);


      // All values same as default constructor except the RMS
      // where I put in an arbitrary test value
      recob::Hit larsoftHit(raw::InvalidChannelID,
                            0,
                            0,
                            0.,
                            -1.,
                            200.0 + i + j,  // RMS just an arbitrary value to test
                            0.,
                            -1.,
                            0.,
                            0.,
                            -1.,
                            0,
                            -1,
                            0.,
                            -1,
                            geo::kUnknown,
                            geo::kMysteryType,
                            geo::WireID());

      my_larsofthits->product()->push_back(larsoftHit);

      recob::Cluster larsoftCluster(0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    0.0,
                                    i + j,
                                    0.0,
                                    0.0,
                                    0,
                                    static_cast<geo::View_t>(0),
                                    geo::PlaneID()
      );

      my_larsoftclusters->product()->push_back(larsoftCluster);
    }

    larlite::AssSet_t ass;
    ass.push_back(larlite::AssUnit_t(1,0));

    // Commented this out because it fails to compile
    // I do not know why it was here or what it was supposed to do.
//    my_event_track->set_association(larlite::data::kHit,"test",ass);

    // Store event
    my_storage.next_event();

  }

  my_storage.close();
  return 1;
}


