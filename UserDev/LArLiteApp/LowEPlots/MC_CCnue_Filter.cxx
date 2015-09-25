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

    return true;
  }
  
  bool MC_CCnue_Filter::analyze(storage_manager* storage) {
  
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
	return false;

      // If there are any detector external interactions veto the event
      if(!(TPC.Contain(ev_mctruth.GetNeutrino().Nu().Trajectory().back().Position())))
	return false;       
      
            
      //Enforce that there is exactly 1 electron, above 20MeV kinetic energy
      //I don't care about neutrons, weird quarks, the neutrino, etc.
      size_t n_electrons = 0;
      
      for(auto const& particle : ev_mctruth.GetParticles()){
	
	if( abs(particle.PdgCode()) == 11 && 
	    particle.StatusCode() == 1)
	  n_electrons++;
	
      }
      
      if( n_electrons > 0)
	ret = false;

      
      if(ret == true) continue;

    }  
      
      //check the status of the ret variable
      if (_flip)
	return (!ret);
      return ret;  
    
  }

  bool MC_CCnue_Filter::finalize() {

  
    return true;
  }

}
#endif
