/**
 * \file LiteData2Geo.h
 *
 * \ingroup GeoAlgoBackend
 * 
 * \brief Class def header for a class LiteData2Geo
 *
 * @author kazuhiro
 */

/** \addtogroup GeoAlgoBackend

    @{*/
#ifndef LARLITE_LITEDATA2GEO_H
#define LARLITE_LITEDATA2GEO_H

#include <iostream>

#include "DataFormat/DataFormat-TypeDef.h"
#include "GeoAlgo/GeoAlgo-TypeDef.h"

namespace larlite {
  /**
     \class LiteData2Geo
     User defined class LiteData2Geo ... these comments are used to generate
     doxygen documentation!
  */
  class LiteData2Geo{
    
  public:
    
    /// Default constructor
    LiteData2Geo(){}
    
    /// Default destructor
    virtual ~LiteData2Geo(){}

    ::geoalgo::Trajectory_t GeoObj(const mctrack& trk) const;
    ::geoalgo::HalfLine_t GeoObj(const mcshower& shw) const;

    std::vector<geoalgo::HalfLine_t> GeoObj(const std::vector<larlite::mcshower>& shw_v) const;
    std::vector<geoalgo::Trajectory_t> GeoObj(const std::vector<larlite::mctrack>& trk_v) const;
    

  };
}

#endif
/** @} */ // end of doxygen group 

