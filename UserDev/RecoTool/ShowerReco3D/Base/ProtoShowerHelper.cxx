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

}

} // showerreco

#endif
