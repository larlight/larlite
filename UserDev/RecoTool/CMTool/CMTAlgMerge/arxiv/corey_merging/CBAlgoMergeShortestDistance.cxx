#ifndef RECOTOOL_CBALGOMERGESHORTESTDISTANCE_CXX
#define RECOTOOL_CBALGOMERGESHORTESTDISTANCE_CXX

#include "CBAlgoMergeShortestDistance.h"

namespace cmtool {

  CBAlgoMergeShortestDistance::CBAlgoMergeShortestDistance() : CBoolAlgoBase()
  {
    _max_distance = 0.4;
  }


  
  bool CBAlgoMergeShortestDistance::Bool(
      const ::cluster::ClusterParamsAlg &cluster1,
      const ::cluster::ClusterParamsAlg &cluster2)
  {


    // Shortest Distance is symmetric wrt cluster1/2
    if (getShortestDist(cluster1, cluster2) < _max_distance) return true;

    return false;
  }

  float CBAlgoMergeShortestDistance::getShortestDist(
      const ::cluster::ClusterParamsAlg &cluster1,
      const ::cluster::ClusterParamsAlg &cluster2)
  {


    float minDist(9999.0);
    for (auto & hit : cluster2. GetHitVector()){
      for (auto & otherHit : cluster1.GetHitVector()){
        float thisDist = sqrt(pow(hit.w - otherHit.w,2 ) + pow(hit.t - otherHit.t, 2));
        if ( thisDist < minDist){
          minDist = thisDist;
        }
      }
    }
    // std::cout << "(Plane " << cluster1.Plane() << ", start pos 1: "
    //           << cluster1.GetParams().start_point.w << ", " 
    //           << cluster1.GetParams().start_point.t << "), "
    //           << "start pos 2: "
    //           << cluster2.GetParams().start_point.w << ", " 
    //           << cluster2.GetParams().start_point.t << "), "
    //           << "dist " << minDist << "\n";

    return minDist;
  }


}

#endif
