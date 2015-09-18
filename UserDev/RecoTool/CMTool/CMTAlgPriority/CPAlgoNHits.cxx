#ifndef RECOTOOL_CPALGONHITS_CXX
#define RECOTOOL_CPALGONHITS_CXX

#include "CPAlgoNHits.h"

namespace cmtool {

  //----------------------------------------------
  CPAlgoNHits::CPAlgoNHits() : CPriorityAlgoBase()
  //----------------------------------------------
  {
    _min_hits = 0;
  }

  //------------------------------------------------------------------------
  float CPAlgoNHits::Priority(const ::cluster::cluster_params &cluster)
  //------------------------------------------------------------------------
  {
    auto nhit = cluster.hit_vector.size();

    return ( nhit < _min_hits ? -1 : (float)nhit );
  }
  
}
#endif
