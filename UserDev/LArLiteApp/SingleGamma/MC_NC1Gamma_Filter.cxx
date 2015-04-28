#ifndef LARLITE_MC_NC1GAMMA_FILTER_CXX
#define LARLITE_MC_NC1GAMMA_FILTER_CXX

#include "MC_NC1Gamma_Filter.h"

namespace larlite {

  bool MC_NC1Gamma_Filter::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

		total_events  = 0;
		looped_events = 0;
		kept_events   = 0;

    return true;
  }
  
  bool MC_NC1Gamma_Filter::analyze(storage_manager* storage) {
  
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
 	
		// By default, we'll keep the MC event.  This will be switched
		// to "false" below when we decide we don't want it.
		bool ret = true;
		total_events++;
		
		// If max_events was set to -1, or we haven't reached max_events yet, 
		// then return the pass/fail flag.  Otherwise, return FALSE.
		if( max_events < 0 || total_events <= max_events) {
			looped_events++;
			if( ret ) kept_events++;
			return ret;
		} else {
			return false;
		}
  
	}

  bool MC_NC1Gamma_Filter::finalize() {

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
 
		std::cout<<"MC_NC1Gamma_Filter: Total MC events in file   :"<<total_events<<"\n";
		std::cout<<"MC_NC1Gamma_Filter: Total events looped over  :"<<looped_events<<"\n";
		std::cout<<"MC_NC1Gamma_Filter: Events kept               :"<<kept_events<<"\n";

    return true;
  }

}
#endif
