#ifndef LARLITE_REMOVETRKLIKEHITS_CXX
#define LARLITE_REMOVETRKLIKEHITS_CXX

#include "RemoveTrkLikeHits.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {

  bool RemoveTrkLikeHits::initialize() {

    
    return true;
  }
  
  bool RemoveTrkLikeHits::analyze(storage_manager* storage) {

    // produce new shower-like hits
    auto ev_shr_hits = storage->get_data<event_hit>("shrlike");

    // get PFParticles
    auto ev_pfpart = storage->get_data<event_pfpart>(_pfpart_producer);

    storage->set_id(storage->run_id(), storage->subrun_id(), storage->event_id());

    // if no clusters -> exit
    if ( (!ev_pfpart) or (ev_pfpart->size() == 0) ){
      std::cout << "No PFPart -> skip event" << std::endl;
      return false;
    }

    // get clusters associated w/ the PFParticles
    larlite::event_cluster *ev_clus = nullptr;
    auto & clusters_ass_to_pfpart = storage->find_one_ass(ev_pfpart->id(), ev_clus, ev_pfpart->name());

    // if no clusters -> exit
    if ( (!ev_clus) or (ev_clus->size() == 0) ){
      std::cout << "No Cluster -> skip event" << std::endl;
      return false;
    }

    // get hits associated with the clusters
    larlite::event_hit *ev_hit = nullptr;
    auto & hits_ass_to_cluster    = storage->find_one_ass(ev_pfpart->id(), ev_hit, ev_pfpart->name());

    // if no hits -> return false
    if ( (!ev_hit) or (ev_hit->size() == 0) ){
      std::cout << "No Hit -> skip event" << std::endl;
      return false;
    }

    /*
    // loop through PFParticles
    for (size_t pfp_i = 0; pfp_i < ev_pfpart->size(); pfp_i++){

      auto const& pfp = ev_pfpart->at(pfp_i);

      // if this pfparticle is track-like -> skip
      if (pfp.PdgCode() == 13)
	continue;

      // otherwise, go and find the clusters associated to this PFPart
      auto const& ass_clus_idx_v = clusters_ass_to_pfpart[pfp_i];

      for (auto const& ass_clus_idx : ass_clus_idx_v){

	std::cout << "Added cluster..." << std::endl;

	// and finally the hits associated to that PFPart
	auto ass_hit_idx_v = hits_ass_to_cluster[ass_clus_idx];
	std::cout << "w/ " << ass_hit_idx_v.size() << " hits" << std::endl;

	// add all these hits to the new producer
	for (auto const& hit_idx : ass_hit_idx_v)
	  ev_shr_hits->emplace_back( ev_hit->at( hit_idx ) );

      }// for all clusters
    }// for all PFParts
    */
  
    return true;
  }

  bool RemoveTrkLikeHits::finalize() {

  
    return true;
  }

}
#endif
