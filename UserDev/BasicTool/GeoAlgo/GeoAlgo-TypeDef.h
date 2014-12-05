//
// type-def file for ROOT
//

#ifndef BASICTOOL_GEOALGO_TYPEDEF_H
#define BASICTOOL_GEOALGO_TYPEDEF_H

// Header files included here
#include "GeoAlgoConstants.h"
#include "GeoAlgoException.h"
#include "GeoVector.h"
#include "GeoTrajectory.h"
#include "GeoHalfLine.h"
#include "GeoLine.h"
#include "GeoLineSegment.h"
#include "GeoAABox.h"
#include "IntersectAlgo.h"
#include "DistanceAlgo.h"

#include "DistToBoxWall.h"
#include "PointToLineDist.h"
#include "TwoLineIntersection.h"
#include "TrajectoryInVolume.h"
#include "SegmentPoCA.h"
#include "GeoAlgoMPLBackEnd.h"
//ADD_NEW_HEADER ... do not change this comment line

// Class forward declaration here
namespace geoalgo {
  class GeoAlgoException;
  class Vector;
  class Trajectory;
  class HalfLine;
  class Line;
  class LineSegment;
  class AABox;
  class DistanceAlgo;
  class IntersectAlgo;

  class DistToBoxWall;
  class PointToLineDist;
  class TwoLineIntersection;
  class TrajectoryInVolume;
  class SegmentPoca;
}
namespace mpl {
  class GeoAlgoMPLBackEnd;
}
//ADD_EMPTY_CLASS ... do not change this comment line
#endif
















