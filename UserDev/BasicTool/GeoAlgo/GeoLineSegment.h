/**
 * \file GeoLineSegment.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class LineSegment
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOLINESEGMENT_H
#define BASICTOOL_GEOLINESEGMENT_H

#include "GeoVector.h"

namespace geoalgo {
  /**
     \class LineSegment
     @brief Representation of a simple 3D line segment
     Defines a finite 3D straight line by having the start and end position (Point_t). \n
  */
  class LineSegment{
    
  public:
    
    /// Default constructor
    LineSegment()
      : _start(3)
      , _end(3)
      , _dir(3)
    {DirReset();}
    
    /// Default destructor
    virtual ~LineSegment(){}

    /// Alternative ctor (1)
    LineSegment(const double start_x, const double start_y, const double start_z,
		const double end_x,   const double end_y,   const double end_z   )
      : _start ( start_x, start_y, start_z )
      , _end   ( end_x,   end_y,   end_z   )
      , _dir   (3)
    {DirReset();}

    /// Altenartive ctor (2)
    LineSegment(const Point_t& start, const Vector_t& end)
      : _start ( start )
      , _end   ( end   )
      , _dir   (3)
    { 
      if(start.size()!=3 || end.size()!=3)
	throw GeoAlgoException("LineSegment ctor accepts only 3D Point!");
      DirReset();
    }

    /// Alternative ctor using template (3)
    template <class T, class U> LineSegment(const T& start, const U& end)
      : LineSegment(Point_t(start), Point_t(end))
    {}

    /// Start getter
    const Point_t& Start() const { return _start; }

    /// End getter
    const Point_t& End() const { return _end; }

    /// Direction getter
    const Vector_t Dir() const { return _dir; }

    /// Start setter
    void Start(const double x, const double y, const double z)
    { _start[0] = x; _start[1] = y; _start[2] = z; 
      DirReset();
    }

    /// End setter
    void End(const double x, const double y, const double z)
    { _end[0] = x; _end[1] = y; _end[2] = z; 
      DirReset();
    }

  protected:

    /// Internal function to reset direction
    void DirReset() { _dir = _end - _start; }

    /// Start position of a line
    Point_t _start;

    /// End position of a line
    Point_t _end;

    /// Direction
    Vector_t _dir;
    
  };

  typedef LineSegment LineSegment_t;
}

#endif
/** @} */ // end of doxygen group 

