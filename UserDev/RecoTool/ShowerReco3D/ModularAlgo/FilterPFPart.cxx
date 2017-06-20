#ifndef FILTERPFPART_CXX
#define FILTERPFPART_CXX

#include "FilterPFPart.h"

namespace showerreco {

void FilterPFPart::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                    Shower_t& resultShower) {

  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()) {
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
    throw ShowerRecoException(ss.str());
  }
  
  auto & clusters = proto_shower.params();

  // keep track of largest cluster
  size_t largest_cluster_nhits = 0;
  
  // loop through clusters, find number of hits in each
  for (size_t n = 0; n < clusters.size(); n++) {
    
    size_t nhits = clusters.at(n).hit_vector.size();

    if (nhits < _min_nhits_absolute) {
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " : input cluster has too few hits";
      throw ShowerRecoException(ss.str());
    }
    
    if (nhits > largest_cluster_nhits)
    largest_cluster_nhits = nhits;
    
  }// for all clusters

  if (largest_cluster_nhits < _min_nhits_largest) {
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " : largest cluster too small";
    throw ShowerRecoException(ss.str());
    }
  
  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower
  
}


} //showerreco

#endif
