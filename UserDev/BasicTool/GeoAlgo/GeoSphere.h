/**
 * \file GeoSphere.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class HalfLine
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo
    
    @{*/
#ifndef BASICTOOL_GEOSPHERE_H
#define BASICTOOL_GEOSPHERE_H

#include "GeoVector.h"

namespace geoalgo {
  /**
     \class Spehere
     @brief Representation of a 3D sphere
     Defines a 3D Sphere having an origin (Point_t) and a radius (double) 
  */
  class Sphere {
    
  public:
    
    /// Default constructor
  Sphere() 
    : _origin(0,0,0)
      {_radius = 0.;}
    
    /// Alternative ctor (1)
  Sphere(const double x,    const double y,    const double z,
	 const double r)
    : _origin (x, y, z)
      {_radius = r; }
    
    /// Altenartive ctor (2)
  Sphere(const Point_t& origin, const double r)
    : _origin ( origin )
    { 
      if( _origin.size()!=3 )
	throw GeoAlgoException("Sphere ctor accepts only 3D origin!");
      _radius = r;
    }
    
    /// Alternative ctor (3) - 1 Point
  Sphere(const Point_t& origin)
    : _origin ( origin )
    {
      if( _origin.size()!=3 )
	throw GeoAlgoException("Sphere ctor accepts only 3D origin!");
      _radius = 0;
    }
    
    /// Alternative ctor (4) - 2 Points
    Sphere(const Point_t& pt1, const Point_t& pt2)
      {
	if( pt1.size()!=3 || pt2.size()!=3 )
	  throw GeoAlgoException("Sphere ctor accepts only 3D points!");
	_origin = (pt1+pt2)/2.;
	_radius = pt1.Dist(pt2)/2.;
      }
    
    /// Alternative ctor (5) - 3 Points
    Sphere(const Point_t& A, const Point_t& B, const Point_t& C)
      {
	// Dimensionality check
	if( A.size()!=3 || B.size()!=3 || C.size()!=3 )
	  throw GeoAlgoException("Sphere ctor accepts only 3D points!");
	
	// any three points are co-planar
	// (if collinear no sphere passing  through all 3)
	// These 3 points make a triangle
	// find the perpendicular bi-sectors to the segments
	// making up the triangle. They will intersect
	// at the sphere's origin
	
	// check if collinear. If so return exception
	Vector_t AB(B-A);
	Vector_t AC(C-A);
	Vector_t BC(C-B);

	double dABAB = AB.Dot(AB);
	double dACAC = AC.Dot(AC);
	double dABAC = AB.Dot(AC);

	double d = dABAB * dACAC - dABAC * dABAC;
	double s,t;
	
	// if d == 0 they lie on one line
	if (d == 0){
	  std::cout << "d is 0!" << std::endl;
	  double lenAB = AB.Length();
	  double lenAC = AC.Length();
	  double lenBC = BC.Length();
	  // which segment is longest?
	  if ( (lenAB > lenAC) && (lenAB > lenBC) ){
	    _origin = (A+B)/2.;
	    _radius = _origin.Dist(A);
	  }
	  else if( lenAC > lenBC ){
	    _origin = (A+C)/2.;
	    _radius = _origin.Dist(A);
	  }
	  else{
	    _origin = (B+C)/2;
	    _radius = _origin.Dist(B);
	  }
	}// if d == 0
	    
	else{
	  std::cout << "d not == 0!" << std::endl;
	  s = 0.5 * ( dABAB * dACAC - dACAC * dABAC ) / d;
	  t = 0.5 * ( dACAC * dABAB - dABAB * dABAC ) / d;
	  
	  // if s & t both > 0 && 1-s-t also > 0 then P = A + s*(B-A) + t*(C-A) is the origin
	  if ( (s > 0) && (t > 0) && ((1-s-t) > 0) ){
	    _origin = A+(B-A)*s+(C-A)*t;
	    _radius = _origin.Dist(A);
	  }

	  // otherwise only one will be negative. The side it belongs on will be
	  // the longest side and will determine the side to take as diameter
	  else if (s <= 0){
	    // side AB is the one
	    _origin = (A+C)/2.;
	    _radius = _origin.Dist(A);
	  }
	  else if (t <= 0){
	    // AC is the side
	    _origin = (A+B)/2.;
	    _radius = _origin.Dist(A);
	  }
	  else{
	    _origin = (B+C)/2;
	    _radius = _origin.Dist(B);
	  }
	}// else (if d not equal to 0)

      }




    //  Alternative ctor (5) - 4 Points
    //  http://steve.hollasch.net/cgindex/geometry/sphere4pts.html (TOP)
    Sphere(const Point_t& A, const Point_t& B, const Point_t& C, const Point_t& D){

      // get sphere from 3 points (A,B,C)
      Vector_t AB(A-B);
      Vector_t AC(A-C);
      Sphere S(A,B,C);
      Point_t P = S.Origin();
      // get the direction perpendicular to plane in which ABC lie (simply cross product of any two segments)
      Vector_t d = AB.Cross(AC);
      d.Normalize();
      // Find E: point on circle ABC that lies on plane passing through direction d and point D
      // to do this from P (origin of circle) go in the direction of the PD vector a length = to circle radius
      Vector_t PD(D-P);
      PD.Normalize();
      Vector_t PDcrossd = PD.Cross(d);
      // find vector perpendicular to both d and PDcrossd
      double denom = (d[1] * PDcrossd[2]) - (PDcrossd[1] * d[2]);
      if (denom == 0)
	throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
      double x=1;
      double y,z;
      z = x * ( (d[0] * PDcrossd[1]) - (d[1] * PDcrossd[0]) ) / denom;
      if (d[1] ==0)
	throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
      y = ((d[0] * x) + (d[2] * z))/(-d[1]);
      // get bi-sector direction for AB
      Vector_t EP(x,y,z);
      EP.Normalize();
      Point_t E(P+EP*S.Radius());
      // The circle origin will be the point on the line d passing through P that is half-way through E and D
      // this is the intersection of the ED bisector and the line passing through P with direction d
      // Midpoint between E and D is M:
      Point_t M((E+D)/2.);
      // direction of ED line:
      Vector_t ED(E-D);
      ED.Normalize();
      // bi-sector direction trhough M that intersects OP is perpendicular to ED and PDcrossd
      denom = (ED[1] * PDcrossd[2]) - (PDcrossd[1] * ED[2]);
      if (denom == 0)
	throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
      z = x * ( (ED[0] * PDcrossd[1]) - (ED[1] * PDcrossd[0]) ) / denom;
      if (ED[1] ==0)
	throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
      y = ((ED[0] * x) + (ED[2] * z))/(-ED[1]);
      // get bi-sector direction for AB
      Vector_t EDbisector(x,y,z);
      EDbisector.Normalize();
      // now find intersection between line passing through P with direction d
      // and line passing through M with direction EDbisector
      denom = (EDbisector[1]*d[0]-d[1]*EDbisector[0]);
      if (denom == 0)
	throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
      double t2 = ( (d[0] * (P[1]-M[1]))
		    + (d[1] * (M[0]-P[0])) ) / denom;
      Point_t O = M + EDbisector*t2;
      _origin = O;
      // radius is distance to any of the 4 points
      _radius = _origin.Dist(A);
      //cout distances to check
      //std::cout << "dist to A: " << _origin.Dist(A) << "\tB: " << _origin.Dist(B) << "\tC: " << _origin.Dist(C) << "\tD: " << _origin.Dist(D) << std::endl;
    }


    // Alternative ctor (6) - Set of points
    Sphere(const std::vector<Point_t>& pts){

      if (pts.size() > 4)
	throw GeoAlgoException("Cannot call Sphere constructor with more than 4 points. Something went wront");
      if (pts.size() == 0){
	_origin = Point_t(0,0,0);
	_radius = 0.;
      }
      if (pts.size() == 1){
	Sphere tmp(pts[0]);
	_origin = tmp.Origin();
	_radius = tmp.Radius();
      }
      if (pts.size() == 2){
	Sphere tmp(pts[0],pts[1]);
	_origin = tmp.Origin();
	_radius = tmp.Radius();
      }
      if (pts.size() == 3){
	Sphere tmp(pts[0],pts[1],pts[2]);
	_origin = tmp.Origin();
	_radius = tmp.Radius();
      }
      
      if (pts.size() == 4){
	Sphere tmp(pts[0],pts[1],pts[2],pts[3]);
	_origin = tmp.Origin();
	_radius = tmp.Radius();
      }
    }

    /// Alternative ctor using template (3)
    template <class T, class U> Sphere(const T& origin, const U& r)
      : Sphere(Point_t(origin), double(r))
    {}

    /// Default destructor
    virtual ~Sphere(){};

    /// Start getter
    const Point_t& Origin() const { return _origin; }

    /// Direction getter
    const double Radius() const { return _radius; }
    
    /// Start setter
    void Origin(const double x, const double y, const double z)
    { _origin[0] = x; _origin[1] = y; _origin[2] = z; }

    /// Radius setter
    void Radius(const double r)
    { _radius = r; }

    /// Start setter template
    template<class T>
    void Origin(const T& origin)
    { 
      _origin = Point_t(origin); 
      if(_origin.size()!=3) throw GeoAlgoException("<<Start>> Only 3 dimensional start point allowed! for sphere"); 
    }
    
    /// Dir setter template
    template<class T>
    void Radius(const T& r)
    { 
      _radius = double(r);
    }

    /// Contain Function (bool return)
    bool Contain(Point_t p){
      if(p.size()!=3) 
	throw GeoAlgoException("Only 3D points allowed for sphere");
      if (p.Dist(_origin) < _radius) return true;
      return false;
    }

  protected:

    /// Origin of Sphere
    Point_t  _origin;

    /// Radius of Sphere
    double _radius;

  };
  
  typedef Sphere Sphere_t;
}
#endif
/** @} */ // end of doxygen group 

