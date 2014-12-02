/**
 * \file TwoLineIntersection.h
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
#ifndef BASICTOOL_TWOLINEINTERSECTION_H
#define BASICTOOL_TWOLINEINTERSECTION_H

#include "GeoVector.h"
#include "GeoTrajectory.h"

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
    Point_t Intersection3D(Point_t const& start1, 
			   Point_t const& dir1, 
			   Point_t const& start2, 
			   Point_t const& dir2);

  };
}

#endif
/** @} */ // end of doxygen group 

