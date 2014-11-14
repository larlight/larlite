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
    PointToLineDist(){ _debug=false; };
    
    /// Default destructor
    virtual ~PointToLineDist(){};
    
  double Distance(std::vector<double> *point,
		  std::vector<double> *segmentStart,
		  std::vector<double> *segmentEnd);

  double DistanceToTrack(std::vector<double> *p, std::vector<std::vector<double> > *track);

  double DistanceToListOfTracks(std::vector<double> *p, std::vector<std::vector<std::vector<double> > > *trackList);

  double DistanceToTrack(std::vector<double> *p, std::vector<std::vector<double> > *track, double bufferDist);

  double DotProduct(std::vector<double> *A, std::vector<double> *B);

  void TestDist();

  void SetDebug(bool on) { _debug = on; }

  private:
    
    bool _debug;
    
  };
}

#endif
/** @} */ // end of doxygen group 

