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
#include "GeoTrajectory.h"
#include "GeoAABox.h"

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
    

    //******************************************
    //CLOSEST APPROACH BETWEEN TWO LINE SEGMENTS
    //******************************************
    /// LineSegment_t & LineSegment_t distance - keep track of points
    double SqDist(const LineSegment_t& seg1, const LineSegment_t& seg2, Point_t& c1, Point_t& c2) const
    { seg1.Start().compat(seg2.Start()); return _SqDist_(seg1, seg2, c1, c2); }
    /// LineSegment & LineSegment, don't keep track of points
    double SqDist(const LineSegment_t& seg1, const LineSegment_t& seg2) const
    { Point_t c1; Point_t c2; return SqDist(seg1, seg2, c1, c2); }


    //******************************************
    //CLOSEST APPROACH BETWEEN SEGMENT AND TRACK
    //******************************************
    /// LineSegment & Trajectory, keep track of points
    double SqDist(const LineSegment_t& seg, const Trajectory_t& trj, Point_t& c1, Point_t& c2) const;
    /// LineSegment & Trajectory, keep track of points
    double SqDist(const Trajectory_t& trj, const LineSegment_t& seg, Point_t& c1, Point_t& c2) const
    { return SqDist(seg, trj, c1, c2); }
    /// LineSegment & Trajectory, don't keep track of points
    double SqDist(const Trajectory_t& trj, const LineSegment_t& seg) const
    { Point_t c1; Point_t c2; return SqDist(seg, trj, c1, c2); }
    /// LineSegment & Trajectory, don't keep track of points
    double SqDist(const LineSegment_t& seg, const Trajectory_t& trj) const
    { Point_t c1; Point_t c2; return SqDist(seg, trj, c1, c2); }
    

    //*****************************************************
    //CLOSEST APPROACH BETWEEN SEGMENT AND VECTOR OF TRACKS
    //*****************************************************
    /// LineSegment & vector of Trajectories, keep track of points
    double SqDist(const LineSegment_t& seg, const std::vector<Trajectory_t> &trj, Point_t& c1, Point_t& c2, int& trackIdx) const;
    /// LineSegment & vector of Trajectories, keep track of points
    double SqDist(const std::vector<Trajectory_t> &trj, const LineSegment_t& seg, Point_t& c1, Point_t& c2, int& trackIdx) const
    { return SqDist(seg, trj, c1, c2, trackIdx); }
    /// LineSegment & vector of Trajectories, don't keep track of points
    double SqDist(const std::vector<Trajectory_t> &trj, const LineSegment_t& seg) const
    { Point_t c1; Point_t c2; int trackIdx; return SqDist(seg, trj, c1, c2, trackIdx); }
    /// LineSegment & vector of Trajectories, don't keep track of points
    double SqDist(const LineSegment_t& seg, const std::vector<Trajectory_t> &trj) const
    { Point_t c1; Point_t c2; int trackIdx; return SqDist(seg, trj, c1, c2, trackIdx); }


    //****************************************
    //CLOSEST APPROACH BETWEEN POINT AND TRACK
    //****************************************
    /// Point_t & Trajectory_t distance
    double SqDist(const Point_t& pt, const Trajectory_t& trj) const;
    /// Point_t & Trajectory_t distance
    double SqDist(const Trajectory_t& trj, const Point_t& pt) const
    { return SqDist(pt,trj); }

    /// Point_t & Trajectory_t closest point
    Point_t ClosestPt(const Point_t& pt, const Trajectory_t& trj) const;
    /// Point_t & Trajectory_t closest point
    Point_t ClosestPt(const Trajectory_t& trj, const Point_t& pt) const
    { return ClosestPt(pt,trj); }

    /// Point & LineSegment_t distance
    double SqDist(const Point_t& pt, const LineSegment_t& line) const
    { pt.compat(line.Start()); return _SqDist_(pt,line); }
    /// Point & LineSegment distance
    double SqDist(const LineSegment_t& line, const Point_t& pt) const
    { return SqDist(pt,line); }

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

    /// Point & AABox distance
    double SqDist(const Point_t& pt, const AABox_t& box) const
    { pt.compat(box.Min()); return _SqDist_(pt,box); }
    /// Point & AABox distance
    double SqDist(const AABox_t& box, const Point_t& pt)
    { return SqDist(pt,box); }

    /// Point & AABox closest point
    Point_t ClosestPt(const Point_t& pt, const AABox_t& box) const
    { pt.compat(box.Min()); return _ClosestPt_(pt,box); }
    /// Point & AABox closest point
    Point_t ClosestPt(const AABox_t& box, const Point_t& pt) const
    { return ClosestPt(pt,box); }

  protected:

    /// Point & Point distance w/o dimensionality check
    double _SqDist_(const Point_t& pt1, const Point_t& pt2) const;

    /// Point & LineSegment distance w/o dimensionality check
    double _SqDist_(const Point_t& pt, const LineSegment_t& line) const;
    /// Point & LineSegment distance w/o dimensionality check
    double _SqDist_(const LineSegment_t& line, const Point_t&pt) const
    { return _SqDist_(pt,line); }

    /// LineSegment & LineSegment distance w/o dimensionality check
    double _SqDist_(const LineSegment_t& seg1, const LineSegment_t& seg2, Point_t& c1, Point_t& c2) const;

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

    /// Point & AABox distance w/o dimensionality check
    double _SqDist_(const Point_t& pt, const AABox_t& box) const;
    /// Point & AABox distance w/o dimensionality check
    double _SqDist_(const AABox_t& box, const Point_t& pt) const
    { return _SqDist_(pt,box); }

    /// Point & AABox closest point w/o dimensionality check
    Point_t _ClosestPt_(const Point_t& pt, const AABox_t& box) const;
    /// Point & AABox closest point w/o dimensionality check
    Point_t _ClosestPt_(const AABox_t& box, const Point_t& pt) const
    { return _ClosestPt_(pt,box); }

    /// Clamp function: checks if value out of bounds
    double _Clamp_(const double n, const double min, const double max) const;

  };
}

#endif
/** @} */ // end of doxygen group 

