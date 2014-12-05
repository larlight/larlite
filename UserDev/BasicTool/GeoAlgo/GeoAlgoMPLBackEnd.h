/**
 * \file GeoAlgoMPLBackEnd.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoAlgoMPLBackEnd
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOALGOMPLBACKEND_H
#define BASICTOOL_GEOALGOMPLBACKEND_H

#include <iostream>
#include "GeoTrajectory.h"
#include "GeoAABox.h"
#include "GeoLineSegment.h"
#include "GeoAlgoException.h"
struct _object;
typedef _object PyObject;
namespace mpl {

  typedef std::vector<std::vector<double> > XYZ_v;
  typedef std::vector<double> X_v;
  typedef std::vector<double> Y_v;
  typedef std::vector<double> Z_v;
  /**
     \class GeoAlgoMPLBackEnd
     User defined class GeoAlgoMPLBackEnd ... these comments are used to generate
     doxygen documentation!
  */
  class GeoAlgoMPLBackEnd{
    
  public:
    
    /// Default constructor
    GeoAlgoMPLBackEnd(){};
    
    /// Default destructor
    virtual ~GeoAlgoMPLBackEnd(){};

    XYZ_v Convert(const geoalgo::Trajectory_t &data);
    
    XYZ_v Convert(const geoalgo::AABox_t &data);

    XYZ_v Convert(const geoalgo::LineSegment_t &data);
    
  };
}

#endif
/** @} */ // end of doxygen group 

