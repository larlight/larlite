#ifndef BASICTOOL_INTERSECTALGO_CXX
#define BASICTOOL_INTERSECTALGO_CXX

#include "IntersectAlgo.h"

namespace geoalgo {

  Point_t IntersectAlgo::Intersection(const AABox_t& box, const HalfLine_t& line, bool back) const
  {

    auto l = line;
    // if back == true then we need to invert the line's direction
    // this way we get distance to back of wall
    if (back)
      l.Dir(l.Dir() * -1);

    // If line start point is outside either 0 or 2 intersections
    if ( !box.Contain(l.Start()) )
      return Point_t(3);

    // Calculate intersection point w.r.t. all boundaries

    double tmin = 0;
    double tmax = kINVALID_DOUBLE;

    // Get line start point & direction
    auto const& s = l.Start();
    auto const& d = l.Dir();

    // Get Box Min & Max boundaries
    auto const& Min = box.Min();
    auto const& Max = box.Max();
    
    // for all boundaries
    for (size_t n=0; n < box.Min().size(); n++){

      //if the direction of the line is // to box
      if ( d[n] == 0 ){
	// line is parallel -> no hit if point outside of box
	if ( (s[n] < Min[n]) or (s[n] > Max[n]) )
	  return Point_t(3);
      }
      // compute t-value with near & far end of box
      double ood = 1. / d[n];
      double t1 = (Min[n]-s[n]) * ood;
      double t2 = (Max[n]-s[n]) * ood;
      // t1 should be closest plane (or plane in back)
      // swap t1 & t2 if t1 > t2
      _Swap_(t1, t2);
      // compute the intersection of of slab-interesection intervals
      if ( t1 > tmin ) tmin = t1;
      if ( t2 < tmax ) tmax = t2;
      // Exit with no intersection if slab intersection becomes empty
      if ( tmin > tmax ) return Point_t(3);
    }//for all dimensions of box

    // for inside point, tmax marks the intersection with the line going forward
    return s + d*tmax;
  }

  std::vector<Point_t> IntersectAlgo::Intersection(const AABox_t& box, 
						   const Trajectory_t& trj) const
  {

    std::vector<Point_t> result;
    if(trj.size() < 2) return result;

    /*
    auto const& min_pt = box.Min();
    auto const& max_pt = box.Max();

    bool inside=false;
    for(size_t i=0; i<trj.size(); ++i) {
      auto const& pt_b = trj[i];
      if(!i) inside = box.Contain(pt_b);
      else{
	if(inside != box.Contain(pt_b)) {
	  auto const& pt_a = trj[i-1];
	  // There's a boundary between last and this point
	  auto  ab = pt_b - pt_a;
	  double t = (

	  inside = box.Contain(pt_b);
	}
      }      
    }
    */
    return result;
  }

  // LineSegment sub-segment of HalfLine inside an AABox w/o checks
  LineSegment_t IntersectAlgo::BoxOverlap(const AABox_t& box, const HalfLine_t& line) const
  {

    // First find interection point of half-line and box
    Point_t intersection = Intersection(box, line);

    // Build a new LineSegment
    LineSegment_t x;
    // if intersection point is valid over-write line segment
    if ( intersection.IsValid() )
      x = LineSegment(line.Start(), intersection);

    return x;
  }

  /// Get Trajectory inside box given some input trajectory -> now assumes trajectory cannot exit and re-enter box
  Trajectory_t IntersectAlgo::BoxOverlap(const AABox_t& box, const Trajectory_t& trj) const
  {

    // if first & last points inside, then return full trajectory
    if ( box.Contain(trj[0]) and box.Contain(trj.back()) )
      return trj;

    return trj;

  }

}

#endif
