#ifndef SEGMENTPOCA_CXX
#define SEGMENTPOCA_CXX

#include "SegmentPoCA.h"

namespace geoalgo {


  double SegmentPoCA::ClosestApproachToTrajectory(const Trajectory_t &traj,
						  const Point_t &s2,
						  const Point_t &e2,
						  Point_t &c1,
						  Point_t &c2) const
  {

    traj.compat(s2);
    traj.compat(e2);
    
    double min = 99999999;
    Point_t c1tmp, c2tmp;
    for (size_t i=0; i < (traj.size()-1); i++){
      double minTmp = ClosestApproach(traj[i],traj[i+1],s2,e2,c1tmp,c2tmp);
      if (minTmp < min) {
	min = minTmp;
	c1 = c1tmp;
	c2 = c2tmp;
      }
    }//for all trajectory segments

    return min;
  }
  
  // Find smallest distance between segmens (s1-e1) and (s2-e2)
  // See "Real-Time Collision Analysis" Sec. 5.1.9
  double SegmentPoCA::_ClosestApproach_(const Point_t &s1,
					const Point_t &e1,
					const Point_t &s2,
					const Point_t &e2,
					Point_t &c1,
					Point_t &c2) const
  {
    
    // c1 is the closest point on line 1
    // c2 is the closest point on line 2
    // t1 is the parametrized point for the first segment (line = s1 + t1*(e1-s1)) with t1 from 0 to 1
    // t2 is the parametrized point for the first segment (line = s2 + t2*(e2-s2)) with t2 from 0 to 1
    double t1, t2;
    
    Point_t dir1 = e1 - s1;
    Point_t dir2 = e2 - s2;
    Point_t r = s1 - s2;

    double len1 = dir1.Length();
    double len2 = dir2.Length();
    double f = dir2 * r;

    // Check if segments too short
    if ( (len1 < _epsilon) and (len2 < _epsilon) ){
      //both segments are too short
      t1 = t2 = 0.;
      c1 = s1;
      c2 = s2;
      Point_t distVector = c2 - c1;
      return distVector.Length();
    }
    if (len1 < _epsilon){
      //first segment degenerates into a point
      t1 = 0.;
      t2 = f/len2;
      t2 = Clamp(t2,0.,1.);
    }
    else{
      float c = dir1 * r;
      if (len2 < _epsilon){
	//second segment degenerates into a point
	t2 = 0.;
	t1 = Clamp(-c/len1,0.,1.);
      }
      else{
	// the general case...no degeneracies
	float b = dir1 * dir2;
	float denom = (len1*len2)-(b*b);
	
	if (denom != 0.)
	  t1 = Clamp((b*f-c*len2)/denom, 0., 1.);
	else
	  t1 = 0.;

	t2 = (b*t1+f)/len2;

	if (t2 < 0.){
	  t2 = 0.;
	  t1 = Clamp(-c/len1, 0., 1.);
	}
	else if (t2 > 1.){
	  t2 = 1.;
	  t1 = Clamp((b-c)/len1, 0., 1.);
	}

      }
    }
    
    c1 = s1 + dir1 * t1;
    c2 = s2 + dir2 * t2;

    Point_t distVector = c2 - c1;
    return distVector.Length();
    
  }

  double SegmentPoCA::Clamp(const double n, 
			    const double min, 
			    const double max) const
  {

    if (n < min) { return min; }
    if (n > max) { return max; }
    return n;
  }
  
  void SegmentPoCA::TestPoCA(const Point_t &s1, 
			     const Point_t &e1,
			     const Point_t &s2, 
			     const Point_t &e2) const
  {
    Point_t c1;
    Point_t c2;

    double distance = ClosestApproach(s1, e1, s2, e2, c1, c2);
    std::cout << "line 1: [" << s1[0] << ", " << s1[1] << ", " << s1[2] << "]   --.    ["
	      << e1[0] << ", " << e1[1] << ", " << e1[2] << "]" << std::endl;
    std::cout << "line 2: [" << s2[0] << ", " << s2[1] << ", " << s2[2] << "]   --.    ["
	      << e2[0] << ", " << e2[1] << ", " << e2[2] << "]" << std::endl;
    std::cout << "Distance between two segments is: " << distance << std::endl;
    std::cout << "Closest point on line1: [" << c1[0] << ", " << c1[1] << ", " << c1[2] << "]" << std::endl;
    std::cout << "Closest point on line2: [" << c2[0] << ", " << c2[1] << ", " << c2[2] << "]" << std::endl;
    return;
  }
}

#endif
