/**
 * \file GeoHalfLine.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class HalfLine
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOHALFLINE_H
#define BASICTOOL_GEOHALFLINE_H

#include "GeoVector.h"
namespace geoalgo {
  /**
     \class HalfLine
     @brief Representation of a 3D semi-infinite line.
     Defines a semi-infinite 3D line by having a start point (Point_t) and direction (Vector_t) \n
     along which the line extends. It hides the start and direction attributes from users for   \n
     protecting the dimensionality.
  */
  class HalfLine {
    
  public:
    
    /// Default constructor
    HalfLine() 
      : _start(3)
      , _dir(3)
    {Normalize();}

    /// Alternative ctor (1)
    HalfLine(const double x,    const double y,    const double z,
	     const double dirx, const double diry, const double dirz)
      : _start (x,    y,    z   )
      , _dir   (dirx, diry, dirz)
    {Normalize();}

    /// Altenartive ctor (2)
    HalfLine(const Point_t& start, const Vector_t& dir)
      : _start ( start )
      , _dir   ( dir   )
    { 
      if(start.size()!=3 || dir.size()!=3)
	throw GeoAlgoException("HalfLine ctor accepts only 3D Point!");
      Normalize();
    }

    /// Alternative ctor using template (3)
    template <class T, class U> HalfLine(const T& start, const U& dir)
      : HalfLine(Point_t(start), Point_t(dir))
    {}

    /// Default destructor
    virtual ~HalfLine(){};

    /// Start getter
    const Point_t& Start() const { return _start; }

    /// Direction getter
    const Vector_t& Dir() const { return _dir; }
    
    /// Start setter
    void Start(const double x, const double y, const double z)
    { _start[0] = x; _start[1] = y; _start[2] = z; }

    /// Dir setter
    void Dir(const double x, const double y, const double z)
    { _dir[0] = x; _dir[1] = y; _dir[2] = z; Normalize(); }

  protected:

    /// Normalize direction
    void Normalize()
    {
      auto l = _dir.SqLength();
      if(!l)
	throw GeoAlgoException("<<Normalize>> cannot normalize 0-length direction vector!");
      _dir /= l;
    }

    /// Beginning of the half line
    Point_t  _start;

    /// Direction of the half line from _start
    Vector_t _dir;

  };
  
  typedef HalfLine HalfLine_t;
}
#endif
/** @} */ // end of doxygen group 

