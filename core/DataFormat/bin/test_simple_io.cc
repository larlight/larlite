#include "DataFormat-TypeDef.hh"

int main()
{

  //
  // This is a simple routine to test I/O.
  //
  
  // (1) Open file and write 10 events of various data type

  const std::string test_fname("from_io_test_you_can_remove_me.root");
  
  larlight::storage_manager man;
  man.set_io_mode(man.WRITE);
  man.set_out_filename(test_fname);
  man.set_verbosity(larlight::MSG::NORMAL);

  int nevents_written[larlight::DATA::DATA_TYPE_MAX]={0};

  std::cout 
    << std::endl
    << "test routine event loop starts here..." << std::endl
    << std::endl;

  man.open();
  for(int i=0; i<10; i++) {

    for(int j=0; j<larlight::DATA::DATA_TYPE_MAX; ++j) {

      if( j == larlight::DATA::Seed ||
	  j == larlight::DATA::Event ||
	  j == larlight::DATA::MCTrajectory ||
	  j == larlight::DATA::MCNeutrino ||
	  j == larlight::DATA::DATA_TYPE_MAX )

	continue;
      man.get_data((larlight::DATA::DATA_TYPE)j);
      
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

  for(int i=0; i<larlight::DATA::DATA_TYPE_MAX; ++i) {
    
    if(nevents_written[i])

      std::cout 
	<< Form("Written %s_tree with %d events",
		larlight::DATA::DATA_TREE_NAME[i].c_str(),
		nevents_written[i])
	<< std::endl;
  }
  std::cout << std::endl;
    
  // (2) Alright now let's test reading what was written.
  man.set_io_mode(man.READ);
  man.add_in_filename(test_fname);

  std::cout
    << "Now attempting to read what is written." << std::endl
    << std::endl;

  man.open();
  while(man.next_event()) {

    for(int i=0; i<larlight::DATA::DATA_TYPE_MAX; ++i) {

      larlight::event_base* my_event_data = man.get_data((larlight::DATA::DATA_TYPE)i);

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

  for(int i=0; i<larlight::DATA::DATA_TYPE_MAX; ++i) {

    if(nevents_written[i]>0)

      larlight::Message::send(larlight::MSG::ERROR,
			      Form("TTree, \"%s_tree\", has more entries (+%d) than expected!",
				   larlight::DATA::DATA_TREE_NAME[i].c_str(),
				   nevents_written[i]));
    
    else if(nevents_written[i]<0)
      
      larlight::Message::send(larlight::MSG::ERROR,
			      Form("TTree, \"%s_tree\", has less entries (%d) than expected!",
				   larlight::DATA::DATA_TREE_NAME[i].c_str(),
				   nevents_written[i]));
    
  }
  std::cout 
    << "//********************************************//"  << std::endl
    << std::endl
    << "End of test routine..." << std::endl
    << std::endl;    

  return 0;
}
