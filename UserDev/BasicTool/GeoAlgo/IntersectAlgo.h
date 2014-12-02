/**
 * \file IntersectAlgo.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class IntersectAlgo
 *
 * @author kazuhiro & David C
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_INTERSECTALGO_H
#define BASICTOOL_INTERSECTALGO_H

#include <iostream>
#include "GeoTrajectory.h"
#include "GeoHalfLine.h"
#include "GeoAABox.h"
#include "GeoVector.h"
#include "GeoLineSegment.h"

namespace geoalgo {
  /**
     \class IntersectAlgo
     User defined class IntersectAlgo ... these comments are used to generate
     doxygen documentation!
  */
  class IntersectAlgo{
    
  public:
    
    /// Default constructor
    IntersectAlgo(){};
    
    /// Default destructor
    virtual ~IntersectAlgo(){};

    /// Intersection between a HalfLine and an AABox
    Point_t Intersection(const HalfLine_t& line, const AABox_t& box) const
    { return _Intersection_(box, line); }
    /// Intersection between a HalfLine and an AABox
    Point_t Intersection(const AABox_t& box, const HalfLine_t& line) const
    { return _Intersection_(box, line); }

    /// LineSegment sub-segment of HalfLine inside an AABox
    LineSegment_t BoxOverlap(const AABox_t& box, const HalfLine_t& line) const
    { return _BoxOverlap_(box, line); }

    /// Get Trajectory inside box given some input trajectory -> now assumes trajectory cannot exit and re-enter box
    Trajectory_t BoxOverlap(const AABox_t& box, const Trajectory_t& trj) const
    { return _BoxOverlap_(box, trj); }
    /// Get Trajectory inside box given some input trajectory -> now assumes trajectory cannot exit and re-enter box
    Trajectory_t BoxOverlap(const Trajectory_t& trj, const AABox_t& box) const
    { return _BoxOverlap_(box, trj); }
    
  protected:
    
    /// Intersection between a LineSegment and an AABox w/o checks
    Point_t _Intersection_(const AABox_t& box, const HalfLine_t& line) const;

    /// LineSegment sub-segment of HalfLine inside an AABox w/o checks
    LineSegment_t _BoxOverlap_(const AABox_t& box, const HalfLine_t& line) const;

    /// Get Trajectory inside box given some input trajectory -> now assumes trajectory cannot exit and re-enter box
    Trajectory_t _BoxOverlap_(const AABox_t& box, const Trajectory_t& trj) const;

    /// Swap two points if min & max are inverted
    inline void _Swap_(double& tmin, double& tmax) const
    { if(tmin > tmax) std::swap(tmin,tmax); }

  };
}

#endif
/** @} */ // end of doxygen group 

