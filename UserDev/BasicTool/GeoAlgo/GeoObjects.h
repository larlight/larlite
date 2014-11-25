/**
 * \file GeoObjects.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoObjects
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef LARLITE_GEOOBJECTS_H
#define LARLITE_GEOOBJECTS_H

#include "GeoAlgoException.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
namespace geoalgo {

  // Forward declaration (don't worry)
  class Trajectory;

  /**
     \class Point
     This class represents an n-dimensional point
  */
  class Point : public std::vector<double> {
    friend class Trajectory;
  public:

    /// Default ctor to instantiate with invalid value
    Point(size_t n=0) : std::vector<double>(n,std::numeric_limits<double>::max())
    {}

    /// Default ctor w/ a bare std::vector<double>
    Point(const std::vector<double> &obj) : std::vector<double>(obj)
    {}
    
    /// Default dtor
    virtual ~Point(){}

    /// Compute squared distance between 2 points with dimension check (no sqrt particularly useful for speed)
    double SquaredDistance(const Point& obj) const {     
      check(obj);
      return _SquaredDistance_(obj);      
    }

    /// Compute distance between between 2 points with dimension check
    double Distance(const Point& obj) const
    { return sqrt(SquaredDistance(obj)); }

    /// Compute a dot product of two points (each representing a vector from the origin)
    double DotProduct(const Point &obj) const {
      
      check(obj);
      double res=0;
      for(size_t i=0; i<size(); ++i)
	res += (*this)[i] * obj[i];
      return res;
    }

  protected:

    /// Compute a squared distance w/o dimensional check. Available to friend classes
    double _SquaredDistance_(const Point& obj) const {
      double dist2=0;
      for(size_t i=0; i<size(); ++i) dist2 += pow(obj[i] - (*this)[i],2);
      return dist2;
    }

    /// Compute a distance w/o dimensional check. Available to friend classes
    double _Distance_(const Point& obj) const
    { return sqrt(_SquaredDistance_(obj)); }

  private:

    /// Dimensional check for a compatibility
    void check(const Point& obj) const {
      if(size() != obj.size()) {
	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>" 
	    << " size mismatch: "
	    << size() << " != " << obj.size()
	    << std::endl;
	throw GeoAlgoException(msg.str());
      }
    }
    
  };

  /**
     \class Trajectory
     This class represents a trajectory which is an ordered list of Point.
     It is a friend class w/ geoalgo::Point hence it has an access to protected functions that avoids
     dimensionality sanity checks for speed.
   */
  class Trajectory : public std::vector<geoalgo::Point> {

  public:
    
    /// Ctor to specify # points and dimension of each point
    Trajectory(size_t npoints, size_t ndimension) : std::vector<geoalgo::Point>(npoints, Point(ndimension))
    {}

    /// Ctor using a vector of point
    Trajectory(const std::vector<geoalgo::Point> &obj){
      if(obj.size()) {
	size_t dim=(*obj.begin()).size();
	for(auto const& p : obj) {
	  if(p.size() != dim) {
	    std::ostringstream msg;
	    msg << "<<" << __FUNCTION__ << ">>"
		<< " Input vector of Point contains point of different dimension!" << std::endl;
	    throw GeoAlgoException(msg.str());
	  }
	}
	this->reserve(obj.size());
	for(auto const& p : obj) this->push_back(p);
      }
    }

    /// Ctor using a vector of mere vector point expression
    Trajectory(const std::vector<std::vector<double> > &obj)
    {
      if(obj.size()) {
	size_t dim=(*obj.begin()).size();
	for(auto const& p : obj) {
	  if(p.size() != dim) {
	    std::ostringstream msg;
	    msg << "<<" << __FUNCTION__ << ">>"
		<< " Input vector of Point contains point of different dimension!" << std::endl;
	    throw GeoAlgoException(msg.str());
	  }
	}
	this->reserve(obj.size());
	for(auto const& p : obj) this->push_back(Point(p));
      }
    }

    /// Default dtor
    virtual ~Trajectory(){}

    /// Returns the cumulative distance along all trajectory points
    double Length() const {

      if(size()<2) return 0;

      double length = 0;
      for(size_t i=0; i<size()-1; ++i) 

	length += (*this)[i]._Distance_((*this)[i+1]);
      
      return length;
    }

    /// push_back overrie w/ dimensionality check 
    void push_back(const Point& obj) {
      check(obj); std::vector<geoalgo::Point>::push_back(obj);
    }

  private:

    /// Dimensionality check function w/ Trajectory
    void check(const Point& obj) const {

      if(!size()) return;
      if( (*(this->begin())).size() != obj.size() ) {

	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " size mismatch: "
	    << (*(this->begin())).size() << " != " << obj.size() << std::endl;
	throw GeoAlgoException(msg.str());
      }
    }

    /// Dimensionality check function w/ Point
    void check(const Trajectory &obj) const {

      if(!size() || !(obj.size())) return;

      if( (*(this->begin())).size() != (*obj.begin()).size() ) {

	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " size mismatch: "
	    << (*(this->begin())).size() << " != " << (*obj.begin()).size() << std::endl;
	throw GeoAlgoException(msg.str());

      }
    }

  };

}

#endif
/** @} */ // end of doxygen group 

