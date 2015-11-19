/**
 * \file LightPath.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class LightPath
 *
 * @author Rui
 */

/** \addtogroup Algorithms

    @{*/
#ifndef LIGHTPATH_H
#define LIGHTPATH_H

#include <iostream>
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include <numeric>
#include "GeoAlgo/GeoAlgo.h"
#include "LArUtil/Geometry.h"
#include <functional>
#include <algorithm>

namespace flashana{
/**
   \class LightPath
   User defined class LightPath ... these comments are used to generate
   doxygen documentation!
 */


  class LightPath{
    
  public:
    
    /// Default constructor
    LightPath();
    
    /// Default destructor
    ~LightPath(){}
    
    // Setter function
    bool TrackStart( bool a) { _start_bool =a; return _start_bool;}
    bool TrackEnd  ( bool b) { _end_bool   =b; return _end_bool;}
    double Set_Gap (double x){ _gap   =x; return _gap;}

    // Getter function
    flashana::QCluster_t FlashHypothesis(::geoalgo::Trajectory trj) const;

    // Calculation fuction
    std::vector<double> PhotonLibrary(::geoalgo::Vector pt_1,
                                      ::geoalgo::Vector pt_2,
                                      std::vector<double> pe) const;
    void QCluster(::geoalgo::Vector pt_1,
                  ::geoalgo::Vector pt_2,
                  flashana::QCluster_t& Q_cluster) const;
    
  protected:
    bool   _start_bool = true;
    bool   _end_bool   = true;
    double _gap   = 0.5;
    double _xmax, _xmin;
    double _ymax, _ymin;
    double _zmax, _zmin;
  };
} 

#endif
/** @} */ // end of doxygen group 

