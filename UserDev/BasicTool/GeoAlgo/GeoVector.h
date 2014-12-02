/**
 * \file GeoObjects.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class Point and Vector
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOPOINT_H
#define BASICTOOL_GEOPOINT_H

#include "GeoAlgoConstants.h"
#include "GeoAlgoException.h"
#include <TVector3.h>
#include <TLorentzVector.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
namespace geoalgo {

  // Forward declaration (don't worry)
  class Trajectory;
  class LineSegment;
  class HalfLine;
  class PointToLineDist;

  /**
     \class Vector
     This class represents an n-dimensional vector
  */
  class Vector : public std::vector<double> {
    friend class Trajectory;
    friend class HalfLine;
    friend class LineSegment;
    friend class PointToLineDist;

  public:
    /// Default ctor
    Vector() : std::vector<double>()
    {}

    /// Ctor to instantiate with invalid value
    Vector(size_t n) : std::vector<double>(n,kINVALID_DOUBLE)
    {}

    /// Default ctor w/ a bare std::vector<double>
    Vector(const std::vector<double> &obj) : std::vector<double>(obj)
    {}

    /// ctor w/ x & y
    Vector(const double x, const double y) : Vector(2)
    { (*this)[0] = x; (*this)[1] = y; }

    /// ctor w/ x, y & z
    Vector(const double x, const double y, const double z) : Vector(3)
    { (*this)[0] = x; (*this)[1] = y; (*this)[2] = z; }

    /// ctor w/ TVector3
    Vector(const TVector3 &pt) : Vector(3)
    { (*this)[0] = pt[0]; (*this)[1] = pt[1]; (*this)[2] = pt[2]; }

    /// ctor w/ TLorentzVector
    Vector(const TLorentzVector &pt) : Vector(3)
    { (*this)[0] = pt[0]; (*this)[1] = pt[1]; (*this)[2] = pt[2]; }
    
    /// Default dtor
    virtual ~Vector(){}

    /// Check if point is valid
    bool IsValid() const {

      for (auto const &v : (*this)){
	// if any point is different from kINVALID_DOUBLE
	// then the point is valid
	if (v != kINVALID_DOUBLE)
	  return true;
      }

      return false;
    }

    /// Compute the squared length of the vector
    double SqLength() const {
      double res=0;
      for(auto const &v : (*this)) res += v*v;
      return res;
    }

    /// Compute the length of the vector
    double Length() const
    { return sqrt(SqLength()); }

    /// Compute the squared distance to another vector
    double SqDist(const Vector &obj) const {
      compat(obj);
      return _SqDist_(obj);
    }

    /// Compute the distance to another vector
    double Dist(const Vector& obj) const 
    { return sqrt(SqDist(obj)); }

    /// Compute a dot product of two vectors
    double Dot(const Vector &obj) const {
      compat(obj);
      return _Dot_(obj);
    }
    
    /// Compute a cross product of two vectors
    Vector Cross(const Vector &obj) const {

      if(size()!=3 || obj.size()!=3)
	
	throw GeoAlgoException("<<Cross>> only possible for 3-dimensional vectors!");

      return _Cross_(obj);
    }

    /// Compute an opening angle w.r.t. the given vector
    Vector Angle(const Vector &obj) const {
      compat(obj);
      if(size()!=2 && size()!=3)
	throw GeoAlgoException("<<Angle>> only possible for 2 or 3-dimensional vectors!");
      return _Angle_(obj);
    }
      
    /// Dimensional check for a compatibility
    void compat(const Vector& obj) const {
      if(size() != obj.size()) {
	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>" 
	    << " size mismatch: "
	    << size() << " != " << obj.size()
	    << std::endl;
	throw GeoAlgoException(msg.str());
      }
    }

  public:

    Vector& operator+=(const Vector& rhs) { 
      for(size_t i=0; i<size(); ++i) (*this)[i] += rhs[i];
      return *this;
    }

    Vector& operator-=(const Vector& rhs) {
      for(size_t i=0; i<size(); ++i) (*this)[i] -= rhs[i];
      return *this;
    }

    Vector& operator*=(const double rhs) {
      for(auto &v : *this) v *= rhs;
      return *this;
    }

    Vector& operator/=(const double rhs) {
      for(auto &v : *this) v /= rhs;
      return *this;
    }

    inline Vector operator+(const Vector& rhs) const
    { 
      Vector res((*this));
      res+=rhs;
      return res;
    }

    inline Vector operator-(const Vector& rhs) const
    { 
      Vector res((*this));
      res-=rhs;
      return res;
    }

    inline double operator*(const Vector& rhs) const
    { 
      double res=0;
      for(size_t i=0; i<size(); ++i) res += (*this)[i] * rhs[i];
      return res;
    }

    inline Vector operator*(const double& rhs) const
    {
      Vector res((*this));
      res *= rhs;
      return res;
    }

    inline Vector operator/(const double& rhs) const
    {
      Vector res((*this));
      res /= rhs;
      return res;
    }

    /// Streamer
    #ifndef __CINT__
    friend std::ostream& operator << (std::ostream &o, ::geoalgo::Vector const& a)
    { o << "Vector ("; for(auto const& v : a) o << v << " ";
      o << ")";
      return o;
    }
    #endif

  protected:

    /// Compute the squared-distance to another vector w/o dimension check
    double _SqDist_(const Vector& obj) const
    {
      double dist = 0;
      for(size_t i=0; i<size(); ++i) dist += ((*this)[i] - obj[i]) * ((*this)[i] - obj[i]);
      return dist;
    }
    /// Compute the distance to another vector w/o dimension check
    double _Dist_(const Vector& obj) const
    { return sqrt(_SqDist_(obj)); }

    /// Compute a dot product w/o dimention check.
    double _Dot_(const Vector& obj) const
    { return (*this) * obj; }

    /// Compute a cross product w/o dimension check.
    Vector _Cross_(const Vector& obj) const
    {
      Vector res(3);
      res[0] = (*this)[1] * obj[2] - obj[1] * (*this)[2];
      res[1] = (*this)[2] * obj[0] - obj[2] * (*this)[0];
      res[2] = (*this)[0] * obj[1] - obj[0] * (*this)[1];
      return res;
    }    

    /// Compute the angle in degrees between 2 vectors w/o dimension check.
    double _Angle_(const Vector& obj) const
    { return acos( _Dot_(obj) / Length() / obj.Length() ) / kPI * 180.; }

  };

  /// Point has same feature as Vector
  typedef Vector Vector_t;
  typedef Vector Point_t;


}

#endif
/** @} */ // end of doxygen group 

