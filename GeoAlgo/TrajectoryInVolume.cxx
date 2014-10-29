#ifndef TRAJECTORYINVOLUME_CXX
#define TRAJECTORYINVOLUME_CXX

#include "TrajectoryInVolume.h"


void TrajectoryInVolume::AddTrajectoryPoint(std::vector<float> point){
 
  if (point.size() != 3)//allow only for 3D stuff
    std::cout << "Warning: Point added to trajectory does not have 3 coordinates. Point not added..." << std::endl;

  else
    _traj.push_back(point);

  return;
}

bool TrajectoryInVolume::IsInVolume(){

  for (size_t n=0; n < (_traj.size()-1); n++){
    if ( LineInVolume(_traj.at(n),_traj.at(n+1)) )
      return true;
  }
  return false;
}

bool TrajectoryInVolume::IsInVolume(std::vector<std::vector<float> > traj){

  _traj = traj;
  
  for (size_t n=0; n < (_traj.size()-1); n++){
    if ( LineInVolume(_traj.at(n),_traj.at(n+1)) )
      return true;
  }
  return false;
}


bool TrajectoryInVolume::LineInVolume(std::vector<float> point1, std::vector<float> point2){

  //first make sure neither point is inside volume. If so then trajectory is in volume!
  if ( PointInVolume(point1) or PointInVolume(point2) )
    return true;
  
  //else check intersections
  else{

    double tMin =  99999;
    double tMax = -99999;
    
    for (int i=0; i < 3; i++){
      // if line along this plane is almost parallel to volume planes
      // since we already established that the points are both out of volume
      // then the line cannot intersect the volume
      if ( ((point1.at(i) - point2.at(i))/Distance(point1,point2)) < _epsilon)
	return false;
      // else find intersections
      else{
	// line parametrized as x(t) = x1 + (x2-x1)*t
	//                      y(t) = y1 + (y2-y1)*t
	//                      z(t) = z1 + (z2-z1)*t
	// t parameter keeps track of a point on the line
	// find t-point for interesection with volume boundaries for all coordinates
	float t1 = (_xMin-point1.at(i))/(point2.at(i)-point1.at(i));
	float t2 = (_xMax-point1.at(i))/(point2.at(i)-point1.at(i));
	// make t1 intersection with near plane, t2 intersection with far plane
	// i.e. don't care about line's orientation
	if ( t1 > t2 ) { double tmp = t1; t1 = t2; t2 = tmp; }
	// now compare with tMin and tMax:
	// minimum and maximum times t of parametrization
	// point of line being in volume
	if ( t1 > tMin ) { tMin = t1; }
	if ( t2 < tMax ) { tMax = t2; }
	//if t-space becomes empty => no intersection
	if ( tMin > tMax )
	  return false;

      }//if not parallel

    }//for all 3 coordinates

  }//if neither point in volume
  
  return true;
}

bool TrajectoryInVolume::PointInVolume(std::vector<float> point){

  if ( (point.at(0) > _xMin) and (point.at(0) < _xMax) and
       (point.at(1) > _yMin) and (point.at(1) < _yMax) and
       (point.at(2) > _zMin) and (point.at(2) < _zMax) )
    return true;
  
  return false;
}

bool TrajectoryInVolume::PointInVolume(std::vector<double> point){

  if ( (point.at(0) > _xMin) and (point.at(0) < _xMax) and
       (point.at(1) > _yMin) and (point.at(1) < _yMax) and
       (point.at(2) > _zMin) and (point.at(2) < _zMax) )
    return true;
  
  return false;
}

double TrajectoryInVolume::Distance(std::vector<float> point1, std::vector<float> point2){
  
  double dist = sqrt( (point1.at(0)-point2.at(0))*(point1.at(0)-point2.at(0)) +
		      (point1.at(1)-point2.at(1))*(point1.at(1)-point2.at(1)) +
		      (point1.at(2)-point2.at(2))*(point1.at(2)-point2.at(2)) );
  
  return dist;
}

#endif
