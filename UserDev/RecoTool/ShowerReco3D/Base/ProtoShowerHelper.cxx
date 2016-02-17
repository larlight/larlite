#ifndef PROTOSHOWERHELPER_CXX
#define PROTOSHOWERHELPER_CXX

#include "ProtoShowerHelper.h"

namespace showerreco {

// From the storage manager, and with the pfpart producer name, generate
// a vector of all the protoshowers in the event.
void ProtoShowerHelper::GenerateProtoShowers(::larlite::storage_manager* storage,
    const std::string &pfpart_producer_name,
    std::vector<ProtoShower> & proto_showers) {

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

  proto_showers.resize(ev_pfpart->size());

  // For each data product, fetch it and feed
  // it into the proto showers if possible
  //
  // That means using cluster_params and cluster3D_params if needed

  // Try to get the clusters if they are there
  ::larlite::event_cluster * ev_clust = nullptr;
  auto const& ass_cluster_v
    = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());

  // Now, check to see if there are clusters:
  if (ev_clust != 0  && ev_clust -> size() != 0) {

    // Want to get the hits from the cluster too:
    ::larlite::event_hit * ev_hit = nullptr;
    auto const& ass_hit_v
      = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());

    // Check that the cluster associations are the same length as the pfparticle list
    if (ass_cluster_v.size() == ev_pfpart -> size()) {
      // From here, we have the list of clusters for each particle.
      // We can build cluster_params and add it to the proto_showers
      //
      // We should have two to three sets of clusters per pfparticle


      for (size_t i = 0; i < ass_cluster_v.size(); i ++) {

        // If there are some, set the cluster2D bool to true
        // Else, set it to false
        if (ass_cluster_v.at(i).size() == 0)
          proto_showers.at(i)._hasCluster2D = false;
        else {
          proto_showers.at(i)._hasCluster2D = true;
        }
        // Make space for the params in this protoshower:
        proto_showers.at(i)._params.resize(ass_cluster_v.at(i).size());
        for (auto j_clust : ass_cluster_v.at(i)) {
          _cru_helper.GenerateParams(ass_hit_v.at(j_clust),
                                     ev_hit,
                                     proto_showers.at(i)._params.at(j_clust));
          // Now fill in the parameters:
          _params_alg.FillParams(proto_showers.at(i)._params.at(j_clust));
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

  // Now the 2D clusters are finished.  Move on to the 3D clusters (spacepoints)
  //
  larlite::event_spacepoint * ev_sps = nullptr;
  auto const& ass_sps_v
    = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());

  // if there are spacepoints, make the clusters for them.

  if ( ev_sps && ev_sps -> size() != 0) {

    std::cout << "Number of sps associations: " << ass_sps_v.size() << std::endl;

    // This is simpler than 2D clusters since there should be just one set
    // per pfparticle, and we don't have to get another data product.
    size_t index = 0;
    for (auto & sps_v : ass_sps_v) {
      if (sps_v.size() != 0) {
        std::cout << "Number of spacepoints: " << sps_v.size() << std::endl;
        /// Generate: from 1 set of sps => 1 Params3D using indexes (association)
        _cru3D_helper.GenerateParams3D(sps_v,
                                       ev_sps,
                                       proto_showers.at(index)._params3D);
        // std::cout << "proto_showers.at(index)._params3D.point_vector.size() "
        // << proto_showers.at(index)._params3D.point_vector.size()
        // << std::endl;
        // Now, fill the params:
        _params3D_alg.FillParams(proto_showers.at(index)._params3D);
        proto_showers.at(index)._hasCluster3D = true;
      }
      else {
        proto_showers.at(index)._hasCluster3D = false;
      }
      index ++;
    }
  }
  else {
    // if there are no spacepoints, set the bool false:
    for (auto & ps : proto_showers) {
      ps._hasCluster3D = false;
    }
  }

  // Do the vertex:

  larlite::event_vertex * ev_vertex = nullptr;
  auto const& ass_vertex_v
    = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());

  // if there are spacepoints, make the clusters for them.

  if ( ev_vertex && ev_vertex -> size() != 0) {

    std::cout << "Number of vertex associations: " << ass_vertex_v.size() << std::endl;

    // This is simpler than 2D clusters since there should be just one set
    // per pfparticle, and we don't have to get another data product.
    size_t index = 0;
    for (auto & vertex_v : ass_vertex_v) {
      if (vertex_v.size() != 0) {
        std::cout << "Number of vertexes: " << vertex_v.size() << std::endl;
        for (auto index : vertex_v){
          double xyz[3];
          ev_vertex -> at(index).XYZ(xyz);
          proto_showers.at(index)._vertexes.push_back(TVector3(xyz[0],xyz[1],xyz[2]));
        }
        proto_showers.at(index)._hasVertex = true;
      }
      else {
        proto_showers.at(index)._hasVertex = false;
      }
      index ++;
    }
  }
  else {
    // if there are no spacepoints, set the bool false:
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
