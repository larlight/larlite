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
  bool CBAlgoProhibitBigClusters::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			       const ::cluster::ClusterParamsAlg &cluster2)
  //----------------------------------------------------------------
  {
    //if both clusters are big enough, prohibit merging
    if(cluster1.GetNHits() > _min_hits && cluster2.GetNHits() > _min_hits)
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
