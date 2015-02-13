#ifndef LARLITE_MC_CCNUMU_FILTER_CXX
#define LARLITE_MC_CCNUMU_FILTER_CXX

#include "MC_CCnumu_Filter.h"

namespace larlite {

  bool MC_CCnumu_Filter::initialize() {

    total_events = 0;
    kept_events = 0;

    return true;
  }
  
  bool MC_CCnumu_Filter::analyze(storage_manager* storage) {
  
    //Using MCTruth Information search for numu CC interactions
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
        if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    total_events++;

    bool ret = true;

    //Require the interaction is numuCC 
    if(ev_mctruth->at(0).GetNeutrino().CCNC() == 0 && 
       abs(ev_mctruth->at(0).GetNeutrino().Nu().PdgCode()) == 14){  
      ret = true;
      
    }
    else
      ret = false;

    //check the status of the ret variable
    if (ret && !_flip)
      kept_events++;
    if (!ret && _flip)
      kept_events++;
    if (_flip)
      return (!ret);
    return ret;  
    
  }

  bool MC_CCnumu_Filter::finalize() {

    std::cout << "MC_CCnumu_Filter : Total events = \t" << total_events << std::endl;
    std::cout << "MC_CCnumu_Filter : Retained events = \t" << kept_events << std::endl;
   
    return true;
  }

}
#endif
