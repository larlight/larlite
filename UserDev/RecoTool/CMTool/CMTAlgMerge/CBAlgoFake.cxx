#ifndef RECOTOOL_CBALGOFAKE_CXX
#define RECOTOOL_CBALGOFAKE_CXX

#include "CBAlgoFake.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoFake::CBAlgoFake() : CBoolAlgoBase()
  //----------------------------------------
  {
    _flip = false;
    _ctr  = 0;
    // Nothing to be done in the base class
  }

  //--------------------------------------------------------
  bool CBAlgoFake::Bool(const ::cluster::cluster_params &cluster1,
                        const ::cluster::cluster_params &cluster2)
  //--------------------------------------------------------
  {
    if(cluster1.hit_vector.size() && cluster2.hit_vector.size()) {
      _ctr++;
      if( (_ctr%64) == 0)
        _flip = (!_flip);
      return _flip;
    }
    else return false;
  }

}

#endif
