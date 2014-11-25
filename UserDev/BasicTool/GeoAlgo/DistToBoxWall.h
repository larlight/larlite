/**
 * \file DistToBoxWall.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class DistToBoxWall
 *
 * @author kazu 
 */

/** \addtogroup GeoAlgo
    
    @{*/
#ifndef LARLITE_DISTTOBOXWALL_H
#define LARLITE_DISTTOBOXWALL_H

#include "GeoAlgoException.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <TVector3.h>
#include <TLorentzVector.h>

namespace geoalgo {

  /**
     \class DistToBoxWall
     User defined class DistToBoxWall ... these comments are used to generate
     doxygen documentation!
  */
  
  class DistToBoxWall{
    
  public:
    
    /// Default constructor
    DistToBoxWall();
    
    /// Alternative constructor
    DistToBoxWall(double const x_min, double const x_max,
		  double const y_min, double const y_max,
		  double const z_min, double const z_max);
    
    /// Default deconstructor 
    virtual ~DistToBoxWall(){};
    
    /// Reset function to get default values
    void Reset();
    
    /// Setter for maximum coordinate value (if you want to change)
    void SetXYZMax(double const x, double const y, double const z)
    {
      _xyz_max[0] = x;
      _xyz_max[1] = y;
      _xyz_max[2] = z;
    }
    
    /// Setter for minimum coordinate value (if you want to change)
    void SetXYZMin(double const x, double const y, double const z)
    {
      _xyz_min[0] = x;
      _xyz_min[1] = y;
      _xyz_min[2] = z;
    }
    
    /// Base getter for a distance from a 3-point to the box boundary
    double DistanceToWall(double x, double y, double z) const;

    /// Getter for a distance from a std::vector point to the box boundary
    double DistanceToWall(std::vector<double> const& point) const;
    
    /// Getter for distance from a TLorentzVector point to the box boundary
    double DistanceToWall(TLorentzVector const& point) const
    { return DistanceToWall(point.X(),point.Y(),point.Z()); }

    /// Getter for distance from a TVector3 point to the box boundary
    double DistanceToWall(TVector3 const& point) const
    { return DistanceToWall(point.X(),point.Y(),point.Z()); }

    /// Wrapper for TLorentzVector + TLorentzVector
    double DistanceToWall(TLorentzVector const& point,
			  TLorentzVector const& dir,
			  bool ForwardOrBack) const
    {  return DistanceToWall(point.X(),point.Y(),point.Z(),dir.X(),dir.Y(),dir.Z(),ForwardOrBack); }

    /// Wrapper for TVector3 + TVector3
    double DistanceToWall(TVector3 const& point,
			  TVector3 const& dir,
			  bool ForwardOrBack) const {

      return DistanceToWall(point.X(),point.Y(),point.Z(),dir.X(),dir.Y(),dir.Z(),ForwardOrBack);
    };

    /// Getter for a distance from a point to the box boundary along a direction (or backwards)
    /// Specify 0 for backwards, 1 for forwards
    double DistanceToWall(std::vector<double> const& point,
			  std::vector<double> dir,
			  bool ForwardOrBack ) const;
    
    double DistanceToWall(double x, double y, double z, 
			  double dirx, double diry, double dirz, 
			  bool ForwardOrBack) const;

    /// Getter for distance from point to box (when point is outside box)
    double DistanceToWallFromOut(std::vector<double> const &point) const;
    
  protected:
    
    /// "rectangular box" xyz minimum coordinate values
    std::vector<double> _xyz_min;
    
    /// "rectangular box" xyz maximum coordinate values
    std::vector<double> _xyz_max;
    
    
  };
  
}
#endif
/** @} */ // end of doxygen group 

