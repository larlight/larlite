#ifndef DISTTOBOXWALL_CXX
#define DISTTOBOXWALL_CXX

#include "DistToBoxWall.h"

namespace geoalgo {
  
  DistToBoxWall::DistToBoxWall()
    : _xyz_min(3,0), _xyz_max(3,0)
  {
    Reset();
  }
  
  DistToBoxWall::DistToBoxWall(double const x_min, double const x_max,
                   double const y_min, double const y_max,
                   double const z_min, double const z_max)
    : _xyz_min(3,0), _xyz_max(3,0)
  {
    Reset();
    SetXYZMax(x_max,y_max,z_max);
    SetXYZMin(x_min,y_min,z_min);
  }
  
  void DistToBoxWall::Reset() 
  {
    _xyz_min[0] = 0.; 
    _xyz_min[1] = -116.5;
    _xyz_min[2] = 0.; 
    
    _xyz_max[0] = 256.35;
    _xyz_max[1] = 116.5;
    _xyz_max[2] = 1036.8;
  }
  
  double DistToBoxWall::DistanceToWall(std::vector<double> const& point) const
  {

    if( point.size() < 3) {
    
      std::ostringstream msg;
      msg
    << "<<" << __FUNCTION__ << ">>"
    << " expects length 3 vector for point ... given: "
    << point.size()
    << std::endl;
      throw GeoAlgoException(msg.str());
    }
	
	//    
	// This function computes the distance from a point to the closest wall of the box.
	// The box is pre-defined by _xyz_min and _xyz_max values.
	// The function only works only if the point is inside the box for now because
	// otherwise it adds a complication to the problem.
	// 
	// The function proceeds in the following steps of computation:
	//
	// (1) Check if a point is within the box. If not, return -1.
	// (2) Compute the distance to the YZ wall (i.e. x=0 plane)
	// (3) Compute the distance to the ZX wall (i.e. y=0 plane)
	// (4) Compute the distance to the XY wall (i.e. z=0 plane)
	// (5) Return the minimum of (2), (3) and (4).
	
	//
	// (1) Check if a point is inside the box. If not, return -1.
	//
	if( (point[0] < _xyz_min[0] || _xyz_max[0] < point[0]) || // point is outside X boundaries OR
  	(point[1] < _xyz_min[1] || _xyz_max[1] < point[1]) || // point is outside Y boundaries OR
   	(point[2] < _xyz_min[2] || _xyz_max[2] < point[2]) )  // point is outside Z boundaries 
		return -99; 

	//
    // (2) Compute the distance to the YZ wall
    //   
 
    double dist_to_yz = point[0] - _xyz_min[0];
    if( dist_to_yz > (_xyz_max[0] - point[0]) ) dist_to_yz = _xyz_max[0] - point[0];

	//
    // (3) Compute the distance to the XZ wall
    //   
	double dist_to_zx = point[1] - _xyz_min[1];
    if( dist_to_zx > (_xyz_max[1] - point[1]) ) dist_to_zx = _xyz_max[1] - point[1];

	//
    // (4) Compute the distance to the XY wall
    //   
    double dist_to_xy = point[2] - _xyz_min[2];
    if( dist_to_xy > (_xyz_max[2] - point[2]) ) dist_to_xy = _xyz_max[2] - point[2];

    //
    // (6) Return the minimum of (3), (4), and (5)
    //
    double dist = ( dist_to_yz < dist_to_zx ? dist_to_yz : dist_to_zx );

    dist = ( dist < dist_to_xy ? dist : dist_to_xy );

    return dist;
  }

double DistToBoxWall::DistanceToWall(std::vector<double> const& point,
                       std::vector<double> dir, bool ForwardOrBack ) const
  {

    if( point.size() < 3 || dir.size() <3) {

      std::ostringstream msg;
      msg
    << "<<" << __FUNCTION__ << ">>"
    << " expects length 3 vector for point & dir ... given: "
    << point.size() << " and " << dir.size()
    << std::endl;
      throw GeoAlgoException(msg.str());
    }

	//This function computes the distance from a point to the closest wall of the box
	// along the specified direction (both taken from input arguments). The box is
	// pre-defined by _xyz_min and _xyz_max values. Specify 0 for backwards, 1 for forwards. 
    // The function only works only if the point is inside the box for now because
    // otherwise it adds a complication to the problem.
    // 
    // The function proceeds in the following steps of computation:
    //
    // (1) Check if a point is within the box. If not, return -1.
    // (2) Normalize dir to make it into a unit vector.
    // (3) Compute distance to reach YZ plane (i.e. x=0 plane)
    // (4) Compute distance to reach ZX plane (i.e. y=0 plane)
    // (5) Compute distance to reach XY plane (i.e. z=0 plane)
    // (6) Return the minimum of (3), (4), and (5)
    //

	//
	// (1) Check if a point is inside the box. If not, return -1.
	//
	if( (point[0] < _xyz_min[0] || _xyz_max[0] < point[0]) || // point is outside X boundaries OR
    (point[1] < _xyz_min[1] || _xyz_max[1] < point[1]) || // point is outside Y boundaries OR
    (point[2] < _xyz_min[2] || _xyz_max[2] < point[2]) )  // point is outside Z boundaries 
	return -99;

	//
	// (2) Normalize dir vector
	//
    double dir_magnitude = sqrt( pow(dir[0],2) + pow(dir[1],2) + pow(dir[2],2) );
    dir[0] /= dir_magnitude;
    dir[1] /= dir_magnitude;
    dir[2] /= dir_magnitude;

	//
	// (2.5) Swap direction if user wants backwards direction
	// 	
    if(!ForwardOrBack){
      dir[0] *= -1 ;
      dir[1] *= -1 ;
      dir[2] *= -1 ;
    }

	//
	// (3) Compute distance to reach YZ plane
	//

	double dist_to_yz = 0;
    if(dir[0] < 0 )

      dist_to_yz = (point[0] - _xyz_min[0]) / (-1. * dir[0]);

    else

      dist_to_yz = (_xyz_max[0] - point[0]) / dir[0];

	//
	// (4) Compute distance to reach XY plane
	//
    double dist_to_xy = 0;
    if(dir[2] < 0)

      dist_to_xy = (point[2] - _xyz_min[2]) / (-1. * dir[2]);

    else

      dist_to_xy = (_xyz_max[2] - point[2]) / dir[2];


	//
	// (5) Compute distance to reach XZ plane
	//
 	double dist_to_zx = 0;
    if(dir[1] < 0)

      dist_to_zx = (point[1] - _xyz_min[1]) / (-1. * dir[1]);

    else

      dist_to_zx = (_xyz_max[1] - point[1]) / dir[1];

	//
	//(6) Return the minimum of (3), (4), and (5)
	//
   double dist = ( dist_to_yz < dist_to_zx ? dist_to_yz : dist_to_zx );
   
    dist = ( dist < dist_to_xy ? dist : dist_to_xy ); 

    return dist;
  }


  double DistToBoxWall::DistanceToWallFromOut(std::vector<double> const &point) const{

    //
    // This function computes the distance from a point outside the box 
    // to the nearest wall of the box.
    // 
    // The function proceeds in the following steps of computation:
    //
    // (1) Check if a point is within the box. If so, return -99.
    // (2) Compute shortest distance from point to whichever boundary the point exceeded 
    // (3) Thas all.
    //

    // (1) Check if point is inside box--if it is, return -99
 	if( (point[0] > _xyz_min[0] && _xyz_max[0] > point[0]) && // point is inside X boundaries OR
    (point[1] > _xyz_min[1] && _xyz_max[1] > point[1]) && // point is inside Y boundaries OR
    (point[2] > _xyz_min[2] && _xyz_max[2] > point[2]) )  // point is inside Z boundaries 

      return -99;

	// (2) Compute shortest distance
    double x_dist_min = abs(point[0] - _xyz_min[0]) ;
    double y_dist_min = abs(point[1] - _xyz_min[1]) ;
    double z_dist_min = abs(point[2] - _xyz_min[2]) ;

    double x_dist_max = abs(point[0] - _xyz_max[0]) ;
    double y_dist_max = abs(point[1] - _xyz_max[1]) ;
    double z_dist_max = abs(point[2] - _xyz_max[2]) ;

    if( point[0] > _xyz_max[0] || point[0] < _xyz_min[0] )

      return ( x_dist_min < x_dist_max ? x_dist_min : x_dist_max ) ;

    else if( point[1] > _xyz_max[1] || point[1] < _xyz_min[1])

      return ( y_dist_min < y_dist_max ? y_dist_min : y_dist_max ) ;

    else

      return ( z_dist_min < z_dist_max ? z_dist_min : z_dist_max ) ;
   
  }
 
 
}


#endif
