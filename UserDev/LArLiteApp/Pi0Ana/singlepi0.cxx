#ifndef LARLITE_SINGLEPI0_CXX
#define LARLITE_SINGLEPI0_CXX

#include "singlepi0.h"

namespace larlite {

  bool singlepi0::initialize() {

        _total_events = 0 ;
        _passed_events = 0;
	
    return true;
  }
  
  bool singlepi0::analyze(storage_manager* storage) {
        // Bring in the info for the event
        auto mctruth = storage->get_data<event_mctruth>("generator");
            if(!mctruth) {
                print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
                return false;}// if no mctruth
        auto mcpart = mctruth->at(0).GetParticles();
        auto mcnu = mctruth->at(0).GetNeutrino();

        _total_events++;

        size_t totalpi0 =0;
        size_t cmeson =0;

	// If it is ccnc 
	if(_ccnc != mcnu.CCNC()) return false;

	//loop over all particles to look topology 1 pi0 and no pi+/-
	for(auto const& p : mcpart){
		if(p.PdgCode()==111 && p.StatusCode()==1) totalpi0+=1;
		if(abs(p.PdgCode())==211 && p.StatusCode()==1) cmeson+=1;
		   }// loop over mcparts

	// This will return a bool for signal events
	if(_Sig){
		// Fill in the topo you want to look at 
		if(_Topo == 0){if(totalpi0!=1) return false;}
		if(_Topo ==1 ){if(totalpi0!=1 || cmeson!=0) return false;}
		_passed_events++;
	    return true;
		}

	// Repeat with negatives if !_Sig
		if(_Topo == 0){if(totalpi0!=1) return true;}
		if(_Topo ==1 ){if(totalpi0!=1 || cmeson!=0) return true;}
		_passed_events++;
	    return false;

		







  }

  bool singlepi0::finalize() {

       std::cout<<"\n =====================\n Summary of Generator Events \n ====================="<<std::endl;
        std::cout<<" Total Events generated  " << _total_events<<std::endl;
	std::cout<<" We are looking at ccnc = "<<_ccnc<<std::endl;
        std::cout<<" Total Single pi0 Topology :"<<_Topo<<":  Has this many events: " << _passed_events<<std::endl;
        std::cout<<" Percent of sample " << (double) _passed_events/_total_events<<std::endl;
       std::cout<<"\n =====================\n End  of Generator Events \n ========================"<<std::endl;

  
    return true;
  }

}
#endif
