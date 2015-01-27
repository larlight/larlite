/**
 * \file GeoCone.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class HalfLine
 *
 * @author David Caratelli
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOCONE_H
#define BASICTOOL_GEOCONE_H

#include "GeoVector.h"
#include "GeoHalfLine.h"
namespace geoalgo {
  /**
     \class Cone
     @brief Representation of a 3D semi-infinite line.
     Defines a 3D cone with the following properties:                                           \n
     Start point (or vertex), Direction, Length (or Length), Radius, opening angle              \n
     When 2 of Length, Radius, opening angle are defined the third is automatically set         
  */
  class Cone : public HalfLine {
    
  public:
    
    /// Default constructor
  Cone() : HalfLine()
      {
	_length = 1;
	_radius = 1;
	_angle = atan(_radius/_length);
      }
    
    /// Alternative ctor (1)
  Cone(const double x,    const double y,    const double z,
       const double dirx, const double diry, const double dirz,
       const double length, const double radius)
    : HalfLine(x, y, z, dirx, diry, dirz)
      {
	if (length == 0){
	  std::ostringstream msg;
	  msg << "<<" << __FUNCTION__ << ">>"
	      << " Cone Length cannot be 0." << std::endl;
	  throw GeoAlgoException(msg.str());
	}
	_length = length;
	_radius = radius;
	_angle = atan(_radius/_length);
      }
    
    /// Altenartive ctor (2)
  Cone(const Point_t& start, const Vector_t& dir,
       const double length, const double radius)
    : HalfLine( start,  dir )
      { 
      if (length == 0){
	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " Cone Length cannot be 0." << std::endl;
	throw GeoAlgoException(msg.str());
      }
      _length = length;
      _radius = radius;
      _angle  = atan(_radius/_length);
    }

    /// Alternative ctor using template (3)
    template <class T, class U> Cone(const T& start, const U& dir)
      : Cone(Point_t(start), Vector_t(dir))
    {}

    /// Default destructor
    virtual ~Cone(){};

    /// Length getter
    const double Length() const { return _length; }

    /// Length getter
    const double Radius() const { return _radius; }

    /// Angle getter
    const double Angle() const { return _angle; }
    
    /// Length setter
    void Length(const double l) 
    { 
      if (l == 0){
	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " Cone Length cannot be 0." << std::endl;
	throw GeoAlgoException(msg.str());
      }
      _length = l; 
      _angle = atan(_radius/_length);
    }

    /// Length setter
    void Radius(const double r) { _radius = r; _angle = atan(_radius/_length); }

  protected:

    /// Helight (length) of the cone
    double _length;
    
    /// Radius of the cone at the base
    double _radius;

    /// Opening Angle
    double _angle;

  };
  
  typedef Cone Cone_t;
}
#endif
/** @} */ // end of doxygen group 

