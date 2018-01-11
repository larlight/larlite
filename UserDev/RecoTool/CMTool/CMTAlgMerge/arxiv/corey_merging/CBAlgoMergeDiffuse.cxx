#ifndef RECOTOOL_CBALGOMERGEDIFFUSE_CXX
#define RECOTOOL_CBALGOMERGEDIFFUSE_CXX

#include "CBAlgoMergeDiffuse.h"

namespace cmtool {

  CBAlgoMergeDiffuse::CBAlgoMergeDiffuse() : CBoolAlgoBase()
  {
    _max_average_min_distance = 0.0;
  }


  
  bool CBAlgoMergeDiffuse::Bool(
      const ::cluster::ClusterParamsAlg &cluster1,
      const ::cluster::ClusterParamsAlg &cluster2)
  {
    if (cluster1.GetNHits() <= cluster2.GetNHits()){
      if (getBinnedDist(cluster1, cluster2) > _hit_fraction)
        // std::cout << "Merging!\n";
        return true;
    }
    else {
      if (getBinnedDist(cluster2, cluster1) > _hit_fraction)
        // std::cout << "Merging!\n";
        return true;      
    }

    return false;
  }

  float CBAlgoMergeDiffuse::getBinnedDist(
      const ::cluster::ClusterParamsAlg &cluster1,
      const ::cluster::ClusterParamsAlg &cluster2)
  {
    // This function finds the closest hit for each other hit, and bins
    // them into a quasi histogram to see what fraction are within the
    // required distance

    // It's very similar to inline merging, but it only returns if the cluster to merge is "diffuse"
    // and the cluster to merge to is not.

    const float wirePitch = 0.4;
    if (cluster1.GetParams().hit_density_2D > 1) return 0;
    if (cluster1.GetParams().hit_density_2D == 0) return 0;

    std::vector<float> n_hits_by_dist;
    // Units of this are wire spacing in argoneut: 4mm.
    // Want to be able to bin out to a few centimeters.  Use 10 spots (4cm)
    n_hits_by_dist.resize(10);
    for (auto & hit : cluster1.GetHitVector()){
      float minDist(9999.0);
      for (auto & otherHit : cluster2.GetHitVector()){
        float thisDist = sqrt(pow(hit.w - otherHit.w,2 ) + pow(hit.t - otherHit.t, 2));
        if ( thisDist < minDist){
          minDist = thisDist;
        }
      }
      // Now have the min dist for this hit.  Put it in the histogram
      int index = minDist/wirePitch;
      if (index > 9) 
        n_hits_by_dist.back() ++;
      else n_hits_by_dist.at(index) ++;
    }

    // Find out how many hits are within the _max_average_min_distance
    int n_good_hits = 0;
    for(int i = 0; i < n_hits_by_dist.size(); i++){
      if (wirePitch*(i+1) <= _max_average_min_distance) n_good_hits += n_hits_by_dist[i];
    }

    // if (n_good_hits > 1){
    //   std::cout << n_good_hits << " of " << cluster1.GetNHits() << " were good.";
    //   std::cout << "(Plane " << cluster1.Plane() << ", start pos: "
    //             << cluster1.GetParams().start_point.w << ", " << cluster1.GetParams().start_point.t << ")\n"
    //             << "cluster1 rms: (" << cluster1.GetParams().rms_x << ", " << cluster1.GetParams().rms_y << ")\n"
    //             << "cluster 1 density ... 1D: " << cluster1.GetParams().hit_density_1D 
    //             << ", 2D: " << cluster1.GetParams().hit_density_2D << "\n"
    //             ;

    //   std::cout << "Printing final vector: \n";
    //   for(int i = 0; i < n_hits_by_dist.size(); i++){
    //     std::cout << "\t[" << wirePitch*i << " - " << wirePitch*(i+1) << "]:\t"
    //               << n_hits_by_dist[i] << "\n";
    //   }
    // }

    return (float) n_good_hits/cluster1.GetNHits();

  }


}

#endif
