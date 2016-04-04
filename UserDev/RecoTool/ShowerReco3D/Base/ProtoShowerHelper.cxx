#ifndef PROTOSHOWERHELPER_CXX
#define PROTOSHOWERHELPER_CXX

#include "ProtoShowerHelper.h"

namespace showerreco {

  ProtoShowerHelper::ProtoShowerHelper()
    : _proto_shower_alg(nullptr)
  {}
  
  // From the storage manager, and with the pfpart producer name, generate
  // a vector of all the protoshowers in the event.
  void ProtoShowerHelper::GenerateProtoShowers(::larlite::storage_manager* storage,
					       const std::string &pfpart_producer_name,
					       std::vector<ProtoShower> & proto_showers,
					       std::vector<unsigned int> showerLikePFParts) {
    
    
    // Clear the current proto-shower container.
    proto_showers.clear();
    
    // First, get the pfparticles and verify they are there.
    auto * ev_pfpart = storage -> get_data<larlite::event_pfpart>(pfpart_producer_name);
    
    // Check if there are pfparticles:
    if (!ev_pfpart) {
      return;
    }
    if (ev_pfpart -> size() == 0) {
      return;
    }
    
    // Now we have pfparticles, so start to build protoshowers
    
    // In general, only build the ones with index specified by showerLikePFParts.
    // If it's not specified, build them all.
    
    
    if (showerLikePFParts.size() == 1) {
      
      if (showerLikePFParts.at(0) == 999999) {
	
	
	showerLikePFParts.resize(ev_pfpart -> size());
	for (unsigned int i = 0; i < showerLikePFParts.size(); ++i)
	  {
	    showerLikePFParts.at(i) = i;
	  }
      }
      
    }
    
    if (showerLikePFParts.size() == 0) {
      return;
    }
    
    
    proto_showers.resize(showerLikePFParts.size());
    
    
    
    // For each data product, fetch it and feed
    // it into the proto showers if possible
    //
    // That means using cluster_params and cluster3D_params if needed
    
    // Try to get the clusters if they are there
    ::larlite::event_cluster * ev_clust = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());
    
    // Want to get the hits from the cluster too:
    ::larlite::event_hit * ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());
    
    // Get any possible spacepoints
    larlite::event_spacepoint * ev_sps = nullptr;
    auto const& ass_sps_v = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());
    
    // Do the vertex:
    larlite::event_vertex * ev_vertex = nullptr;
    auto const& ass_vertex_v = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());
    
    
    // Check that the cluster associations are the same length as the pfparticle list
    if (ass_cluster_v.size() == ev_pfpart -> size()) {
      
      // From here, we have the list of clusters for each particle.
      // We can build cluster_params and add it to the proto_showers
      //
      // We should have two to three sets of clusters per pfparticle
      
      
      // use the indexes stored in showerLikePFParts to loop through the clusters
      for (size_t i = 0; i < showerLikePFParts.size(); i ++ ) {
	
	// this is the index of the PFParticle for the protoshower we are trying to build
	size_t proto_shower_pfpart = showerLikePFParts.at(i);
	
	// prepare a vector of clusters
	std::vector<::larlite::cluster> cluster_v;
	// prepare a vector of vector of hits
	// 1st index is the cluster index
	// then each subvector contains the larlite hits for that cluster
	std::vector< std::vector< ::larlite::hit> > cluster_hits_v_v;
	// if there are any clusters:
	if ( ev_clust && ev_clust->size() != 0) {
	  for (auto j_clust : ass_cluster_v.at(proto_shower_pfpart)) {
	    cluster_v.push_back( ev_clust->at( j_clust ) );
	    std::vector< ::larlite::hit > cluster_hits_v;
	    auto const& ass_hits = ass_hit_v.at(j_clust);
	    for (auto const&  hit_idx : ass_hits)
	      cluster_hits_v.push_back( ev_hit->at(hit_idx) );
	    cluster_hits_v_v.push_back( cluster_hits_v );
	  }// for all clusters associated to the PFParticle
	}// if clusters were found
	
	
	// fill vector fo spacepoints to be passed on to algorithm
	std::vector< ::larlite::spacepoint > sps_v;
	// if there are any spacepoints:
	if ( ev_sps && ev_sps -> size() != 0) {
	  // get spacepoints associated to this PFParticle
	  auto & sps_idx_v = ass_sps_v.at(proto_shower_pfpart);
	  for (auto const& idx : sps_idx_v)
	    sps_v.push_back( ev_sps->at( idx ) );
	}// if spacepoints were found
	
	// fill vertices, if any associated
	std::vector< ::larlite::vertex > vtx_v;
	if ( ev_vertex && ev_vertex -> size() != 0) {
	  auto & vtx_idx_v = ass_vertex_v.at(proto_shower_pfpart);
	  for (auto idx : vtx_idx_v) {
	    double xyz[3];
	    vtx_v.push_back( ev_vertex->at(idx) );
	  }
	}// if there are vertices
	
	_proto_shower_alg->GenerateProtoShower( cluster_v,
						cluster_hits_v_v,
						sps_v,
						vtx_v,
						proto_showers.at(i) );
      }// for all PFParticles
    }// pfparticle has same size as clusters associated
    
    // Do the seeds:
    // \TODO
    /*  _____ ___  ____   ___
	|_   _/ _ \|  _ \ / _			\
	| || | | | | | | | | |
	| || |_| | |_| | |_| |
	|_| \___/|____/ \___/
    */

    return;
  }
  
} // showerreco

#endif
