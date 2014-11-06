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
#ifndef POINTTOLINEDIST_HH
#define POINTTOLINEDIST_HH

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
    PointToLineDist(){ _point.clear(); _segmentStart.clear(); _segmentEnd.clear(); _debug=false; };
    
    /// Default destructor
    virtual ~PointToLineDist(){};
    
  double Distance();

  double Distance(std::vector<double> point,
		  std::vector<double> segmentStart,
		  std::vector<double> segmentEnd) { _point = point; _segmentStart = segmentStart; _segmentEnd = segmentEnd; return Distance(); }

  double DistanceToTrack(std::vector<double> p, std::vector<std::vector<double> > track);

  double DistanceToListOfTracks(std::vector<double> p, std::vector<std::vector<std::vector<double> > > *trackList);

  double DistanceToTrack(std::vector<double> p, std::vector<std::vector<double> > track, double bufferDist);

  double DotProduct(std::vector<double> A, std::vector<double> B);

  void TestDist();

  void SetDebug(bool on) { _debug = on; }

  private:
    
    std::vector<double>  _point;
    std::vector<double>  _segmentStart;
    std::vector<double> _segmentEnd;
    
    bool _debug;
    
  };
}

#endif
/** @} */ // end of doxygen group 

