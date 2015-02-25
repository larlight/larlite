#ifndef TESTCOSMICTAG_CXX
#define TESTCOSMICTAG_CXX

#include "testCosmicTag.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/cosmictag.h"

namespace larlite {

  bool testCosmicTag::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool testCosmicTag::analyze(storage_manager* storage) {
  
  		auto ev_tag = storage->get_data<event_cosmictag>("mcshowermctag");
		auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");

		auto ass_info = ev_mcshower->association(ev_tag->id());

		int count=0;

         for(auto const& mcs : ass_info){ 
			count++;
			std::cout<<"MCShower is: "<<count<<std::endl;
			for(auto const& i : mcs ){
				std::cout<<"tags: "<<ev_tag->at(i).CosmicScore()<<std::endl ;
		 	// std::cout<<"Shower X: "<<ev_mcshower->at(i).Start().X()<<std::endl ;

				}

			}
  
    return true;
  }

  bool testCosmicTag::finalize() {

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

}
#endif
