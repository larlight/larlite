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
#ifndef SEGMENTPOCA_H
#define SEGMENTPOCA_H

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


    double ClosestApproachToTrajectory(std::vector<std::vector<double>> traj,
				       std::vector<double> s2,
				       std::vector<double> e2,
				       std::vector<double> &c1,
				       std::vector<double> &c2,
				       double &t1,
				       double &t2);

    double ClosestApproach(std::vector<double> s1,
			   std::vector<double> e1,
			   std::vector<double> s2,
			   std::vector<double> e2,
			   std::vector<double> &c1,
			   std::vector<double> &c2,
			   double &t1,
			   double &t2);
    
    double Clamp(double n, double min, double max);

    double Dot(std::vector<double> v1, std::vector<double> v2);

    void TestPoCA(std::vector<double> s1, std::vector<double> e1,
		  std::vector<double> s2, std::vector<double> e2);


  private:
    
    double _epsilon;

  };

}

#endif
/** @} */ // end of doxygen group 

