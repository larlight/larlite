/**
 * \file SegmentPoCA.hh
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoAlgo
 * \brief This class takes in two (point + direction) vectors and finds the
 * intersection in 3D space (or, the closest thing to an intersection
 * since two 3D lines frequently don't intersect).
 * see: http://paulbourke.net/geometry/pointlineplane/
 *
 * @author David Caratelli
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_SEGMENTPOCA_H
#define BASICTOOL_SEGMENTPOCA_H

#include "GeoVector.h"
#include "GeoTrajectory.h"

namespace geoalgo {
  /**
     \class SegmentPoCA
     User defined class SegmentPoCA ... these comments are used to generate
     doxygen documentation!
  */
  
  class SegmentPoCA {
    
  public:
    
    /// Default constructor
    SegmentPoCA(){_epsilon=0.000001;}
    
    /// Default destructor
    virtual ~SegmentPoCA(){}


    double ClosestApproachToTrajectory(const Trajectory_t& traj,
				       const Point_t& s2,
				       const Point_t& e2,
				       Point_t &c1,
				       Point_t &c2) const;

    double ClosestApproach(const Point_t &s1,
			   const Point_t &e1,
			   const Point_t &s2,
			   const Point_t &e2,
			   Point_t &c1,
			   Point_t &c2) const
    { 
      compat(s1); compat(e1); compat(s2); compat(e2); 
      return _ClosestApproach_(s1,e1,s2,e2,c1,c2);
    }
    
    double Clamp(const double n, const double min, const double max) const;

    void TestPoCA(const Point_t &s1, 
		  const Point_t &e1,
		  const Point_t &s2, 
		  const Point_t &e2) const;

    void compat(const Point_t &obj) const
    {
      if(obj.size()!=3) {
	std::ostringstream msg;
	msg << "<<" << __FUNCTION__ << ">>"
	    << " cannot handle 3 dimensional object!" << std::endl;
	throw GeoAlgoException(msg.str());
      }
    }

    void compat(const Trajectory_t &obj) const
    {
      if(obj.size()) compat((*obj.begin()));
    }
    
  protected:

    double _ClosestApproach_(const Point_t &s1,
			     const Point_t &e1,
			     const Point_t &s2,
			     const Point_t &e2,
			     Point_t &c1,
			     Point_t &c2) const;

  private:
    
    double _epsilon;

  };

}

#endif
/** @} */ // end of doxygen group 

