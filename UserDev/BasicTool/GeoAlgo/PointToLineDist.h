/**
 * \file PointToLineDist.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class PointToLineDist
 *
 * @author david
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_POINTTOLINEDIST_H
#define BASICTOOL_POINTTOLINEDIST_H

#include "GeoVector.h"
#include "GeoTrajectory.h"

namespace geoalgo {
  /**
     \class PointToLineDist
     User defined class PointToLineDist ... these comments are used to generate
     doxygen documentation!
  */
  class PointToLineDist{
    
  public:
    
    /// Default constructor
    PointToLineDist(){ _debug=false; };
    
    /// Default destructor
    virtual ~PointToLineDist(){};
    
    double Distance(const Point_t &point,
		    const Point_t &segmentStart,
		    const Point_t &segmentEnd) const;

    double DistanceToTrack(const Point_t &point,
			   const Trajectory_t &track) const;
    
    double DistanceToListOfTracks(const Point_t &p,
				  const std::vector<geoalgo::Trajectory_t> &trkList) const;
    
    double DistanceToTrack(const Point_t &p,
			   const Trajectory_t &trk,
			   const double &bufferDist) const;
    
    void TestDist() const;
    
    void SetDebug(bool on) { _debug = on; }
    
  private:
    
    bool _debug;

  };
}

#endif
/** @} */ // end of doxygen group 

