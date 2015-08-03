#ifndef LARLITE_NP_FILTER_CXX
#define LARLITE_NP_FILTER_CXX

#include "NP_Filter.h"

namespace larlite {

  bool NP_Filter::initialize() {
    
    return true;

  }
  
  bool NP_Filter::analyze(storage_manager* storage) {
  
    //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
       }
    


    return 0;  
  }

  bool NP_Filter::finalize() {

    return true;
  }

}
#endif
