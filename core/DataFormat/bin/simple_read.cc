#include <TSystem.h>
#include <TVector3.h>
#include "DataFormat/storage_manager.h"
#include "DataFormat/hit1.h"
#include "DataFormat/track.h"
#include "DataFormat/wrapper.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include <vector>

int main(int argc, char** argv){

  if(argc<2) {
    
    std::cerr << Form("Usage: %s $INPUT_FILE\n",argv[0]) << std::endl;

    return 1;

  }

  //
  // A simple routine to read a data file and perform an event loop.
  // This is a test routine for storage_manager class which interacts
  // decoder output root file. 
  //

  larlite::storage_manager my_storage;

  // If you wish, change the message level to DEBUG.
  // Commented out by default.
  my_storage.set_verbosity(larlite::msg::kDEBUG);
  
  // Step 0: Set I/O mode: we are reading in, so "READ"
  my_storage.set_io_mode(larlite::storage_manager::kREAD);

  // Step 1: Add input file from user input
  my_storage.add_in_filename(argv[1]);

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

  // Let's loop over!
  while(my_storage.next_event()){
    
    auto my_track_v = my_storage.get_data<larlite::event_track>("test");
    //auto my_track_v = (::larlite::event_track*)(my_storage.get_data(::larlite::data::kTrack,"test"));
    auto my_hit1_v = my_storage.get_data<larlite::wrapper<std::vector<larlite::hit1> > >("test");
    auto my_int = my_storage.get_data<larlite::wrapper<int> >("test");
    auto my_m_intdouble = my_storage.get_data<larlite::wrapper<std::map<int,double> > >("test");
    auto my_larsofthits = my_storage.get_data<larlite::wrapper<std::vector<recob::Hit> > >("test");
    auto my_larsoftclusters = my_storage.get_data<larlite::wrapper<std::vector<recob::Cluster> > >("test");

    if(!my_track_v) {

      std::cout << "No event_track found!" << std::endl;

      break;
    }

    // Check if pointer is valid
    std::cout 
      << Form("Found event %d ... %zu tracks! ", my_track_v->event_id(), my_track_v->size())
      << std::endl;
    std::cout << "vector hit1 size = " << my_hit1_v->product()->size() << std::endl;
    std::cout << "hit1 rms = " << my_hit1_v->product()->at(0).RMS() << std::endl;
    std::cout << "hit1 rms = " << my_hit1_v->product()->at(1).RMS() << std::endl;

    std::cout << "int = " << *(my_int->product()) << std::endl;

    std::cout << "map value = " << my_m_intdouble->product()->at(31) << std::endl;

    std::cout << "larsoft hits, size = " <<  my_larsofthits->product()->size()
              << ", 0 RMS = " << my_larsofthits->product()->at(0).RMS()
              << ", 1 RMS = " << my_larsofthits->product()->at(1).RMS() << std::endl;
    std::cout << "larsoft clusters size = " <<  my_larsoftclusters->product()->size()
              << ", 0 NHits = " << my_larsoftclusters->product()->at(0).NHits()
              << ", 1 NHits = " << my_larsoftclusters->product()->at(1).NHits() << std::endl;

    // Commented this out because it fails to compile
    // I do not know why it is here or what it does.
    // my_track_v->list_association();
  }

  my_storage.close();
  return 1;
}
