#include <TSystem.h>
#include <TVector3.h>
#include "DataFormat/storage_manager.h"
#include "DataFormat/track.h"
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

    if(!my_track_v) {

      std::cout << "No event_track found!" << std::endl;

      break;
    }

    // Check if pointer is valid
    std::cout 
      << Form("Found event %d ... %zu tracks! ", my_track_v->event_id(), my_track_v->size())
      << std::endl;
    my_track_v->list_association();
  }

  my_storage.close();
  return 1;
}


