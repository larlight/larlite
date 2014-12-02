#ifndef BASICTOOL_DISTANCEALGO_CXX
#define BASICTOOL_DISTANCEALGO_CXX

#include "DistanceAlgo.h"

namespace geoalgo {

  // Ref. RTCD Ch 5.1 p. 130
  double DistanceAlgo::_SqDist_(const Point_t& pt, const LineSegment_t& line) const
  {
    auto const& ab = line.Dir();
    auto const  ac = pt - line.Start();
    auto const  bc = pt - line.End();
    auto e = ac * ab;
    if( e <= 0. ) return ac.Length();
    auto f = ab.Length();
    if( e >= f ) return bc.Length();
    return (ac.Length() - e * e / f);
  }

  // Ref. RTCD Ch 5.1 p. 128-129
  Point_t DistanceAlgo::_ClosestPt_(const Point_t& pt, const LineSegment_t& line) const
  {
    auto const& ab = line.Dir();
    // Project pt on line (ab), but deferring divide by ab * ab
    auto t = (pt - line.Start()) * ab;
    // pt projects outside line, on the start side; clamp to start
    if( t <= 0. ) return line.Start();
    else {
      auto denom = ab.Length();
      // pt projects outside line, on the end side; clamp to end
      if( t>= denom ) return line.End();
      // pt projects inside the line. must deferred divide now
      else return (line.Start() + ab * (t/denom));
    }
  }
  
  // Ref. RTCD Ch 5.1 p. 130
  double DistanceAlgo::_SqDist_(const Point_t& pt, const HalfLine_t& line) const
  {
    auto const& ab = line.Dir();
    auto const  ac = pt - line.Start();
    auto const  bc = ac - ab;

    auto e = ac * ab;
    if( e <= 0. ) return (ac * ac);
    auto f = ab.Length();
    return (ac.Length() - e * e / f);
  }

  // Ref. RTCD Ch 5.1 p. 128-129
  Point_t DistanceAlgo::_ClosestPt_(const Point_t& pt, const HalfLine_t& line) const
  {
    auto const& ab = line.Dir();
    auto t = (pt - line.Start()) * ab;
    if( t <= 0. ) return line.Start();
    else {
      auto denom = ab.Length();
      return (line.Start() + ab * (t/denom));
    }
  }

  // Ref. RTCD Ch 5.1 p. 131-132 ... modified to consider distance to the box's wall
  double DistanceAlgo::_SqDist_(const Point_t& pt, const AABB_t& box) const
  {
    double dist = kINVALID_DOUBLE;

    // If a point is inside the box, simply compute the smallest perpendicular distance
    if(box.Contain(pt)) {

      auto const& pt_min = box.Min();
      auto const& pt_max = box.Max();
      // (1) Compute the distance to the YZ wall
      double dist_to_yz = pt[0] - pt_min[0];
      if( dist_to_yz > (pt_max[0] - pt[0]) ) dist_to_yz = pt_max[0] - pt[0];
      
      // (2) Compute the distance to the XZ wall
      double dist_to_zx = pt[1] - pt_min[1];
      if( dist_to_zx > (pt_max[1] - pt[1]) ) dist_to_zx = pt_max[1] - pt[1];
      
      // (3) Compute the distance to the XY wall
      double dist_to_xy = pt[2] - pt_min[2];
      if( dist_to_xy > (pt_max[2] - pt[2]) ) dist_to_xy = pt_max[2] - pt[2];
      
      // (4) Compute the minimum of (3), (4), and (5)
      dist = ( dist_to_yz < dist_to_zx ? dist_to_yz : dist_to_zx );
      dist = ( dist < dist_to_xy ? dist : dist_to_xy );
      dist *= dist;

    }else{
      // This refers to Ref. RTCD 5.1.3.1
      for(size_t i=0; i<pt.size(); ++i) {
	auto const& v_pt  = pt[i];
	auto const& v_max = box.Max()[i];
	auto const& v_min = box.Min()[i];

	if(v_pt < v_min) dist += (v_min - v_pt) * (v_min - v_pt);
	if(v_pt > v_max) dist += (v_pt - v_max) * (v_pt - v_max);
      }
    }
    return dist;
  }

  // Ref. RTCD Ch 5.1 p. 130-131 ... modified to consider a point on the surface 
  Point_t DistanceAlgo::_ClosestPt_(const Point_t& pt, const AABB_t& box) const
  {
    // Result
    auto res = pt;
    // For each coordinate axis, if the point coordinate value is outside box,
    // clamp it to the box, else keep it as is
    for(size_t i=0; i<pt.size(); ++i) {
      auto const& v_pt  = pt[i];
      auto const& v_min = box.Min()[i];
      auto const& v_max = box.Max()[i];
      res[i] = v_pt;
      if( v_pt < v_min ) res[i] = v_min;
      if( v_pt > v_max ) res[i] = v_max;
    }
    return res;
  }
  

}
#endif
