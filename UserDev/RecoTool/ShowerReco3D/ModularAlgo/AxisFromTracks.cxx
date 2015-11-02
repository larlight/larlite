#ifndef AXISFROMTRACKS_CXX
#define AXISFROMTRACKS_CXX

#include "AxisFromTracks.h"
#include "DataFormat/track.h"
#include "DataFormat/cluster.h"

namespace showerreco {

void AxisFromTracks::do_reconstruction(const ShowerClusterSet_t & inputClusters, Shower_t & shower) {

  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower

  std::string _track_producer = "ct";

  // Get the tracks for this event:
  auto ev_track = _storage->get_data<larlite::event_track>(_track_producer);

  if (! ev_track){
    std::cout << "Bailing because no track pointer.\n";
    return;
  }
  if (!ev_track -> size()){
    std::cout << "Bailing because no tracks\n";
    return;
  }

  // Get the clusters associated to these tracks  
  
  _storage -> set_id(1, 0, ev_track->event_id());


  // Get the associated clusters to this track:
  ::larlite::event_cluster * ev_clus = nullptr;
  auto & clust_index_v = _storage->find_one_ass(ev_track->id(), ev_clus, _track_producer);
  if (!ev_clus) {
    std::cout << "Bailing because no cluster pointer.\n";
    return;
  }
  if (!ev_clus -> size()) {
    std::cout << "Bailing because no clusters\n";
    return;
  }


  std::cout << "clust_index_v has the following associations:\n";
  for (auto & set : clust_index_v){
    for (auto & index : set){
      std::cout << index << "\t";
    }
    std::cout << std::endl;
  }


  for (auto & cluster : inputClusters) {
    std::cout << "This cluster had an original producer: " << cluster.original_producer
              << ", and was made from the original clusters:\n";
    for (auto & index : cluster.original_indexes) {
      std::cout << "\t" << index << std::endl;
    }

  }


  std::cout << "Storage manager is " << _storage << std::endl;

  return;

}


} //showerreco

#endif