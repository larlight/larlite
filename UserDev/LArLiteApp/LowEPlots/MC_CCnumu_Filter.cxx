#ifndef LARLITE_MC_CCNUMU_FILTER_CXX
#define LARLITE_MC_CCNUMU_FILTER_CXX

#include "MC_CCnumu_Filter.h"

namespace larlite {

  bool MC_CCnumu_Filter::initialize() {

    return true;
  }
  
  bool MC_CCnumu_Filter::analyze(storage_manager* storage) {
    
    //Grab the MCTruth
    auto ev_mctruth_v = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth_v) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }
    
    bool ret = true;
    
    for(auto &ev_mctruth : *ev_mctruth_v){
      
      //Enforce CC interaction channel
      if( ev_mctruth.GetNeutrino().CCNC() != 0 )
	ret = false;
      
      //Enforce that there is exactly 1 electron, above 20MeV kinetic energy
      //I don't care about neutrons, weird quarks, the neutrino, etc.
      size_t n_muons = 0;
      
      for(auto const& particle : ev_mctruth.GetParticles()){
	
	if( abs(particle.PdgCode()) == 13 && 
	    particle.StatusCode() == 1)
	  n_muons++;
      }
      
      if( n_muons > 0)
	ret = false;
      
      if(ret == true) continue;
      
    }  
    
    //check the status of the ret variable
    return ret;  
  

  }

  bool MC_CCnumu_Filter::finalize() {

  
    return true;
  }

}
#endif
