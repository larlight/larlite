#ifndef BASICTOOL_INTERSECTALGO_CXX
#define BASICTOOL_INTERSECTALGO_CXX

#include "IntersectAlgo.h"

namespace geoalgo {

  Point_t IntersectAlgo::_Intersection_(const AABox_t& box, const HalfLine_t& line) const
  {

    // Prepare return point
    Point_t res(3);

    // If line start point is outside either 0 or 2 intersections
    if ( !box.Contain(line.Start()) )
      return res;

    // Calculate intersection point w.r.t. all boundaries

    double tmin = 0;
    double tmax = kINVALID_DOUBLE;

    // Get line start point & direction
    Point_t s  = line.Start();
    Vector_t d = line.Dir();

    // Get Box Min & Max boundaries
    Point_t Min = box.Min();
    Point_t Max = box.Max();
    
    // for all boundaries
    for (size_t n=0; n < box.Min().size(); n++){

      //if the direction of the line is // to box
      if ( d[n] == 0 ){
	// line is parallel -> no hit if point outside of box
	if ( (s[n] < Min[n]) or (s[n] > Max[n]) )
	  return res;
      }
      // compute t-value with near & far end of box
      double ood = 1. / d[n];
      double t1 = (Min[n]-s[n]) / ood;
      double t2 = (Max[n]-s[n]) / ood;
      // t1 should be closest plane (or plane in back)
      // swap t1 & t2 if t1 > t2
      _Swap_(t1, t2);
      // compute the intersection of of slab-interesection intervals
      if ( t1 > tmin ) tmin = t1;
      if ( t2 < tmax ) tmax = t2;
      // Exit with no intersection if slab intersection becomes empty
      if ( tmin > tmax ) return res;
    }//for all dimensions of box

    // tmin is the intersection as soon as we hit the first wall
    // find point of intersection
    res = s + d*tmin;
    
    return res;
  }

  // LineSegment sub-segment of HalfLine inside an AABox w/o checks
  LineSegment_t IntersectAlgo::_SubSegmentInside_(const AABox_t& box, const HalfLine_t& line) const
  {

    // First find interection point of half-line and box
    Point_t intersection = _Intersection_(box, line);

    // Build a new LineSegment
    LineSegment_t x;
    // if intersection point is valid over-write line segment
    if ( intersection.IsValid() )
      x = LineSegment(line.Start(), intersection);

    return x;
  }

  /// Get Trajectory inside box given some input trajectory -> now assumes trajectory cannot exit and re-enter box
  Trajectory_t IntersectAlgo::_TrajectoryInside_(const AABox_t& box, const Trajectory_t& trj) const
  {

    // if first & last points inside, then return full trajectory
    if ( box.Contain(trj[0]) and box.Contain(trj.back()) )
      return trj;

    return trj;

  }

  
  // swap tmin & tmax if tmin > tmax
  void IntersectAlgo::_Swap_(double& tmin, double& tmax) const
  {
    double ttmp;
    
    if (tmin > tmax){
      ttmp = tmax;
      tmax = tmin;
      tmin = ttmp;
    }

    return;
  }

}

#endif
