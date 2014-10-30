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
  float CPAlgoNHits::Priority(const ::cluster::ClusterParamsAlg &cluster)
  //------------------------------------------------------------------------
  {
    auto nhit = cluster.GetNHits();

    return ( nhit < _min_hits ? -1 : (float)nhit );
  }
  
}
#endif
