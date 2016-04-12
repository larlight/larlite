#ifndef PROTOSHOWERALGCLUSTERPARAMS_CXX
#define PROTOSHOWERALGCLUSTERPARAMS_CXX

#include "ProtoShowerAlgClusterParams.h"
#include "TVector3.h"

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/seed.h"
#include "DataFormat/vertex.h"

namespace showerreco {

  ProtoShowerAlgClusterParams::ProtoShowerAlgClusterParams() {

    _params_alg = 0;
    _params3D_alg = 0;
    _name = "ProtoShowerAlgClusterParams";
    
  }

  void ProtoShowerAlgClusterParams::SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg) {
    
    if (_params_alg) {
      delete _params_alg;
    }
    _params_alg = _new_params_alg;
    
    return;
  }
  
  void ProtoShowerAlgClusterParams::SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg) {
    
    if (_params3D_alg) {
      delete _params3D_alg;
    }
    _params3D_alg = _new_params3D_alg;
    
    return;
  }

  void ProtoShowerAlgClusterParams::GenerateProtoShower(::larlite::storage_manager* storage,
							::larlite::event_pfpart* ev_pfpart,
							const size_t proto_shower_pfpart,
							showerreco::ProtoShower & proto_shower)
  {
    
    
    // if no algo for calculating cluster params -> set up default ones
    if (!_params_alg) {
      _params_alg = new ::cluster::DefaultParamsAlg();
    }
    if (!_params3D_alg) {
      _params3D_alg = new ::cluster3D::Default3DParamsAlg();
    }
    
    // For each data product, fetch it and feed
    // it into the proto showers if possible
    //
    // That means using cluster_params and cluster3D_params if needed
    
    // Try to get the clusters if they are there
    ::larlite::event_cluster * ev_clust = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());

    if (ev_clust and (ev_clust->size() != 0) ){
      
      // Want to get the hits from the cluster too:
      ::larlite::event_hit * ev_hit = nullptr;
      auto const& ass_hit_v = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());
      
      if (ev_hit and (ev_hit->size() != 0) ){
	
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
	// if no clusters for this PFParticle -> no 2D information
	proto_shower.hasCluster2D(true);
	if (cluster_v.size() == 0)
	  proto_shower.hasCluster2D(false);
	
	// resize _params according to number of clusters
	proto_shower._params.resize( cluster_v.size() );
	
	// fill 2D information, if available
	for (size_t i=0; i < cluster_v.size(); i++){
	  _cru_helper.GenerateParams( cluster_hits_v_v[i], proto_shower._params.at( i ) );
	  _params_alg->FillParams( proto_shower._params.at( i ) );
	}// for all input clusters

      }// if there are associated hits
      else
	proto_shower.hasCluster2D(false);
    }// if there are associated clusters
    else
      proto_shower.hasCluster2D(false);

    
    //*********************************
    // 3D Cluster Params reconstruction
    //*********************************


    if (!_params3D_alg)
      _params3D_alg = new ::cluster3D::Default3DParamsAlg();
    
    // Get any possible spacepoints
    larlite::event_spacepoint * ev_sps = nullptr;
    auto const& ass_sps_v = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());
    
    if (ev_sps and (ev_sps->size() != 0) ){
      // fill vector fo spacepoints to be passed on to algorithm
      std::vector< ::larlite::spacepoint > sps_v;
      // if there are any spacepoints:
      if ( ev_sps && ev_sps -> size() != 0) {
	// get spacepoints associated to this PFParticle
	auto & sps_idx_v = ass_sps_v.at(proto_shower_pfpart);
	for (auto const& idx : sps_idx_v)
	  sps_v.push_back( ev_sps->at( idx ) );
      }// if spacepoints were found
      // if there are spacepoints, fill 3D params as well
      if (sps_v.size() == 0)
	proto_shower.hasCluster3D(false);
      else{
	proto_shower.hasCluster3D(true);
	_cru3D_helper.GenerateParams3D( sps_v, proto_shower._params3D);
      }
    }// if there are spacepoints
    else
      proto_shower.hasCluster3D(false);
    
    // Do the vertex:
    larlite::event_vertex * ev_vertex = nullptr;
    auto const& ass_vertex_v = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());

    if (ev_vertex and (ev_vertex->size() != 0) ){
	// fill vertices, if any associated
	std::vector< ::larlite::vertex > vtx_v;
	if ( ev_vertex && ev_vertex -> size() != 0) {
	  auto & vtx_idx_v = ass_vertex_v.at(proto_shower_pfpart);
	  for (auto idx : vtx_idx_v) {
	    double xyz[3];
	    vtx_v.push_back( ev_vertex->at(idx) );
	  }
	}// if there are vertices
	// fill vertex information
	if (vtx_v.size() == 0)
	  proto_shower.hasVertex(false);
	else{
	  for (auto const& vtx : vtx_v){
	    double xyz[3];
	    vtx.XYZ(xyz);
	    proto_shower._vertexes.push_back(TVector3(xyz[0],xyz[1],xyz[2]));
	  }
	  proto_shower.hasVertex(true);
	}
      }// if there is vertex info
      else
	proto_shower.hasVertex(false);

    std::cout << "has cluster2D : " << proto_shower.hasCluster2D() << std::endl;
    std::cout << "has cluster3D : " << proto_shower.hasCluster3D() << std::endl;
    std::cout << "has vertex    : " << proto_shower.hasVertex() << std::endl;
    
    return;
  }
  

}// namespace

#endif
