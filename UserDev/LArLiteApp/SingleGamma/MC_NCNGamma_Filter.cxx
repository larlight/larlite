#ifndef LARLITE_MC_NCNGAMMA_FILTER_CXX
#define LARLITE_MC_NCNGAMMA_FILTER_CXX

#include "MC_NCNGamma_Filter.h"

namespace larlite {

  bool MC_NCNGamma_Filter::initialize() {

    total_events = 0;
    looped_events = 0;
    kept_events = 0;

    return true;
  }
  
  bool MC_NCNGamma_Filter::analyze(storage_manager* storage) {
  
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    total_events++;
      
    bool ret = true;

    for(auto const& part : ev_mctruth->at(0).GetParticles()){
      if(abs(part.PdgCode()) == 22){
	ret=true;}
    }
  
    
    if( ev_mctruth->at(0).GetNeutrino().CCNC() != 1)
      ret=false;

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

  bool MC_NCNGamma_Filter::finalize() {

    std::cout<<"MC_NC1Gamma_Filter: Total MC events in file   :"<<total_events<<"\n";
    std::cout<<"MC_NC1Gamma_Filter: Total events looped over  :"<<looped_events<<"\n";
    std::cout<<"MC_NC1Gamma_Filter: Events kept               :"<<kept_events<<"\n";

    return true;
  }
}
#endif
