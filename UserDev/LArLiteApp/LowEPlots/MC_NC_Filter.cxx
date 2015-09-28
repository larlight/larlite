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

    //Grab the MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }

    bool ret = true;
    
    bool isCharPion = false;
    bool isPhoton = false;
    bool NC = false;
    
    for(auto &ev_mctruth : *ev_mctruth_v){      
      //Checks if the neutrino exchanged a W or a Z during the interaction 
      if(ev_mctruth.GetNeutrino().CCNC() == 1){
	NC = true;}
      else{
	NC = false;}
      
      // If there are any detector external interactions veto the event
      if(!(TPC.Contain(ev_mctruth.GetNeutrino().Nu().Trajectory().back().Position()))){
	return false;       
      }//What you think?!
      
    }//loops through MCTruth
    
    for(auto &mcs : *ev_mcshower){
      //Checks for prompt photons from delta or neutral pions
      if(mcs.PdgCode() == 22 && 
	 (abs(mcs.AncestorPdgCode()) == 2224 || 
	  abs(mcs.AncestorPdgCode()) == 2214 || 
	  abs(mcs.AncestorPdgCode()) == 2114 || 
	  abs(mcs.AncestorPdgCode()) == 111))
	isPhoton = true;
      //Checks for electrons coming from charged pion->muon->michel decay
      if(abs(mcs.PdgCode()) == 11 &&
	 abs(mcs.MotherPdgCode()) == 13 &&
	 abs(mcs.AncestorPdgCode()) == 211)
	isCharPion = true;	      
    }//loops through MCShowers
    
    
    //Enforce CC interaction channel
    if(NC){
      ret = true;}
    else{ret = false;}
    
    //check the status of the ret variable
    //   if (_flip)
    //  return (!ret);
    return ret;  
    
  }
  
  bool MC_NC_Filter::finalize() {

  
    return true;
  }

}
#endif
