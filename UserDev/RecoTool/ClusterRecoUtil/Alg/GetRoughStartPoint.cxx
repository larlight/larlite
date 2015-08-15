#ifndef GETROUGHSTARTPOINT_CXX
#define GETROUGHSTARTPOINT_CXX

#include "GetRoughStartPoint.h"
#include "CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>

namespace cluster {

  void GetRoughStartPoint::do_params_fill(cluster_params & cluster, bool verbose){

    // Geometry Utilities
    auto geomHelper = ::larutil::GeometryHelper::GetME();

    // The idea:
    // take the intersection between the polygon boundary and the 
    // 2D slope anchored to the mean point in the cluster
    // these two intersections may be good candidated for either
    // start or end points
    
    return;
  }
  
} // cluster

#endif
