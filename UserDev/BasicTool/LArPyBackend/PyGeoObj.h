/**
 * \file PyGeoObj.h
 *
 * \ingroup LArPyBackend
 * 
 * \brief Class def header for a class PyGeoObj
 *
 * @author kazuhiro
 */

/** \addtogroup LArPyBackend

    @{*/
#ifndef BASICTOOL_PYGEOOBJ_H
#define BASICTOOL_PYGEOOBJ_H

#include <iostream>
struct _object;
typedef _object PyObject;

#ifndef __CINT__
#include "Python.h"
#endif

#include <map>
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoTrajectory.h"
#include "GeoAlgo/GeoLineSegment.h"
#include "GeoAlgo/GeoAABox.h"

namespace larpy {
  /**
     \class PyGeoObj
  */
  class PyGeoObj {
    
  public:
    
    /// Default constructor
    PyGeoObj() {}
    
    /// Default destructor
    virtual ~PyGeoObj(){};

    PyObject* Convert(const larlite::geoalgo::Point_t& pt        ) const;
    //PyObject* Convert(const std::string& col              ) const;
    PyObject* Convert(const larlite::geoalgo::LineSegment_t& seg ) const;
    PyObject* Convert(const larlite::geoalgo::HalfLine_t& seg    ) const;
    PyObject* Convert(const larlite::geoalgo::Trajectory_t& trj  ) const;
    PyObject* Convert(const larlite::geoalgo::AABox_t& box       ) const;
		      
    PyObject* Convert(const std::map<larlite::geoalgo::Point_t,std::string>& label) const;
  };
  
}
#endif
/** @} */ // end of doxygen group 

