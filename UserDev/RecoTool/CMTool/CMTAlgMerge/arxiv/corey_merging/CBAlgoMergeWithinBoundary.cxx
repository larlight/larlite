#ifndef RECOTOOL_CBALGOMERGEWITHINBOUNDARY_CXX
#define RECOTOOL_CBALGOMERGEWITHINBOUNDARY_CXX

#include "CBAlgoMergeWithinBoundary.h"

namespace cmtool {

  CBAlgoMergeWithinBoundary::CBAlgoMergeWithinBoundary() : CBoolAlgoBase()
  {

    // Set parameters for merging candadates

  	_max_length = 15;
  	_min_length = 30;
  	_max_hits = 30;
  	_min_hits = 50;
    _max_ep = 0.93;

  }

  bool CBAlgoMergeWithinBoundary::Bool(
    const ::cluster::ClusterParamsAlg &cluster1,
    const ::cluster::ClusterParamsAlg &cluster2)
  {

  	const ::cluster::ClusterParamsAlg * big;
    const ::cluster::ClusterParamsAlg * small;

  	if (isBig(cluster1)){
      if (isBig(cluster2)){
        // both are big, bail
        return false;
      }
      big = &cluster1;
    }
    else if (isBig(cluster2)){
      big =  &cluster2;
    }
    // else neither is big, bail
    else return false;

    // Figure out which is small
    if (isSmall(cluster1)){
      if (isSmall(cluster2)){
        // both are small, bail
        return false;
      }
      // verify that big and small aren't the same
      if (big == &cluster1) return false;
      small = &cluster1;
    }
    else if (isSmall(cluster2)){
      if (big == &cluster2) return false;
      small =  &cluster2;
    }
    // else neither are small, bail
    else return false;

    // Now make the final decision based on distances:
    // Must have the closest approach be less than the max
    // Must have the distance to start and end be greater than the min

    // float* bound1 = inBoundary(big);
    // float* bound2 = inBoundary(small);

    float low_x0 = 1000;
    float low_y0 = 1000;
    float high_x0 = 0;
    float high_y0 = 0;

    for (unsigned int i = 0; i < big->GetParams().PolyObject.Size(); ++i) {

      float w0 = big->GetParams().PolyObject.Point(i).first;
      float t0 = big->GetParams().PolyObject.Point(i).second;

      if ( w0 < low_x0 ) low_x0 = w0;
      if ( t0 < low_y0 ) low_y0 = t0;
      
      if ( w0 > high_x0 ) high_x0 = w0;
      if ( t0 > high_y0 ) high_y0 = t0;
    }

    float bound1[4] = {low_x0,low_y0,high_x0,high_x0};

    float low_x1 = 1000;
    float low_y1 = 1000;
    float high_x1 = 0;
    float high_y1 = 0;

    for (unsigned int i = 0; i < small->GetParams().PolyObject.Size(); ++i) {

      float w1 = small->GetParams().PolyObject.Point(i).first;
      float t1 = small->GetParams().PolyObject.Point(i).second;

      if ( w1 < low_x1 ) low_x1 = w1;
      if ( t1 < low_y1 ) low_y1 = t1;
      
      if ( w1 > high_x1 ) high_x1 = w1;
      if ( t1 > high_y1 ) high_y1 = t1;
    }

    float bound2[4] = {low_x1,low_y1,high_x1,high_x1};


    if (	bound2[0] > bound1[0] &&
    			bound2[1] > bound1[1] &&
    			bound2[2] < bound1[2]	&&
    			bound2[3] < bound1[3] &&
          closestApproach(*big, *small) < 10)
    {

      return true;
    }

    return false;

  }

  bool CBAlgoMergeWithinBoundary::isBig(const ::cluster::ClusterParamsAlg &cluster){

    size_t N_Hits = cluster.GetHitVector().size();
    double length = cluster.GetParams().length;

    if( N_Hits > _min_hits && length > _min_length){

        return true;
    }

  	return false;
  }

  bool CBAlgoMergeWithinBoundary::isSmall(const ::cluster::ClusterParamsAlg &cluster)
  {

    size_t N_Hits = cluster.GetHitVector().size();
    double length = cluster.GetParams().length;
    float ep = cluster.GetParams().eigenvalue_principal;

    if( N_Hits < 10 &&
        length < _max_length &&
        ep < _max_ep){

        return true;
    }

  	return false;
  }

  // float* CBAlgoMergeWithinBoundary::inBoundary(const ::cluster::ClusterParamsAlg &cluster){

  //   float low_x = 1000;
  //   float low_y = 1000;
  //   float high_x = 0;
  //   float high_y = 0;

  // 	for (unsigned int i = 0; i < cluster.GetParams().PolyObject.Size(); ++i) {

  // 		float w = cluster.GetParams().PolyObject.Point(i).first;
  //     float t = cluster.GetParams().PolyObject.Point(i).second;

  // 		if ( w < low_x ) low_x = w;
  // 		if ( t < low_y ) low_y = t;
  		
		// 	if ( w > high_x ) high_x = w;
  // 		if ( t > high_y ) high_y = t;

  // 	}

  // 	float arr[4] = {low_x, low_y, high_x, high_y};

  // 	return arr;
  // }

  float CBAlgoMergeWithinBoundary::closestApproach(
    const ::cluster::ClusterParamsAlg &cluster1,
    const ::cluster::ClusterParamsAlg &cluster2)
  {

      unsigned int npoints1 = cluster1.GetParams().PolyObject.Size();
      unsigned int npoints2 = cluster2.GetParams().PolyObject.Size();
      float bound = 10000;

      //loop over points on first polygon
      for(unsigned int i = 0; i < npoints1; ++i){

        float pt1w = cluster1.GetParams().PolyObject.Point(i).first;
        float pt1t = cluster1.GetParams().PolyObject.Point(i).second;
        
        //loop over points on second polygon
        for(unsigned int j = 0; j < npoints2; ++j){

          float pt2w = cluster2.GetParams().PolyObject.Point(j).first;
          float pt2t = cluster2.GetParams().PolyObject.Point(j).second;
          float dist = pow((pow(pt2w-pt1w,2)+pow(pt2t-pt1t,2)),0.5);

          if(dist<bound) bound = dist;
        }
      }

      return bound;
    } 


}

#endif
