#ifndef RECOTOOL_CPALGOPOLYAREA_CXX
#define RECOTOOL_CPALGOPOLYAREA_CXX

#include "CPAlgoPolyArea.h"

namespace cmtool {

  //----------------------------------------------------------
  CPAlgoPolyArea::CPAlgoPolyArea() : CPriorityAlgoBase()
  //----------------------------------------------------------
  {
    _area_cut = 0;
  }

  //------------------------------------------------------------------------------
  float CPAlgoPolyArea::Priority(const ::cluster::cluster_params &cluster)
  //------------------------------------------------------------------------------
  {

    auto area = cluster.PolyObject.Area();

    return ( area < _area_cut ? -1 : area);
  }
  
}
#endif
