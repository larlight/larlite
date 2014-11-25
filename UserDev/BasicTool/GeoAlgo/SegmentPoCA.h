/**
 * \file SegmentPoCA.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoAlgo
 * \brief This class takes in two (point + direction) vectors and finds the
 * intersection in 3D space (or, the closest thing to an intersection
 * since two 3D lines frequently don't intersect).
 * see: http://paulbourke.net/geometry/pointlineplane/
 *
 * @author David Caratelli
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_SEGMENTPOCA_H
#define BASICTOOL_SEGMENTPOCA_H

#include <iostream>
#include <vector>
#include <math.h> //for pow

namespace geoalgo {
  /**
     \class SegmentPoCA
     User defined class SegmentPoCA ... these comments are used to generate
     doxygen documentation!
  */
  
  class SegmentPoCA {
    
  public:
    
    /// Default constructor
    SegmentPoCA(){_epsilon=0.000001;}
    
    /// Default destructor
    virtual ~SegmentPoCA(){}


    double ClosestApproachToTrajectory(const std::vector<std::vector<double>> &traj,
				       const std::vector<double> &s2,
				       const std::vector<double> &e2,
				       std::vector<double> &c1,
				       std::vector<double> &c2) const;

    double ClosestApproach(const std::vector<double> &s1,
			   const std::vector<double> &e1,
			   const std::vector<double> &s2,
			   const std::vector<double> &e2,
			   std::vector<double> &c1,
			   std::vector<double> &c2) const;
    
    double Clamp(const double n, const double min, const double max) const;

    double Dot(const std::vector<double> &v1, 
	       const std::vector<double> &v2) const;

    void TestPoCA(const std::vector<double> &s1, 
		  const std::vector<double> &e1,
		  const std::vector<double> &s2, 
		  const std::vector<double> &e2) const;


  private:
    
    double _epsilon;

  };

}

#endif
/** @} */ // end of doxygen group 

