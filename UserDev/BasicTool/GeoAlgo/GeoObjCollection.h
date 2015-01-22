/**
 * \file GeoObjCollection.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoObjCollection
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOOBJCOLLECTION_H
#define BASICTOOL_GEOOBJCOLLECTION_H

#include <iostream>
#include "GeoTrajectory.h"
#include "GeoAABox.h"
#include "GeoLineSegment.h"
#include "GeoAlgoException.h"
#include <map>
namespace geoalgo {

  /**
     \class GeoObjCollection
  */
  class GeoObjCollection {
    
  public:
    
    /// Default constructor
    GeoObjCollection(){ Clear(); }
    
    /// Default destructor
    virtual ~GeoObjCollection(){}

    void Clear();
    
    void Add(const Point_t& pt, std::string label="", std::string c="");
	     
    void Add(const AABox_t& box, std::string label="", std::string c="");
	     
    void Add(const LineSegment_t& seg, std::string label="", std::string c="");
	     
    void Add(const Trajectory_t& trj, std::string label="", std::string c="");

    const std::vector< geoalgo::Point_t >& Point() const
    { return _pt_v; }

    const std::vector< geoalgo::AABox_t >& AABox() const
    { return _box_v; }

    const std::vector< geoalgo::LineSegment_t >& LineSegment() const
    { return _seg_v; }

    const std::vector< geoalgo::Trajectory_t >& Trajectory() const
    { return _trj_v; }

    const std::map<geoalgo::Point_t,std::string>& Labels() const 
    { return _labels;}

  protected:

    const Point_t& _Point_(size_t i) const
    { return _pt_v[i]; }

    const AABox_t& _AABox_(size_t i) const
    { return _box_v[i]; }

    const LineSegment_t& _LineSegment_(size_t i) const
    { return _seg_v[i]; }

    const Trajectory_t& _Trajectory_(size_t i) const
    { return _trj_v[i]; }

    void _AddLabel_(const Point_t& pt,
		    std::string label);

    std::vector< geoalgo::Point_t       > _pt_v;
    std::vector< geoalgo::AABox_t       > _box_v;
    std::vector< geoalgo::LineSegment_t > _seg_v;
    std::vector< geoalgo::Trajectory_t  > _trj_v;
    std::map<geoalgo::Point_t,std::string > _labels;

  };
  
}
#endif
/** @} */ // end of doxygen group 

