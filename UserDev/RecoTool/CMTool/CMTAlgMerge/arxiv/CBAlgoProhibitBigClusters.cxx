#ifndef RECOTOOL_CBALGOPROHIBITBIGCLUSTERS_CXX
#define RECOTOOL_CBALGOPROHIBITBIGCLUSTERS_CXX

#include "CBAlgoProhibitBigClusters.h"

namespace cmtool {

  //-------------------------------------------------------
  CBAlgoProhibitBigClusters::CBAlgoProhibitBigClusters() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    SetMinHits(9999);

  }

  //-----------------------------
  void CBAlgoProhibitBigClusters::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------
  bool CBAlgoProhibitBigClusters::Bool(const ::cluster::cluster_params &cluster1,
                                       const ::cluster::cluster_params &cluster2)
  //----------------------------------------------------------------
  {
    //if both clusters are big enough, prohibit merging
    if(cluster1.hit_vector.size() > _min_hits && cluster2.hit_vector.size() > _min_hits)
      return true;
    else
      return false;
  }

  //------------------------------
  void CBAlgoProhibitBigClusters::Report()
  //------------------------------
  {

  }
    
}
#endif
