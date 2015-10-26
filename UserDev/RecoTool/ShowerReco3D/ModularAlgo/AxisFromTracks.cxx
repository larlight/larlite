#ifndef AXISFROMTRACKS_CXX
#define AXISFROMTRACKS_CXX

#include "AxisFromTracks.h"

namespace showerreco{

  void AxisFromTracks::do_reconstruction(const ShowerClusterSet_t & inputClusters, Shower_t & shower){
    
    // This function takes the shower cluster set and computes the best fit 3D axis
    // and then assigns it to the shower

    for (auto & cluster : inputClusters){
      std::cout << "This cluster had an original producer: " << cluster.original_producer
                << ", and was made from the original clusters:\n";
      for (auto & index : cluster.original_indexes){
        std::cout << "\t" << index << std::endl;
      }

    }


    std::cout << "Storage manager is " << _storage << std::endl;

    return;

  }


} //showerreco

#endif