/**
 * \file GeoObjects.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class Trajectory
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOTRAJECTORY_H
#define BASICTOOL_GEOTRAJECTORY_H

#include "GeoVector.h"

namespace geoalgo {

  /**
     \class Trajectory
     This class represents a trajectory which is an ordered list of Point.
     It is a friend class w/ geoalgo::Point_t hence it has an access to protected functions that avoids
     dimensionality sanity checks for speed.
   */
  class Trajectory : public std::vector<geoalgo::Vector> {

  public:
    
    /// Ctor to specify # points and dimension of each point
    Trajectory(size_t npoints=0, size_t ndimension=0) : std::vector<geoalgo::Point_t>(npoints, Point_t(ndimension))
    {}

    /// Ctor using a vector of mere vector point expression
    Trajectory(const std::vector<std::vector<double> > &obj)
    {
      this->reserve(obj.size());
      for(auto const& p : obj) this->push_back(Point_t(p));
    }

    /// Ctor using a vector of point
    Trajectory(const std::vector<geoalgo::Point_t> &obj)
    {
      this->reserve(obj.size());
      for(auto const& p : obj) this->push_back(p);
    }

    /// Default dtor
    virtual ~Trajectory(){}

    /// Returns the cumulative distance along all trajectory points
    double Length() const {

      if(size()<2) return 0;

      double length = 0;
      for(size_t i=0; i<size()-1; ++i) 

	length += _Dir_(i).Length();
      
      return sqrt(length);
    }

    /// push_back overrie w/ dimensionality check 
    void push_back(const Point_t& obj) {
      compat(obj); 
      if(size() && obj == (*rbegin()))
	throw GeoAlgoException("<<push_back>> Cannot add an identical point to a trajectory!");
      std::vector<geoalgo::Point_t>::push_back(obj);
    }

    /// push_back template
    template <class T>
    void push_back(const T& obj) 
    { Point_t pt(obj); push_back(pt); }

    /// Dimensionality check function w/ Trajectory
    void compat(const Point_t& obj) const {

      if(!size()) return;
      if( (*(this->begin())).size() != obj.size() ) {

	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " size mismatch: "
	    << (*(this->begin())).size() << " != " << obj.size() << std::endl;
	throw GeoAlgoException(msg.str());
      }
    }

    /// Dimensionality check function w/ Point_t
    void compat(const Trajectory &obj) const {

      if(!size() || !(obj.size())) return;

      if( (*(this->begin())).size() != (*obj.begin()).size() ) {

	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " size mismatch: "
	    << (*(this->begin())).size() << " != " << (*obj.begin()).size() << std::endl;
	throw GeoAlgoException(msg.str());

      }
    }

    /// Returns a direction vector at a specified trajectory point
    Vector Dir(size_t i=0) const {

      if(size() < (i+2)) {
	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " length=" << size() << " is too short to find a direction @ index=" << i << std::endl;
	throw GeoAlgoException(msg.str());
      }
      return _Dir_(i);
    }

    Trajectory& operator+=(const Point_t& rhs)
    { push_back(rhs); return *this; }

  public:

    /// Streamer
#ifndef __CINT__
    friend std::ostream& operator << (std::ostream &o, Trajectory const& a)
    { o << "Trajectory with " << a.size() << " points " << std::endl;
      for(auto const& p : a )
	o << " " << p << std::endl;
      return o;
    }
#endif
    
    /// Returns a direction vector at a specified trajectory point w/o size check
    Vector _Dir_(size_t i) const {

      return ((*this)[i+1] - (*this)[i]);
      
    }
  };

  typedef Trajectory Trajectory_t;

}

#endif
/** @} */ // end of doxygen group 

