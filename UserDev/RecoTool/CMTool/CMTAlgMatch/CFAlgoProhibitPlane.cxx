#ifndef CFALGOPROHIBITPLANE_CXX
#define CFALGOPROHIBITPLANE_CXX

#include "CFAlgoProhibitPlane.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoProhibitPlane::CFAlgoProhibitPlane() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

    _removePlane = 0;

  }

  //-----------------------------
  void CFAlgoProhibitPlane::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoProhibitPlane::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

    for (auto *cl : clusters){
      if (cl->Plane() == _removePlane)
	return -1;
    }
    
    return 1;
  }

  //------------------------------
  void CFAlgoProhibitPlane::Report()
  //------------------------------
  {

  }
    
}
#endif
