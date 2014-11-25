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

#include "GeoAlgoException.h"
#include <iostream>
#include <vector>
#include <math.h>

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
    
  double Distance(const std::vector<double> &point,
		  const std::vector<double> &segmentStart,
		  const std::vector<double> &segmentEnd) const;

  double DistanceToTrack(const std::vector<double> &p, 
			 const std::vector<std::vector<double> > &track) const;

  double DistanceToListOfTracks(const std::vector<double> &p, 
				const std::vector<std::vector<std::vector<double> > > &trackList) const;

  double DistanceToTrack(const std::vector<double> &p, 
			 const std::vector<std::vector<double> > &track, 
			 const double &bufferDist) const;

  double DotProduct(const std::vector<double> &A, 
		    const std::vector<double> &B) const;

  void TestDist() const;

  void SetDebug(bool on) { _debug = on; }

  private:
    
    bool _debug;
    
  };
}

#endif
/** @} */ // end of doxygen group 

