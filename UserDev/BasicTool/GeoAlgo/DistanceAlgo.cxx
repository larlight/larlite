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
  double DistanceAlgo::_SqDist_(const Point_t& pt, const AABox_t& box) const
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
  Point_t DistanceAlgo::_ClosestPt_(const Point_t& pt, const AABox_t& box) const
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
  


  // Distance between a Trajectory_t and a Point_t
  // Loop over segments that make up the trajectory and keep track
  // of shortest distance between any of them and the point
  // The smallest such distance is the return
  double DistanceAlgo::SqDist(const Point_t& pt, const Trajectory_t& trj) const
  {

    // Make sure trajectory object is properly defined
    if (!trj.size())
      throw GeoAlgoException("Trajectory object not properly set...");
    
    // Check dimensionality compatibility between point and trajectory
    trj.compat(pt);

    // Now keep track of smallest distance and loop over traj segments
    double distMin = kINVALID_DOUBLE;
    for (size_t l=0; l < trj.size()-1; l++){
      LineSegment_t seg( trj[l], trj[l+1] );
      double distTmp = _SqDist_(pt,seg);
      if (distTmp < distMin) { distMin = distTmp; }
    }

    return distMin;
  }


  // Closest point between a Trajectory_t and a Point_t
  // Loop over segments that make up the trajectory and keep track
  // of shortest distance between any of them and the point
  // For the shortest distance find the point at which it is found
  Point_t DistanceAlgo::ClosestPt(const Point_t& pt, const Trajectory_t& trj) const
  {

    // Make sure trajectory object is properly defined
    if (!trj.size())
      throw GeoAlgoException("Trajectory object not properly set...");
    
    // Check dimensionality compatibility between point and trajectory
    trj.compat(pt);

    // Now keep track of smallest distance and loop over traj segments
    double distMin = std::numeric_limits<double>::max();
    // For that smallest distance, keep track of the segment for which it was found
    int pos = 0;
    for (size_t l=0; l < trj.size()-1; l++){
      LineSegment_t seg( trj[l], trj[l+1] );
      double distTmp = _SqDist_(pt,seg);
      if (distTmp < distMin) { distMin = distTmp; pos = l; }
    }

    // Now that we have the segment for the closest approach
    // Use it to find the closest point on that segment
    LineSegment_t segMin(trj[pos], trj[pos+1]);
    return _ClosestPt_(pt,segMin);
  }
  
  // Closest Approach between a segment and a Trajectory
  // loop over segments in trajectory and find the one that
  // is closest. Then find distance
  double DistanceAlgo::SqDist(const LineSegment_t& seg, const Trajectory_t& trj, Point_t& c1, Point_t& c2) const
  {

    // Make sure trajectory object is properly defined
    if (!trj.size())
      throw GeoAlgoException("Trajectory object not properly set...");
    
    // Check dimensionality compatibility between point and trajectory
    trj.compat(seg.Start());

    // keep track of c1 & c2
    Point_t c1min;
    Point_t c2min;
    // Now keep track of smallest distance and loop over traj segments
    double distMin = std::numeric_limits<double>::max();

    for (size_t l=0; l < trj.size()-1; l++){
      LineSegment_t segTmp(trj[l], trj[l+1]);
      double distTmp = _SqDist_(segTmp, seg, c1min, c2min);
      if ( distTmp < distMin ){
	c1 = c1min;
	c2 = c2min;
	distMin = distTmp;
      }
    }//for all segments in the track

    return distMin;
  }


  // Closest Approach between a Segment and a list of tracks
  // keep track of points of closest approach both on nearest
  // track as well as on segment
  // keep track of which track has the point of closest approcah
  double DistanceAlgo::SqDist(const LineSegment& seg, const std::vector<Trajectory_t> &trj, Point_t& c1, Point_t& c2, int& trackIdx) const
  {

    // holders to keep track of track with shortest distance
    double minDist = kINVALID_DOUBLE;
    // holders for points of closest approach
    Point_t c1min;
    Point_t c2min;

    for (size_t t=0; t < trj.size(); t++){

      //need to loop over all tracks and find the one which is closest
      // Make sure trajectory object is properly defined
      if (!trj[t].size())
	throw GeoAlgoException("Trajectory object not properly set...");
    
      // Check dimensionality compatibility between point and trajectory
      trj[t].compat(seg.Start());

      // now calculate closest approach
      double tmpDist = SqDist(seg, trj[t], c1min, c2min);
      
      // is this the best yet?
      if (tmpDist < minDist){
	minDist = tmpDist;
	c1 = c1min;
	c2 = c2min;
	trackIdx = t;
      }
      
    }// for all tracks in vector

    return minDist;
  }

  // Ref. RTCD Sec. 5.1.9
  double DistanceAlgo::_SqDist_(const LineSegment_t& seg1, const LineSegment_t& seg2,
				Point_t& c1, Point_t& c2) const
  {

    double t1, t2;

    Point_t s1 = seg1.Start();
    Point_t s2 = seg2.Start();
    Point_t e1 = seg1.End();
    Point_t e2 = seg2.End();

    Vector_t dir1 = e1 - s1;
    Vector_t dir2 = e2 - s2;
    Vector_t r   = s1 - s2;

    double len1 = dir1.Length();
    double len2 = dir2.Length();
    double f    = dir2 * r;

    // check if segment is too short
    if ( (len1 <= 0) and (len2 <= 0) ){
      //both segments are too short
      t1 = t2 = 0.;
      c1 = s1;
      c2 = s2;
      Vector_t distVector = c2 - c1;
      return distVector.Length();
    }
    if (len1 <= 0){
      //first segment degenerates into a point
      t1 = 0.;
      t2 = f/len2;
      t2 = _Clamp_(t2,0.,1.);
    }
    else{
      float c = dir1 * r;
      if (len2 <= 0){
	//second segment degenerates into a point
	t2 = 0.;
	t1 = _Clamp_(-c/len1,0.,1.);
      }
      else{
	// the general case...no degeneracies
	float b = dir1 * dir2;
	float denom = (len1*len2)-(b*b);
	
	if (denom != 0.)
	  t1 = _Clamp_((b*f-c*len2)/denom, 0., 1.);
	else
	  t1 = 0.;
	
	t2 = (b*t1+f)/len2;
	
	if (t2 < 0.){
	  t2 = 0.;
	  t1 = _Clamp_(-c/len1, 0., 1.);
	}
	else if (t2 > 1.){
	  t2 = 1.;
	  t1 = _Clamp_((b-c)/len1, 0., 1.);
	}
	
      }
    }
    
    c1 = s1 + dir1 * t1;
    c2 = s2 + dir2 * t2;
    
    Vector_t distVector = c2 - c1;
    return distVector.Length();
    
  }


  // Clamp function:
  // if 1st argument out of bounds w.r.t. min & max
  // return the boundary point
  double DistanceAlgo::_Clamp_(const double n, const double min, const double max) const
  {
    if (n < min) { return min; }
    if (n > max) { return max; }
    return n;
  }
  
}
#endif
