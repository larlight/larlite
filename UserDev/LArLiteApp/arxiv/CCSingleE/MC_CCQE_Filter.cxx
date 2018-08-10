#ifndef LARLITE_MC_CCQE_FILTER_CXX
#define LARLITE_MC_CCQE_FILTER_CXX

#include "MC_CCQE_Filter.h"

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
namespace larlite {


  // Constructor to set default values for cuts, can be modified with setters
  MC_CCQE_Filter::MC_CCQE_Filter(){
      _name="MC_CCQE_Filter"; 
      _fout=0; 
      _pdgdb=0;

      _n_mcshowers_requested = 1;
      _n_mctracks_requested = 1;
      _min_mcshower_E_requested = 50.;
      _min_mctrack_E_requested = 50.;
  }

  bool MC_CCQE_Filter::initialize() {
    
    total_events = 0;
    kept_events = 0;

    if(!_pdgdb)
      _pdgdb = new TDatabasePDG();

    return true;

  }
  
  bool MC_CCQE_Filter::analyze(storage_manager* storage) {

    //Grab the MCTruth
    auto ev_mctruth = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }
    //Grab the MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }
    //Grab the MCTracks
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
    if(!ev_mctrack) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }

    total_events++;

    //Enforce CCQE interaction channel
    if( ev_mctruth->at(0).GetNeutrino().InteractionType() != 1001 )
      return false;
    
    size_t n_viable_mcshowers = 0;
    size_t n_viable_mctracks = 0;

    //Ask mcshower have > 50 MEV deposited in detector
    for(auto const& mcs : *ev_mcshower)
      if(mcs.DetProfile().E() > 50.)
	n_viable_mcshowers++;
    
    //Ask mctrack particles have > 50 MeV *kinetic* energy.
    for(auto const& mct: *ev_mctrack) {
      auto iter = _massdb.find(mct.PdgCode());
      if(iter == _massdb.end()) {
	_massdb[mct.PdgCode()] = _pdgdb->GetParticle(mct.PdgCode())->Mass()*1000.;
	iter = _massdb.find(mct.PdgCode());
      }      
      if( (mct.Start().E() - (*iter).second) > 50. )
	n_viable_mctracks++;
    }
    // Want specifically 1 mcshower, 0 mctracks (simplest topology)
    if(n_viable_mcshowers != 1)
      return false;
    if(n_viable_mctracks != 0)
      return false;

    /*
    std::cout<<"MC_CCQE_Filter:: Kept event."<<std::endl;

    std::cout<<"# MCShowers = "<<ev_mcshower->size()
	     <<". Energies are :"<<std::endl;
    for(auto const& mcs : *ev_mcshower)
      std::cout<<"     (PDG "<<mcs.PdgCode()<<", Mass "
	       <<_pdgdb->GetParticle(mcs.PdgCode())->Mass()<<"): "
	       <<mcs.DetProfile().E()<<std::endl;

    std::cout<<"# MCTracks = "<<ev_mctrack->size()
	     <<". Energies are :"<<std::endl;
    for(auto const& mct : *ev_mctrack)
      std::cout<<"     (PDG "<<mct.PdgCode()<<", Mass "
	       <<_pdgdb->GetParticle(mct.PdgCode())->Mass()<<"): "
	       <<mct.Start().E()<<std::endl;
    */
    kept_events++;
    
    return true;
  }

  bool MC_CCQE_Filter::finalize() {

    std::cout<<"MC_CCQE_Filter: Total events = "<<total_events<<std::endl;
    std::cout<<"MC_CCQE_Filter: Final kept events = "<<kept_events<<std::endl;

    delete _pdgdb;

    return true;
  }

}
#endif
