#ifndef LARLITE_TRACK_OFFLENGTH_CXX
#define LARLITE_TRACK_OFFLENGTH_CXX

#include "Track_OffLength.h"

namespace larlite {

  bool Track_OffLength::initialize() {

    OL = new TH1D("OffLength", "", 1000, 0, 20);
    dedx = new TH1D("dEdx", "", 1000, 0, 10);
    
return true;
  }
  
  bool Track_OffLength::analyze(storage_manager* storage) {
  
    auto tracks_v = storage->get_data<event_mctrack>("mcreco");

    std::cout << "N tracks : " << tracks_v->size() << std::endl; 

    for(auto track :  *tracks_v){

      if(track.size() == 0) {continue;} 

      dEdx = track.dEdx();
      of = track.OffLength();

      int gd = 0, all = 0;

      for(int i = 0; i < dEdx.size(); i++){
	
	all++;
	if(dEdx.at(i) != 0){
	  dedx->Fill(dEdx.at(i));
	}
	//	if(dEdx.at(i) < 2.3 && dEdx.at(i) > 2.1){
	  
	//	  gd++;
	  
	  for(auto L : of.at(i)){
	    
	    OL->Fill(L);

	  }
	  //	}

      }
      
      std::cout << "All " << all << " Good " << gd << std::endl; 

    }
    return true;
  }

  bool Track_OffLength::finalize() {

    OL->Write();
    dedx->Write();

    return true;
  }

}
#endif
