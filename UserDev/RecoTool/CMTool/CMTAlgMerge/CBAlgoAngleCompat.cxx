#ifndef RECOTOOL_CBALGOANGLECOMPAT_CXX
#define RECOTOOL_CBALGOANGLECOMPAT_CXX

#include "CBAlgoAngleCompat.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoAngleCompat::CBAlgoAngleCompat() : CBoolAlgoBase()
  //----------------------------------------
  {
    _flip = false;
    _ctr  = 0;
    _min_size = 0.;
    _max_angle_diff = 0.;
    // Nothing to be done in the base class
  }

  //--------------------------------------------------------
  bool CBAlgoAngleCompat::Bool(const ::cluster::Cluster &cluster1,
			       const ::cluster::Cluster &cluster2)
  //--------------------------------------------------------
  {

    // match clusters based on angle compatibility

    // skip if larger cluster (cluster 1) has less then minimum amount of hits
    if (cluster1.size() < _min_size) return false;

    if ( fabs(cluster2._angle - cluster1._angle) < _max_angle_diff ) return true;

    return false;
  }

}

#endif
