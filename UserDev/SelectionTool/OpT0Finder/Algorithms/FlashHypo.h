/**
 * \file FlashHypo.h
 *
 * \ingroup FlashHypo
 * 
 * \brief Class def header for a class FlashHypo
 *
 * @author Rui
 */

/** \addtogroup FlashHypo

    @{*/
#ifndef FLASHHYPO_H
#define FLASHHYPO_H

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
     \class FlashHypo
     User defined class FlashHypo ... these comments are used to generate
     doxygen documentation!
  */
  class FlashHypo{
    
  public:
    
    /// Default constructor
    FlashHypo();
    
    /// Default destructor
    ~FlashHypo(){}
    
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

