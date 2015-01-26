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
namespace geoalgo {
  /**
     \class Cone
     @brief Representation of a 3D semi-infinite line.
     Defines a 3D cone with the following properties:                                           \n
     Start point (or vertex), Direction, Height (or Length), Radius, opening angle              \n
     When 2 of Height, Radius, opening angle are defined the third is automatically set         
  */
  class Cone {
    
  public:
    
    /// Default constructor
    Cone() 
      : _start(3)
      , _dir(3)
    {Normalize();}

    /// Alternative ctor (1)
    Cone(const double x,    const double y,    const double z,
	 const double dirx, const double diry, const double dirz,
	 const double height, const double radius)
      : _start (x,    y,    z   )
      , _dir   (dirx, diry, dirz)
      {
	_height = height; 
	_radius = radius;
	_angle = atan(_radius/_height);
	Normalize();
      }

    /// Altenartive ctor (2)
  Cone(const Point_t& start, const Vector_t& dir,
       const double height, const double radius)
      : _start ( start )
      , _dir   ( dir   )
    { 
      if(start.size()!=3 || dir.size()!=3)
	throw GeoAlgoException("Cone ctor accepts only 3D Point!");
      _height = height;
      _radius = radius;
      _angle  = atan(_radius/_height);
      Normalize();
    }

    /// Alternative ctor using template (3)
    template <class T, class U> Cone(const T& start, const U& dir)
      : Cone(Point_t(start), Vector_t(dir))
    {}

    /// Default destructor
    virtual ~Cone(){};

    /// Start getter
    const Point_t& Start() const { return _start; }

    /// Direction getter
    const Vector_t& Dir() const { return _dir; }

    /// Height getter
    const double Height() const { return _height; }

    /// Length getter
    const double Radius() const { return _radius; }

    /// Angle getter
    const double Angle() const { return _angle; }
    
    /// Start setter
    void Start(const double x, const double y, const double z)
    { _start[0] = x; _start[1] = y; _start[2] = z; }

    /// Dir setter
    void Dir(const double x, const double y, const double z)
    { _dir[0] = x; _dir[1] = y; _dir[2] = z; Normalize(); }

    /// Height setter
    void Height(const double h) { _height = h; }

    /// Length setter
    void Radius(const double r) { _radius = r; }

    /// Angle setter
    void Angle(const double a) { _angle = a; }

    /// Start setter template
    template<class T>
    void Start(const T& pos)
    { 
      _start = Point_t(pos); 
      if(_start.size()!=3) throw GeoAlgoException("<<Start>> Only 3 dimensional start point allowed!"); 
    }
    
    /// Dir setter template
    template<class T>
    void Dir(const T& dir)
    { 
      _dir = Vector_t(dir);
      if(_dir.size()!=3) throw GeoAlgoException("<<Start>> Only 3 dimensional start point allowed!"); 
      Normalize();
    }

  protected:

    /// Normalize direction
    void Normalize()
    {
      auto l = _dir.Length();
      if(!l)
	throw GeoAlgoException("<<Normalize>> cannot normalize 0-length direction vector!");

      // inf check commented out till compatible solution found... --kazu
      //if(isnan(l))
      //throw GeoAlgoException("<<Normalize>> cannot normalize inf-length direction vector!");
      _dir /= l;
    }

    /// Beginning of the cone
    Point_t  _start;

    /// Direction of the cone
    Vector_t _dir;

    /// Helight (length) of the cone
    double _height;
    
    /// Radius of the cone at the base
    double _radius;

    /// Opening Angle
    double _angle;

  };
  
  typedef Cone Cone_t;
}
#endif
/** @} */ // end of doxygen group 

