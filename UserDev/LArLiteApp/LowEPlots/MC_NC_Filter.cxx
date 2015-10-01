#ifndef LARLITE_MC_NC_FILTER_CXX
#define LARLITE_MC_NC_FILTER_CXX

#include "MC_NC_Filter.h"

namespace larlite {

  bool MC_NC_Filter::initialize() {

    TPC.Min(0,
            -(::larutil::Geometry::GetME()->DetHalfHeight()),
            0);

    TPC.Max(2*(::larutil::Geometry::GetME()->DetHalfWidth()),
            ::larutil::Geometry::GetME()->DetHalfHeight(),
            ::larutil::Geometry::GetME()->DetLength());

    return true;
  }
  
  bool MC_NC_Filter::analyze(storage_manager* storage) {
    
    //Grab the MCTruth
    auto ev_mctruth_v = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth_v) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    bool ret = false;
    bool NC = false;
    
    for(auto &ev_mctruth : *ev_mctruth_v){      
      //Checks if the neutrino exchanged a W or a Z during the interaction 
      if(ev_mctruth.GetNeutrino().CCNC() == 1){NC = true;}
      else{NC = false;}
      
      // If there are any detector external interactions veto the event
      if(!(TPC.Contain(ev_mctruth.GetNeutrino().Nu().Trajectory().back().Position()))){
	return false;       
      }//What you think?!
      
    }//loops through MCTruth
    
    //Enforce CC interaction channel
    if(NC){ret = true;}
    else{ret = false;}
    
    //check the status of the ret variable
    return ret;  
    
  }
  
  bool MC_NC_Filter::finalize() {

  
    return true;
  }

}
#endif
