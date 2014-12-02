/**
 * \file DistanceAlgo.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class DistanceAlgo
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_DISTANCEALGO_H
#define BASICTOOL_DISTANCEALGO_H

#include "GeoHalfLine.h"
#include "GeoLineSegment.h"
#include "GeoAABB.h"

namespace geoalgo {

  class PointToLineDist;

  /**
     \class DistanceAlgo
     @brief Algorithm to compute the distance and closest-point-of-approach among geometrical objects.
     Most functions are taken from the reference Real-Time-Collision-Detection (RTCD):

     Ref: http://realtimecollisiondetection.net
  */
  class DistanceAlgo{

    friend class PointToLineDist;
    
  public:
    
    /// Default constructor
    DistanceAlgo(){}
    
    /// Default destructor
    virtual ~DistanceAlgo(){}
    
    //double SqDist(const LineSegment_t& line1, const LineSegment_t& line2) const;

    /// Point & LineSegment_t distance
    double SqDist(const Point_t& pt, const LineSegment_t& line) const
    { pt.compat(line.Start()); return _SqDist_(pt,line); }
    /// Point & LineSegment distance
    double SqDist(const LineSegment_t& line, const Point_t& pt) const
    { return SqDist(line,pt); }

    /// Point & LineSegment closest point
    Point_t ClosestPt(const Point_t& pt, const LineSegment_t& line) const
    { pt.compat(line.Start()); return _ClosestPt_(pt,line); }
    /// Point & LineSegment closest point
    Point_t ClosestPt(const LineSegment_t& line, const Point_t& pt) const
    { return ClosestPt(pt,line); }
    
    //double SqDist(const HalfLine_t& line1, const HalfLine_t& line2) const;

    /// Point & HalfLine distance
    double SqDist(const Point_t& pt, const HalfLine_t& line) const
    { pt.compat(line.Start()); return _SqDist_(pt,line); }
    /// Point & HalfLine distance
    double SqDist(const HalfLine_t& line, const Point_t& pt) const
    { return SqDist(pt,line); }

    /// Point & HalfLine closest point
    Point_t ClosestPt(const Point_t& pt, const HalfLine_t& line) const
    { pt.compat(line.Start()); return _ClosestPt_(pt,line); }
    /// Point & HalfLine closest point
    Point_t ClosestPt(const HalfLine_t& line, const Point_t& pt) const
    { return ClosestPt(pt,line); }

    //double SqDist(const HalfLine_t& hline, const LineSegment_t& sline) const;
    //double SqDist(const LineSegment_t& sline, const HalfLine_t& hline) const
    //{ return SqDist(hline,sline); }  

    /// Point & AABB distance
    double SqDist(const Point_t& pt, const AABB_t& box) const
    { pt.compat(box.Min()); return _SqDist_(pt,box); }
    /// Point & AABB distance
    double SqDist(const AABB_t& box, const Point_t& pt)
    { return SqDist(pt,box); }

    /// Point & AABB closest point
    Point_t ClosestPt(const Point_t& pt, const AABB_t& box) const
    { pt.compat(box.Min()); return _ClosestPt_(pt,box); }
    /// Point & AABB closest point
    Point_t ClosestPt(const AABB_t& box, const Point_t& pt) const
    { return ClosestPt(pt,box); }

  protected:

    /// Point & Point distance w/o dimensionality check
    double _SqDist_(const Point_t& pt1, const Point_t& pt2) const;

    /// Point & LineSegment distance w/o dimensionality check
    double _SqDist_(const Point_t& pt, const LineSegment_t& line) const;
    /// Point & LineSegment distance w/o dimensionality check
    double _SqDist_(const LineSegment_t& line, const Point_t&pt) const
    { return _SqDist_(pt,line); }

    // Point & LineSegment closest point w/o dimensionality check
    Point_t _ClosestPt_(const Point_t& pt, const LineSegment_t& line) const;
    // Point & LineSegment closest point w/o dimensionality check
    Point_t _ClosestPt_(const LineSegment_t& line, const Point_t& pt) const
    { return _ClosestPt_(pt,line); }

    /// Point & HalfLine distance w/o dimensionality check
    double _SqDist_(const Point_t& pt, const HalfLine_t& line) const;
    /// Point & HalfLine distance w/o dimensionality check
    double _SqDist_(const HalfLine_t& line, const Point_t& pt) const
    { return _SqDist_(pt,line); }

    // Point & HalfLine closest point w/o dimensionality check
    Point_t _ClosestPt_(const Point_t& pt, const HalfLine_t& line) const;
    // Point & HalfLine closest point w/o dimensionality check
    Point_t _ClosestPt_(const HalfLine_t& line, const Point_t& pt) const
    { return _ClosestPt_(pt,line); }

    /// Point & AABB distance w/o dimensionality check
    double _SqDist_(const Point_t& pt, const AABB_t& box) const;
    /// Point & AABB distance w/o dimensionality check
    double _SqDist_(const AABB_t& box, const Point_t& pt) const
    { return _SqDist_(pt,box); }

    /// Point & AABB closest point w/o dimensionality check
    Point_t _ClosestPt_(const Point_t& pt, const AABB_t& box) const;
    /// Point & AABB closest point w/o dimensionality check
    Point_t _ClosestPt_(const AABB_t& box, const Point_t& pt) const
    { return _ClosestPt_(pt,box); }

  };
}

#endif
/** @} */ // end of doxygen group 

