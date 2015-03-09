#ifndef LARLITE_MC_NC_FILTER_CXX
#define LARLITE_MC_NC_FILTER_CXX

#include "MC_NC_Filter.h"

namespace larlite {

  bool MC_NC_Filter::initialize() {

    total_events = 0;
    kept_events = 0;
    n_pi = 0;
    kept_events_wPi = 0;

    return true;
  }
  
  bool MC_NC_Filter::analyze(storage_manager* storage) {
  
  //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    total_events++;

    bool ret = true;
    
    //Enforce NC interaction channel
    if( ev_mctruth->at(0).GetNeutrino().CCNC() != 1 )
      ret = false;
    
    for(auto const& part : ev_mctruth->at(0).GetParticles()){

      if(abs(part.PdgCode()) == 111 &&
	 part.StatusCode()   == 1)
	n_pi++;
      

    }


    //check the status of the ret variable
    if (ret && !_flip)
      kept_events++;
    if (!ret && _flip)
      kept_events++;
    if (ret && !_flip && n_pi > 0)
      kept_events_wPi++;
    if (!ret && _flip && n_pi > 0)
      kept_events_wPi++;
    if (_flip)
      return (!ret);
    return ret;    
  
  }

  bool MC_NC_Filter::finalize() {

    std::cout<<"MC_NC_Filter: Total events = \t"<<total_events<<std::endl;
    std::cout<<"MC_NC_Filter: Final kept events = \t"<<kept_events<<std::endl;
    std::cout<<"MC_NC_Filter: Final kept events W/ pi0 = \t"<<kept_events_wPi<<std::endl;

    return true;
  }

}
#endif
