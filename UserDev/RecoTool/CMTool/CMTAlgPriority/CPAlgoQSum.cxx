#ifndef RECOTOOL_CPALGOQSUM_CXX
#define RECOTOOL_CPALGOQSUM_CXX

#include "CPAlgoQSum.h"

namespace cmtool {

  //----------------------------------------------------------
  CPAlgoQSum::CPAlgoQSum() : CPriorityAlgoBase()
  //----------------------------------------------------------
  {
    _qsum_cut = 0;
  }

  //------------------------------------------------------------------------------
  float CPAlgoQSum::Priority(const ::cluster::cluster_params &cluster)
  //------------------------------------------------------------------------------
  {
    if(cluster.sum_charge < _qsum_cut) return -1;

    return cluster.sum_charge;
  }
  
    
}
#endif
