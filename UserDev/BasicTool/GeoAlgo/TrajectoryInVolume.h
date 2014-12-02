/**
 * \file TrajectoryInVolume.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class TrajectoryInVolume
 *
 * @author David Caratelli
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_TRAJECTORYINVOLUME_H
#define BASICTOOL_TRAJECTORYINVOLUME_H

#include "GeoVector.h"
#include "GeoTrajectory.h"

namespace geoalgo {
  /**
     \class TrajectoryInVolume
     Takes in a vector of 3D points and Returns if the trajectory
     delimited by these points crosses a given TPC-like volume
     doxygen documentation!
  */
  class TrajectoryInVolume{
    
  public:
    
    /// Default constructor
    TrajectoryInVolume(){ _epsilon = 0.0001; };
    
    /// Default destructor
    virtual ~TrajectoryInVolume(){};
    
    /// Set Volume
    void SetVolume(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
    { _xMin = xMin; _xMax = xMax; _yMin = yMin; _yMax = yMax; _zMin = zMin; _zMax = zMax; }
    
    /// See if a list of trajectory points intersects the volume
    bool IsInVolume(const Trajectory_t& trj) const;
    
    /// See if Line intersects volume
    bool LineInVolume(const Point_t &point1, const Point_t &point2) const;
    
    /// See if point in volume
    bool PointInVolume(const Point_t &point) const;
    
    /// Measure distance between two points
    double Distance(const Point_t &point1, const Point_t &point2) const;
    
  private:
    
    /// epsilon value to determine if line parallel to one of x,y, or z axis.
    /// equal to cosine of angle
    double _epsilon;
    
    /// Volume boundaries:
    float _xMin, _xMax, _yMin, _yMax, _zMin, _zMax;
    
  };
}

#endif
/** @} */ // end of doxygen group 

