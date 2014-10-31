/**
 * \file TwoLineIntersection.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoAlgo
 * \brief This class takes in two (point + direction) vectors and finds the
 * intersection in 3D space (or, the closest thing to an intersection
 * since two 3D lines frequently don't intersect).
 * see: http://paulbourke.net/geometry/pointlineplane/
 *
 * @author davidkaleko
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef TWOLINEINTERSECTION_HH
#define TWOLINEINTERSECTION_HH

#include <iostream>
#include <vector>
#include <limits>
#include <climits>
#include <math.h> //for pow

namespace geoalgo {
  /**
     \class TwoLineIntersection
     User defined class TwoLineIntersection ... these comments are used to generate
     doxygen documentation!
  */
  
  class TwoLineIntersection {
    
  public:
    
    /// Default constructor
    TwoLineIntersection(){}
    
    /// Default destructor
    virtual ~TwoLineIntersection(){}
    
    //Format of returned vector is (x,y,z,error)
    //where (x,y,z) are the coordinates of the best-intersection
    //(midpoint of shortest line connecting the two 3D input lines)
    //and error is the length^2 of the shortest line connecting the two
    //3D input lines (0 error means the 3D lines actually intersect)  
    std::vector<double> Intersection3D(std::vector<double> const& start1, 
				       std::vector<double> const& dir1, 
				       std::vector<double> const& start2, 
				       std::vector<double> const& dir2);
    
  };
}

#endif
/** @} */ // end of doxygen group 

