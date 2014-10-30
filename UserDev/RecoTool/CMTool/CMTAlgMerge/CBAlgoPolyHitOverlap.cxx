#ifndef RECOTOOL_CBALGOPOLYHITOVERLAP_CXX
#define RECOTOOL_CBALGOPOLYHITOVERLAP_CXX

#include "CBAlgoPolyHitOverlap.h"

namespace cmtool {

  CBAlgoPolyHitOverlap::CBAlgoPolyHitOverlap() : CBoolAlgoBase()
  {
    // Nothing to be done in the base class
    this->reconfigure();
  }


  void CBAlgoPolyHitOverlap::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CBAlgoPolyHitOverlap::Bool(const ::cluster::ClusterParamsAlg &cluster1,
				  const ::cluster::ClusterParamsAlg &cluster2)
  {

    //Check and see if a certain fraction of hits of a cluster
    //lie within polygon boundary of other cluster
    
    if(cluster1.GetNHits() && cluster2.GetNHits()) return false;
    return false;
  }


}

#endif
