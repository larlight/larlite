#ifndef MC_1P1E_FILTER_CXX
#define MC_1P1E_FILTER_CXX

#include "MC_1P1E_Filter.h"

namespace larlite {

  size_t total_events = 0;
  size_t kept_events = 0;


  bool MC_1P1E_Filter::initialize() {

    return true;

  }
  
  bool MC_1P1E_Filter::analyze(storage_manager* storage) {
  

    //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    total_events++;
    
    //Enforce that there is exactly 1 electron and 1 proton, each above 20MeV kinetic energy
    //I don't care about neutrons, weird quarks, the neutrino, etc.
    size_t n_electrons = 0;
    size_t n_protons = 0;

    for(auto const& particle : ev_mctruth->at(0).GetParticles()){    

      double KE = particle.Trajectory().at(0).E()-particle.Mass();

      if(abs(particle.PdgCode()) == 11 && KE > 0.02)
	n_electrons++;
      if(abs(particle.PdgCode()) == 2212 && KE > 0.02)
	n_protons++;

    }

    if(n_electrons != 1 || n_protons != 1)
      return false;

    
    //If you get here, the event has 1 good electron and 1 good proton.
    kept_events++;
    return true;  
  }

  bool MC_1P1E_Filter::finalize() {

    std::cout<<"MC_1P1E_Filter: Total events = "<<total_events<<std::endl;
    std::cout<<"MC_1P1E_Filter: Final kept events = "<<kept_events<<std::endl;

    return true;
  }

}
#endif
