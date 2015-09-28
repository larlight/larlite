#ifndef LARLITE_MC_DIRT_FILTER_CXX
#define LARLITE_MC_DIRT_FILTER_CXX

#include "MC_dirt_Filter.h"
#include "DataFormat/mcshower.h"

namespace larlite {

  bool MC_dirt_Filter::initialize() {

    TPC.Min(0,
            -(::larutil::Geometry::GetME()->DetHalfHeight()),
            0);

    TPC.Max(2*(::larutil::Geometry::GetME()->DetHalfWidth()),
            ::larutil::Geometry::GetME()->DetHalfHeight(),
            ::larutil::Geometry::GetME()->DetLength());

    return true;
  }
  
  bool MC_dirt_Filter::analyze(storage_manager* storage) {
  
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    if(!ev_mctruth->size()){
      print(larlite::msg::kERROR,__FUNCTION__,Form("MCTruth size is zero?? Last time this happened to me, it was because I was trying to read in very old larlite files that had a different data format."));
      return false;
    }

    bool ret = true;

    //If ANY neutrinos interact within the TPC, skip this entire "event".
    for(auto const mct : *ev_mctruth){
      if(TPC.Contain(mct.GetNeutrino().Nu().Trajectory().back().Position())){
	ret = false;
      }
      if(ret == false) continue;
    }
    
      return ret;  

  }

  bool MC_dirt_Filter::finalize() {

  
    return true;
  }

}
#endif
