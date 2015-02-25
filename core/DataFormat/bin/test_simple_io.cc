#include "DataFormat/storage_manager.h"

int main()
{

  //
  // This is a simple routine to test I/O.
  //
  
  // (1) Open file and write 10 events of various data type

  const std::string test_fname("from_io_test_you_can_remove_me.root");
  
  larlite::storage_manager man;
  man.set_io_mode(man.kWRITE);
  man.set_out_filename(test_fname);
  man.set_verbosity(larlite::msg::kNORMAL);

  int nevents_written[larlite::data::kDATA_TYPE_MAX]={0};

  std::cout 
    << std::endl
    << "test routine event loop starts here..." << std::endl
    << std::endl;

  man.open();
  for(int i=0; i<10; i++) {

    for(int j=0; j<larlite::data::kDATA_TYPE_MAX; ++j) {

      if( j == larlite::data::kSeed ||
	  j == larlite::data::kEvent ||
	  j == larlite::data::kMCTrajectory ||
	  j == larlite::data::kMCNeutrino ||
	  j == larlite::data::kDATA_TYPE_MAX )

	continue;
      auto ptr = man.get_data((larlite::data::DataType_t)j,"test");
      if(ptr)
	nevents_written[j]++;
    }

    man.next_event();

  }
  
  man.close();
  man.reset();

  std::cout 
    << std::endl
    << "First event loop finished & file closed."
    << std::endl;

  for(int i=0; i<larlite::data::kDATA_TYPE_MAX; ++i) {
    
    if(nevents_written[i])

      std::cout 
	<< Form("Written %s_tree with %d events",
		larlite::data::kDATA_TREE_NAME[i].c_str(),
		nevents_written[i])
	<< std::endl;
  }
  std::cout << std::endl;
    
  // (2) Alright now let's test reading what was written.
  man.set_io_mode(man.kREAD);
  man.add_in_filename(test_fname);

  std::cout
    << "Now attempting to read what is written." << std::endl
    << std::endl;

  man.open();
  while(man.next_event()) {

    for(int i=0; i<larlite::data::kDATA_TYPE_MAX; ++i) {

      auto my_event_data = man.get_data((larlite::data::DataType_t)i,"test");

      if(my_event_data)

	nevents_written[i]-=1;

    }

  }
  
  man.close();

  std::cout
    << std::endl
    << "Finished read event loop and closed file." << std::endl
    << "Check the following block for an error message." << std::endl
    << "//********************************************//" 
    << std::endl;

  for(int i=0; i<larlite::data::kDATA_TYPE_MAX; ++i) {

    if(nevents_written[i]>0)

      larlite::Message::send(larlite::msg::kERROR,
			      Form("TTree, \"%s_tree\", has more entries (+%d) than expected!",
				   larlite::data::kDATA_TREE_NAME[i].c_str(),
				   nevents_written[i]));
    
    else if(nevents_written[i]<0)
      
      larlite::Message::send(larlite::msg::kERROR,
			      Form("TTree, \"%s_tree\", has less entries (%d) than expected!",
				   larlite::data::kDATA_TREE_NAME[i].c_str(),
				   nevents_written[i]));
    
  }
  std::cout 
    << "//********************************************//"  << std::endl
    << std::endl
    << "End of test routine..." << std::endl
    << std::endl;    

  return 0;
}
