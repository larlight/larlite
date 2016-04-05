#ifndef PROTOSHOWERHELPER_CXX
#define PROTOSHOWERHELPER_CXX

#include "ProtoShowerHelper.h"

namespace showerreco {

  ProtoShowerHelper::ProtoShowerHelper() : hack() {

    _params_alg = 0;
    _params3D_alg = 0;

  }

  ProtoShowerHelper::~ProtoShowerHelper() {


    if (_params_alg) {
      delete _params_alg;
    }

    if (_params3D_alg) {
      delete _params3D_alg;
    }

  }

  void ProtoShowerHelper::SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg) {

    if (_params_alg) {
      delete _params_alg;
    }
    _params_alg = _new_params_alg;


  }

  void ProtoShowerHelper::SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg) {

    if (_params3D_alg) {
      delete _params3D_alg;
    }
    _params3D_alg = _new_params3D_alg;


  }


  // From the storage manager, and with the pfpart producer name, generate
  // a vector of all the protoshowers in the event.
  void ProtoShowerHelper::GenerateProtoShowers(::larlite::storage_manager* storage,
					       const std::string &pfpart_producer_name,
					       std::vector<ProtoShower> & proto_showers,
					       std::vector<unsigned int> showerLikePFParts) {


    if (!_params_alg) {
      _params_alg = new ::cluster::DefaultParamsAlg();
    }
    if (!_params3D_alg) {
      _params3D_alg = new ::cluster3D::Default3DParamsAlg();
    }

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
    auto const& ass_cluster_v
      = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());

    // std::cout << "doing clusters" << std::endl;

    // Now, check to see if there are clusters:
    if (ev_clust != 0  && ev_clust -> size() != 0) {

      // Want to get the hits from the cluster too:
      ::larlite::event_hit * ev_hit = nullptr;
      auto const& ass_hit_v
	= storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());

      // std::cout << "Found " << ev_clust -> size() << " clusters" << std::endl;

      // Check that the cluster associations are the same length as the pfparticle list
      if (ass_cluster_v.size() == ev_pfpart -> size()) {
	// From here, we have the list of clusters for each particle.
	// We can build cluster_params and add it to the proto_showers
	//
	// We should have two to three sets of clusters per pfparticle

	// use the indexes stored in showerLikePFParts to loop through the clusters
	for (size_t i = 0; i < showerLikePFParts.size(); i ++ ) {
	  size_t proto_shower_pfpart = showerLikePFParts.at(i);

	  // std::cout << "On particle " << i << "("
	  //           << "pfparticle " << proto_shower_pfpart
	  //           << "), this particle has "
	  //           << ass_cluster_v.at(proto_shower_pfpart).size()
	  //           << " of " << ev_clust->size()
	  //           << " clusters."
	  //           << std::endl;

	  // If there are some, set the cluster2D bool to true
	  // Else, set it to false
	  if (ass_cluster_v.at(proto_shower_pfpart).size() == 0) {
	    proto_showers.at(i)._hasCluster2D = false;
	    // Don't keep going if there are no clusters.
	    // std::cout << ""
	    continue;
	  }
	  else {
	    proto_showers.at(i)._hasCluster2D = true;
	  }
	  // Make space for the params in this protoshower:
	  proto_showers.at(i)._params.resize(ass_cluster_v.at(proto_shower_pfpart).size());
	  size_t internal_cluster_index = 0;

	  for (auto j_clust : ass_cluster_v.at(proto_shower_pfpart)) {
	    // std::cout << "On cluster " << j_clust << ","
	    //           << " this cluster has " << ass_hit_v.at(j_clust).size()
	    //           << " hits." << std::endl;

	    // std::cout << "proto_showers.at(i)._params.size() is "
	    //           << proto_showers.at(i)._params.size()
	    //           << ", index is " << internal_cluster_index
	    //           << std::endl;

	    _cru_helper.GenerateParams(ass_hit_v.at(j_clust),
				       ev_hit,
				       proto_showers.at(i)._params.at(internal_cluster_index));

	    // std::cout << "params are generated, hit_vector.size() is "
	    // << proto_showers.at(i)._params.at(internal_cluster_index).hit_vector.size()
	    // << std::endl;
	    // Now fill in the parameters:
	    _params_alg->FillParams(proto_showers.at(i)._params.at(internal_cluster_index));


	    auto const& cluster = ev_clust->at(j_clust);
	    auto      & this_one= proto_showers.at(i)._params.at(internal_cluster_index);
	    
	    // std::cout << "\t>>> Override cluster params start+end point <<<\n";
	    // std::cout << "(w,t) : (" << this_one.start_point.w << "," << this_one.end_point.t << ") --> ";

	    // this_one.start_point = hack.convertCSP(cluster);
	    // std::cout << "(w',t') : (" << this_one.start_point.w << "," << this_one.end_point.t << ")\n";

	    // this_one.end_point = hack.convertCEP(cluster);

	    

	  
	    // std::cout << "params filling finished." << std::endl;
	    internal_cluster_index++;
	    // std::cout << "Finished making params for cluster " << j_clust
	    // << "which had " << ass_hit_v.at(j_clust).size() << " hits."
	    // << std::endl;
	  }
	}
      }
    }
    else {
      // if there are no clusters, set the bool false:
      for (auto & ps : proto_showers) {
	ps._hasCluster2D = false;
      }
    }

    // std::cout << "doing spacepoints" << std::endl;

    // Now the 2D clusters are finished.  Move on to the 3D clusters (spacepoints)
    //
    larlite::event_spacepoint * ev_sps = nullptr;
    auto const& ass_sps_v
      = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());

    // if there are spacepoints, make the clusters for them.

    if ( ev_sps && ev_sps -> size() != 0) {


      // This is simpler than 2D clusters since there should be just one set
      // per pfparticle, and we don't have to get another data product.

      for (size_t i = 0; i < showerLikePFParts.size(); i ++ ) {
	size_t proto_shower_pfpart = showerLikePFParts.at(i);

	auto & sps_v = ass_sps_v.at(proto_shower_pfpart);
	if (sps_v.size() != 0) {
	  /// Generate: from 1 set of sps => 1 Params3D using indexes (association)
	  _cru3D_helper.GenerateParams3D(sps_v,
					 ev_sps,
					 proto_showers.at(i)._params3D);
	  // std::cout << "proto_showers.at(i)._params3D.point_vector.size() "
	  // << proto_showers.at(i)._params3D.point_vector.size()
	  // << std::endl;
	  // Now, fill the params:
	  _params3D_alg->FillParams(proto_showers.at(i)._params3D);
	  proto_showers.at(i)._hasCluster3D = true;
	}
	else {
	  proto_showers.at(i)._hasCluster3D = false;
	}
      }
    }
    else {
      // if there are no spacepoints, set the bool false:
      for (auto & ps : proto_showers) {
	ps._hasCluster3D = false;
      }
    }


    // std::cout << "doing vertexes" << std::endl;


    // Do the vertex:

    larlite::event_vertex * ev_vertex = nullptr;
    auto const& ass_vertex_v
      = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());

    // if there are spacepoints, make the clusters for them.

    if ( ev_vertex && ev_vertex -> size() != 0) {


      // This is simpler than 2D clusters since there should be just one set
      // per pfparticle, and we don't have to get another data product.
      for (size_t i = 0; i < showerLikePFParts.size(); i ++ ) {
	size_t proto_shower_pfpart = showerLikePFParts.at(i);

	auto & vertex_v = ass_vertex_v.at(proto_shower_pfpart);
	if (vertex_v.size() != 0) {
	  for (auto index : vertex_v) {
	    double xyz[3];
	    ev_vertex -> at(index).XYZ(xyz);
	    proto_showers.at(i)._vertexes.push_back(TVector3(xyz[0], xyz[1], xyz[2]));
	  }
	  proto_showers.at(i)._hasVertex = true;
	}
	else {
	  proto_showers.at(i)._hasVertex = false;
	}
      }
    }
    else {
      // if there are no vertexes, set the bool false:
      for (auto & ps : proto_showers) {
	ps._hasVertex = false;
      }
    }

    // Do the seeds:
    // \TODO
    /*  _____ ___  ____   ___
	|_   _/ _ \|  _ \ / _ \
	| || | | | | | | | | |
	| || |_| | |_| | |_| |
	|_| \___/|____/ \___/
    */

  }

} // showerreco

#endif
