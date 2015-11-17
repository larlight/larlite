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
#include <numeric>
#include "Analysis/ana_base.h"
#include "DataFormat/track.h"
#include "DataFormat/ophit.h"
#include "DataFormat/opflash.h"
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
    FlashHypo(){}
    
    /// Default destructor
    ~FlashHypo(){}
    
    // Setter function
    bool TrackStart( bool a) { _start_bool =a; return _start_bool;}
    bool TrackEnd  ( bool b) { _end_bool   =b; return _end_bool;}
    double Set_Gap (double x){ _gap   =x; return _gap;}
    
    // Getter function
    std::vector<std::vector<double>> FlashHypothesis(::geoalgo::Trajectory trj) const;
    
    // Calculation fuction
    std::vector<double> PhotonLibrary(::geoalgo::Vector pt_1, ::geoalgo::Vector pt_2, std::vector<double> pe) const;
    std::vector<std::vector<double>> QCluster(::geoalgo::Vector pt_1, ::geoalgo::Vector pt_2, std::vector<std::vector<double>> Q_cluster) const;
    
  protected:
    bool   _start_bool = true;
    bool   _end_bool   = true;
    double _gap   = 0.5;
  };
}
#endif
/** @} */ // end of doxygen group 

