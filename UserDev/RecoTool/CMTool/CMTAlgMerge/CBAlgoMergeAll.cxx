#ifndef RECOTOOL_CBALGOMERGEALL_CXX
#define RECOTOOL_CBALGOMERGEALL_CXX

#include "CBAlgoMergeAll.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoMergeAll::CBAlgoMergeAll() : CBoolAlgoBase()
  //----------------------------------------
  {

  }

  //--------------------------------------------------------
  bool CBAlgoMergeAll::Bool(const ::cluster::cluster_params &cluster1,
                            const ::cluster::cluster_params &cluster2)
  //--------------------------------------------------------
  {
    if(cluster1.hit_vector.size() && cluster2.hit_vector.size()) return true;
    else return false;
  }
  
}

#endif
