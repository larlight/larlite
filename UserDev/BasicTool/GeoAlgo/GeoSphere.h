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
	Vector_t AB(A-B);
	Vector_t AC(A-C);
	Vector_t BC(B-C);
	
	// first try simple way. If longest segment can be taken as diameter
	double lenAB = AB.Length();
	double lenAC = AC.Length();
	double lenBC = BC.Length();
	
	bool success = false;
	if ( (lenAB > lenAC) && (lenAB > lenBC) ){
	  // lenAB -> try with AB the diameter
	  // try and see if midpoint closer to point C then 1/2 of lenAB.
	  // if so midpoint origin & 1/2 lenAB is radius
	  Point_t midAB((A+B)/2.);
	  if ( midAB.Dist(C) < 0.5*lenAB ){
	    _origin = midAB;
	    _radius = 0.5*lenAB;
	    success = true;
	  }
	}
	else if (lenAC > lenBC){
	  // try with AC the diameter
	  Point_t midAC((A+C)/2.);
	  if ( midAC.Dist(B) < 0.5*lenAC ){
	    _origin = midAC;
	    _radius = 0.5*lenAC;
	    success = true;
	  }
	}
	else{
	  // try with BC the diameter
	  Point_t midBC((B+C)/2.);
	  if ( midBC.Dist(A) < 0.5*lenBC ){
	    _origin = midBC;
	    _radius = 0.5*lenBC;
	    success = true;
	  }
	}
	
	if (!success){
	  
	  
	  if ( ((AB.Dot(AC))/(AB.Length()*AC.Length()) == 1) || ((AB.Dot(AC))/(AB.Length()*AC.Length()) == -1) )
	    throw GeoAlgoException("Thre points are collinear: cannot construct a sphere!");
	  
	  // Find vectors normal to AB and AC passing through
	  // midpoints of AB and AC.
	  // Given AB the bi-sector of interest needs to lie
	  // in the plane of the triangle
	  // first find vector perpendicular to AB (1 constraint, still inifite number)
	  // this vector has to be perpendicular to the AB X AC cross-product
	  // second constraint. Then we have our direction (normalizing imposes 3rd constraint)
	  Vector_t Cross = AB.Cross(AC); // cross-product vector
	  Cross.Normalize();
	  // find X, Y, Z of the perpendicualr bi-sector to AB
	  // choose x randomly
	  double x = 1;
	  double y,z;
	  // Solve for Bisector.y and Bisector.z
	  // AB.x * Bisector.x + AB.y * Bisectory.y + AB.z * Bisector.z = 0
	  // cross.x * Bisector.x + cross.y * Bisectory.y + cross.z * Bisector.z = 0
	  double denom = (AB[1] * Cross[2]) - (Cross[1] * AB[2]);
	  if (denom == 0)
	    throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
	  z = x * ( (AB[0] * Cross[1]) - (AB[1] * Cross[0]) ) / denom;
	  if (AB[1] ==0)
	    throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
	  y = ((AB[0] * x) + (AB[2] * z))/(-AB[1]);
	  // get bi-sector direction for AB
	  Vector_t ABbisector(x,y,z);
	  ABbisector.Normalize();
	  Vector_t ABnorm = AB/AB.Length();
	  // Do the same for the AC bisector
	  denom = ( (AC[2] * Cross[1]) - (Cross[2] * AC[1]) );
	  if (denom == 0)
	    throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
	  z = x * ( (AC[1] * Cross[0]) - (AC[0] * Cross[1]) ) / denom;
	  if (AC[1] == 0)
	    throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
	  y = ((AC[0] * x) + (AC[2] * z))/(-AC[1]);
	  // get bi-sector direction for AB
	  Vector_t ACbisector(x,y,z);
	  ACbisector.Normalize();
	  Vector_t ACnorm = AC/AC.Length();
	  // Find the intersection of the bisectors - This is the circle origin
	  // Midpoints on two segments:
	  Point_t ABmidpoint = (A+B)/2.;
	  Point_t ACmidpoint = (A+C)/2.;
	  // O = ABmidpoint + ABbisector * t1 ;
	  // O = ACmidpoint + ACbisector * t2 ;
	  // 6 eqations -> solve
	  denom = (ACbisector[1]*ABbisector[0]-ABbisector[1]*ACbisector[0]);
	  if (denom == 0)
	    throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
	  double t2 = ( (ABbisector[0] * (ABmidpoint[1]-ACmidpoint[1]))
			+ (ABbisector[1] * (ACmidpoint[0]-ABmidpoint[0])) ) / denom;
	  Point_t O = ACmidpoint + ACbisector*t2;
	  _origin = O;
	  // radius is distance from O to any of the points
	  _radius = _origin.Dist(A);
	  // to debug cout all distances
	  //std::cout << "Dist OA: " << _origin.Dist(A) << "\tDist OB: " << _origin.Dist(B) << "\tDist OC: " << _origin.Dist(C) << std::endl;
	  
	}// if no success
	
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

