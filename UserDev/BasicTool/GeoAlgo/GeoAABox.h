/**
 * \file GeoSimpleBox.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class AABox
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOAABox_H
#define BASICTOOL_GEOAABox_H

#include "GeoHalfLine.h"

namespace geoalgo {
  /**
     \class AABox
     @brief Representation of a 3D rectangular box which sides are aligned w/ coordinate axis.
     A representation of an Axis-Aligned-Boundary-Box, a simple & popular representation of   \n
     3D boundary box for collision detection. The concept was taken from the reference,       \n
     Real-Time-Collision-Detection (RTCD), and in particular Ch. 4.2 (page 77):               \n

     Ref: http://realtimecollisiondetection.net

     This class uses one of the simplest representation for AABox: "min-max" representation.   \n
     Though this method requires storing 6 floating point values, class attributes (i.e.      \n
     "min" and "max" points) store intuitive values for most UB analyzers. Also it simplifies \n
     utility function implementations.
  */
  class AABox {
    
  public:
    
    /// Default constructor
    AABox() 
      : _min(3)
      , _max(3)
    {}
    
    /// Alternative ctor (1)
    AABox(const double x_min, const double y_min, const double z_min,
	 const double x_max, const double y_max, const double z_max)
      : _min ( x_min, y_min, z_min )
      , _max ( x_max, y_max, z_max )
    {}
    
    /// Altenartive ctor (2)
    AABox(const Point_t& min, const Vector_t& max)
      : _min ( min )
      , _max ( max   )
    { 
      if(min.size()!=3 || max.size()!=3)
	throw GeoAlgoException("AABox ctor accepts only 3D Point!");
    }

    /// Alternative ctor using template (3)
    template <class T, class U> AABox(const T& min, const U& max)
      : AABox(Point_t(min), Point_t(max))
    {}

    /// Default destructor
    virtual ~AABox(){};

    //
    // Attribute accessor
    //
    /// Minimum point getter
    const Point_t& Min() const { return _min; }
    /// Maximum point getter
    const Point_t& Max() const { return _max; }
    /// Minimum point setter
    void Min(const double x, const double y, const double z)
    { _min[0] = x; _min[1] = y; _min[2] = z; }
    /// Maximum point setter
    void Max(const double x, const double y, const double z)
    { _max[0] = x; _max[1] = y; _max[2] = z; }

    /// Test if a point is contained within the box ... so simple that this is a class attribute
    bool Contain(const Point_t &pt) const {
      return !( (pt[0] < _min[0] || _max[0] < pt[0]) || // point is outside X boundaries OR
		(pt[1] < _min[1] || _max[1] < pt[1]) || // point is outside Y boundaries OR
		(pt[2] < _min[2] || _max[2] < pt[2])    // point is outside Z boundaries
		);
    }

  protected:

    /// Minimum point
    Point_t _min;

    /// Maximum point
    Point_t _max;
    
  };

  typedef AABox AABox_t;
}
#endif
/** @} */ // end of doxygen group 

