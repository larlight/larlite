#ifndef PI0CONTAINMENTFILTER_CXX
#define PI0CONTAINMENTFILTER_CXX

#include "Pi0ContainmentFilter.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
namespace larlite {

  bool Pi0ContainmentFilter::initialize() {

    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@INITIALIZE FILTER@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

    _total_events = 0;
    _kept_events = 0;

    _n_pi0 = 0; 
    _n_E = 0;
    _n_detProf = 0;

   // SetEC(0.75) ;
    //SetEnergyCut(20);

    return true;
  }
  
  bool Pi0ContainmentFilter::analyze(storage_manager* storage) {
  

    //Grab the MCTruth
    auto ev_mctruth  = storage->get_data<event_mctruth>("generator");
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }

    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }

    _total_events++;
    
    //Enforce that there is exactly 1 pi0
    //I don't care about neutrons, weird quarks, the neutrino, etc.
    size_t n_pi0 = 0;
    
    for(auto const& particle : ev_mctruth->at(0).GetParticles()){

//      double KE = particle.Trajectory().at(0).E()-particle.Mass();
      if(abs(particle.PdgCode()) == 111)// && KE > 0.02) //GeV
	n_pi0++;
    }
    if (n_pi0 != 1){
      _n_pi0++ ;
      return false;
      }

    // Enforce there to be 2 mcshowers both > 95% contained
    event_mcshower selected;
    size_t n_shr = 0;
    for (auto const& shr : *ev_mcshower){
      if ( shr.Start().E() > _Energy ) //MeV
	selected.push_back(shr);
    }
    if (selected.size() != 2){
      _n_E++;
      return false;
      }

    for (auto const& sh : selected){
      if ( (sh.DetProfile().E() / sh.Start().E()) > _EC )
	n_shr +=1 ;
	}
    if (n_shr != 2){
      _n_detProf++;
      return false;
      }
    

    //If you get here, the event has 1 pi0 with 2 showers   
    _kept_events++;
    return true;  

  }

  bool Pi0ContainmentFilter::finalize() {

    std::cout<< "Pi0 Containment Filter: Total events = " << _total_events << std::endl;
    std::cout<< "Pi0 Containment Filter: Final kept events = " << _kept_events << std::endl;
  
    std::cout<<"Here's where we lost them:"
	     <<"\nWrong numer pi0s            : "<<_n_pi0
	     <<"\nShowers with energy < 20 MeV: "<<_n_E
	     <<"\nShowers with EC <"<<_EC<<"       : "<<_n_detProf<<std::endl;

    return true;
  }

}
#endif
