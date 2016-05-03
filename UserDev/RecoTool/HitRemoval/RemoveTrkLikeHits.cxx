#ifndef LARLITE_REMOVETRKLIKEHITS_CXX
#define LARLITE_REMOVETRKLIKEHITS_CXX

#include "RemoveTrkLikeHits.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/vertex.h"
#include "DataFormat/PiZeroROI.h"
namespace larlite {

bool RemoveTrkLikeHits::initialize() {


  return true;
}

bool RemoveTrkLikeHits::analyze(storage_manager* storage) {

  // produce new shower-like hits
  auto ev_shr_hits = storage->get_data<event_hit>("shrlike");

  //produce new shower-like clusters
  auto ev_shr_clus = storage->get_data<event_cluster>("shrlike");

  // make the association
  auto *ev_clus_hit_ass = storage->get_data<event_ass>("shrlike");
  std::vector<std::vector<unsigned int> > shr_clus_hit_ass;

  // get PFParticles
  auto ev_pfpart = storage->get_data<event_pfpart>(_pfpart_producer);

  // additionally, save a vtx object associated with the neutrino-vtx stored in the Pi0ROI
  auto _roi_producer = "pizerofilter";
  auto *ev_roi = storage->get_data<event_PiZeroROI>(_roi_producer);

  auto *ev_vtx_roi = storage->get_data<event_vertex>(_roi_producer);

  // produce ROIT -> vtx association
  auto *ev_roi_vtx_ass = storage->get_data<event_ass>(_roi_producer);
  std::vector<std::vector<unsigned int> > roi_vtx_ass;


  storage->set_id(storage->run_id(), storage->subrun_id(), storage->event_id());

  // if no clusters -> exit
  if ( (!ev_pfpart) or (ev_pfpart->size() == 0) ) {
    std::cout << "No PFPart -> skip event" << std::endl;
    return false;
  } else { std::cout << "Found PFPart: " << _pfpart_producer << "\n"; }

  // get clusters associated w/ the PFParticles
  larlite::event_cluster *ev_clus = nullptr;
  auto & clusters_ass_to_pfpart = storage->find_one_ass(ev_pfpart->id(), ev_clus, ev_pfpart->name());

  auto const& ev_ass = storage->get_data<event_ass>(ev_pfpart->name());
  ev_ass->list_association();

  // if no clusters -> exit
  if ( (!ev_clus) or (ev_clus->size() == 0) ) {
    std::cout << "No Cluster -> skip event" << std::endl;
    return false;
  } else { std::cout << "Found cluster: " << ev_clus->name() << "\n"; }

  // get hits associated with the clusters
  larlite::event_hit *ev_hit = nullptr;
  auto & hits_ass_to_cluster = storage->find_one_ass(ev_clus->id(), ev_hit, ev_clus->name());

  // if no hits -> return false
  if ( (!ev_hit) or (ev_hit->size() == 0) ) {
    std::cout << "No Hit -> skip event" << std::endl;
    return false;
  } else { std::cout << "Hit found: " << ev_hit->name() << "\n";}


  bool _save_unclustered_hits = true;
  std::vector<size_t> hits_tobe_removed;

  // loop through PFParticles

  // NOTE: clusters_ass_to_pfpart is being looped over not PFPart size itself

  for (size_t pfp_i = 0; pfp_i < clusters_ass_to_pfpart.size(); pfp_i++) {

    auto const& pfp = ev_pfpart->at(pfp_i);

    // if this pfparticle is track-like -> skip
    bool track = false;

    if (pfp.PdgCode() == 13) track = true;

    // otherwise, go and find the clusters associated to this PFPart
    auto const& ass_clus_idx_v = clusters_ass_to_pfpart.at(pfp_i);
    
    std::cout << "ass clus idx " << ass_clus_idx_v.size() << "\n";

    for (auto const& ass_clus_idx : ass_clus_idx_v) {

      // and finally the hits associated to that PFPart
      auto ass_hit_idx_v = hits_ass_to_cluster[ass_clus_idx];

      // add all these hits to the new producer
      for (auto const& hit_idx : ass_hit_idx_v) {
        if ( ! track ) ev_shr_hits->emplace_back( ev_hit->at( hit_idx ) );
        else hits_tobe_removed.push_back( hit_idx );
      }
      if ( ! track ) shr_clus_hit_ass.push_back( ass_hit_idx_v );
    }// for all clusters
  }// for all PFParts

  //it's hardcoded
  if (_save_unclustered_hits) {

    for (size_t idx = 0; idx < ev_hit->size(); idx++) {

      if ( std::find(hits_tobe_removed.begin(),
                     hits_tobe_removed.end(),
                     idx) == hits_tobe_removed.end() ) // not found, add to shower like hits

        ev_shr_hits->emplace_back( ev_hit->at( idx ) );

    }// for all hits
  }// if we want to save unclustered hits

  // save cluster -> hit associations
  ev_clus_hit_ass->set_association(ev_shr_clus->id(),
                                   product_id(data::kHit, ev_hit->name()),
                                   shr_clus_hit_ass);

  // save PiZeroROI -> vtx association
  ev_roi_vtx_ass->set_association(ev_roi->id(),
                                  product_id(data::kVertex, ev_vtx_roi->name()),
                                  roi_vtx_ass);


  return true;
}

bool RemoveTrkLikeHits::finalize() {


  return true;
}

}
#endif
