#ifndef BASICTOOL_GEOCYLINDER_CXX
#define BASICTOOL_GEOCYLINDER_CXX

#include "GeoCylinder.h"

namespace geoalgo {

  Cylinder::Cylinder() 
    : _min(3)
    , _max(3)
  {}
    
  Cylinder::Cylinder(const double x_min, const double y_min, const double z_min,
			 const double x_max, const double y_max, const double z_max,
			 const double radius)
    : _min ( x_min, y_min, z_min )
    , _max ( x_max, y_max, z_max )
    , _radius ( radius )
  {}
    
  Cylinder::Cylinder(const Point_t& min, const Vector_t& max, const double radius)
    : _min ( min )
    , _max ( max   )
    , _radius ( radius )
  { 
    if(min.size()!=3 || max.size()!=3)
      throw GeoAlgoException("Cylinder ctor accepts only 3D Point!");
  }
  
  bool Cylinder::Contain(const Point_t &pt) const {

    // get a vector that defines the axis of the cylinder
    Vector_t axis = _max-_min;
    Vector_t dirpt = pt-_min;

    // angle of point w.r.t. the axis
    double angleMin = axis.Angle(dirpt);
    
    // if the angle is > 90 -> outside -> return
    if (angleMin > 0.5*3.14/180)
      return false;
    
    // revert the axis direction
    axis = _min-_max;
    dirpt = pt-_max;
    angleMin = axis.Angle(dirpt);
    
    // if the angle is > 90 -> outside -> return
    if (angleMin > 0.5*3.14/180)
      return false;

    // if still here, all that is left to verify is
    // that the point isn't more than a radius
    // away from the cylinder axis
    // 1) make a line corresponding to the axis
    Line_t line(_min,_max);
    // 2) get the distance between the point and the line
    double radial_dist_sq = _geoAlgo.SqDist(line,pt);
    
    if (radial_dist_sq > _radius*_radius)
      return false;
    
    return true;

  }
}
#endif


