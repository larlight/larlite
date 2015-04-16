#ifndef RECOTOOL_CBALGOPOLYOVERLAP_CXX
#define RECOTOOL_CBALGOPOLYOVERLAP_CXX

#include "CBAlgoPolyOverlap.h"

namespace cmtool {

  CBAlgoPolyOverlap::CBAlgoPolyOverlap() : CBoolAlgoBase()
  {
    // Nothing to be done in the base class
    SetDebug(false);
    SetMinNumHits(0);
    SetOverlapFraction(0.0);
    this->reconfigure();
  }


  void CBAlgoPolyOverlap::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  void CBAlgoPolyOverlap::SetOverlapFraction(float frac){
    // check that the fraction is not less than 0 or greater than 1:
    if (frac > 1.0 || frac < 0.0){
      _overlap_fraction = 0.0;
    }
    else 
      _overlap_fraction = frac;
    return;
  }

  
  bool CBAlgoPolyOverlap::Bool(const ::cluster::ClusterParamsAlg &cluster1,
                               const ::cluster::ClusterParamsAlg &cluster2)
  {

    if( (cluster1.GetParams().N_Hits < _min_hits) ||
        (cluster2.GetParams().N_Hits < _min_hits) )
      return false;

    //if either has < 3 sides do not merge!
    if ( (cluster1.GetParams().PolyObject.Size() < 2) or
         (cluster2.GetParams().PolyObject.Size() < 2) ){
      return false;
    }
    if (_debug and cluster1.GetParams().N_Hits > 10 and cluster2.GetParams().N_Hits > 10) {
      std::cout << "Cluster 1:" << std::endl;
      std::cout << "\tN_Hits: " << cluster1.GetParams().N_Hits << std::endl;
      std::cout << "\tN Sides:" << cluster1.GetParams().PolyObject.Size() << std::endl;
      for (unsigned int n=0; n < cluster1.GetParams().PolyObject.Size(); n++)
        std::cout << "\t\t\t(" << cluster1.GetParams().PolyObject.Point(n).first << ", "
                  << cluster1.GetParams().PolyObject.Point(n).second << ")" << std::endl;
      std::cout << "Cluster 2:" << std::endl;
      std::cout << "\tN_Hits: " << cluster2.GetParams().N_Hits << std::endl;
      std::cout << "\tN Sides:" << cluster2.GetParams().PolyObject.Size() << std::endl;
      for (unsigned int n=0; n < cluster2.GetParams().PolyObject.Size(); n++)
        std::cout << "\t\t\t(" << cluster2.GetParams().PolyObject.Point(n).first << ", "
                  << cluster2.GetParams().PolyObject.Point(n).second << ")" << std::endl;
    }

    // Two options.  If the overlap fraction is set to 0.0, then any overlap at all 
    // will return true
    // Get the polygon that is the overlap between the two:
    Polygon2D overlap(cluster1.GetParams().PolyObject, 
                      cluster2.GetParams().PolyObject);

    // If there is no overlap, return false:
    if (overlap.Size() == 0) return false;

    // if the _overlap_fraction parameter is 0, any overlap is sufficient:
    if (_overlap_fraction == 0 && overlap.Size() > 0 ) {
      if (_verbose) { std::cout << "Overlap...merging!" << std::endl; }
      return true;
    }
    else{  // the overlap fraction is bigger than zero.  
      float overlap_area = overlap.Area();
      float cluster_area = 0.0;
      // Find the smaller cluster for comparison:
      if (cluster1.GetParams().N_Hits > cluster2.GetParams().N_Hits){
        cluster_area = cluster1.GetParams().PolyObject.Area();
      }
      else{
        cluster_area = cluster2.GetParams().PolyObject.Area();
      }

      if ( (overlap_area / cluster_area ) > _overlap_fraction ) return true;
    }


    // //if the two polygons overlap even partially
    // //then return true! --> MERGE!
    // if ( cluster1.GetParams().PolyObject.PolyOverlapSegments(cluster2.GetParams().PolyObject) ){
    //   return true;
    // }
    return false;

  }


}

#endif
