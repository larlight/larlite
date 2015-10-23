#ifndef LARLITE_MC_CCNUE_FILTER_CXX
#define LARLITE_MC_CCNUE_FILTER_CXX

#include "MC_CCnue_Filter.h"

namespace larlite {

  bool MC_CCnue_Filter::initialize() {

    TPC.Min(0,
            -(::larutil::Geometry::GetME()->DetHalfHeight()),
            0);

    TPC.Max(2*(::larutil::Geometry::GetME()->DetHalfWidth()),
            ::larutil::Geometry::GetME()->DetHalfHeight(),
            ::larutil::Geometry::GetME()->DetLength());

    _n_total_events = 0;
    _n_kept_events = 0;

    return true;
  }
  
  bool MC_CCnue_Filter::analyze(storage_manager* storage) {
  
    //Grab the MCTruth
    auto ev_mctruth_v = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth_v) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }
    
    _n_total_events++;

    bool ret = false;
    bool CC = false;
    bool nue = false;

    for(auto &ev_mctruth : *ev_mctruth_v){
     
      //Enforce CC interaction channel
      if( ev_mctruth.GetNeutrino().CCNC() == 0 ){
	CC = true;}

      // If there are any detector external interactions veto the event
      if(!(TPC.Contain(ev_mctruth.GetNeutrino().Nu().Trajectory().back().Position()))){
	return false;}
      
      if(ev_mctruth.GetNeutrino().Nu().PdgCode() == 12){
	nue = true;}        
    }
    
    //check the status of the ret variable
    if(CC && nue){ret = true;} 
    else{ret = false;}

    if (ret) _n_kept_events++;
    return ret;  
    

  }

  bool MC_CCnue_Filter::finalize() {
    std::cout<<_n_total_events<<" total events analyzed, "<<_n_kept_events<<" events passed MC_CCnue_Filter."<<std::endl;
  
    return true;
  }

}
#endif
