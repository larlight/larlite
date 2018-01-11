#ifndef RECOTOOL_CBALGOMERGEALL_CXX
#define RECOTOOL_CBALGOMERGEALL_CXX

#include "CBAlgoMergeAll.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoMergeAll::CBAlgoMergeAll() : CBoolAlgoBase()
  //----------------------------------------
  {

  SetHits(0);

  }

  //--------------------------------------------------------
  bool CBAlgoMergeAll::Bool(const ::cluster::cluster_params &cluster1,
                            const ::cluster::cluster_params &cluster2)
  //--------------------------------------------------------
  {
    //if(cluster1.hit_vector.size() && cluster2.hit_vector.size()) return true;
    //Make merge "all" a bit more flexible. Set _hits to 0 for true merge all
    if (cluster1.N_Hits > _hits && cluster2.N_Hits > _hits) return true;
    else return false;
  }
  
}

#endif
