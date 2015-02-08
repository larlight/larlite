#ifndef LARLITE_MC_CC1E_FILTER_CXX
#define LARLITE_MC_CC1E_FILTER_CXX

#include "MC_CC1E_Filter.h"

namespace larlite {

  bool MC_CC1E_Filter::initialize() {
    
    total_events = 0;
    kept_events = 0;
    
    return true;

  }
  
  bool MC_CC1E_Filter::analyze(storage_manager* storage) {
  
    //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    total_events++;

    bool ret = true;
    
    //Enforce CC interaction channel
    if( ev_mctruth->at(0).GetNeutrino().CCNC() != 0 )
      ret = false;
    
    //Enforce that there is exactly 1 electron, above 20MeV kinetic energy
    //I don't care about neutrons, weird quarks, the neutrino, etc.
    size_t n_electrons = 0;
    
    for(auto const& particle : ev_mctruth->at(0).GetParticles()){

      //Note: this KE is in units of GEV!
      double KE = particle.Trajectory().at(0).E()-particle.Mass();

      if( abs(particle.PdgCode()) == 11 && 
	  KE > 0.02 && 
	  particle.StatusCode() == 1
      )
	n_electrons++;

    }

    if( n_electrons != 1 )
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

  bool MC_CC1E_Filter::finalize() {

    std::cout<<"MC_CC1E_Filter: Total events = "<<total_events<<std::endl;
    std::cout<<"MC_CC1E_Filter: Final kept events = "<<kept_events<<std::endl;

    return true;
  }

}
#endif
