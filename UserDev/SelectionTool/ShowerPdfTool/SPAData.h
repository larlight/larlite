/**
 * \file SPAData.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAData and 
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef LARLITE_SPADATA_H
#define LARLITE_SPADATA_H

#include <iostream>
#include <TObject.h>
#include "GeoObjects.h"
#include "SPTEnv.h"
namespace sptool {

  /**
     \class SPAShower
     @brief Data holder class to contain Shower data. This is contained within sptool::SPAData instance.
     This class instance is created per shower, and stored within sptool::SPAData.
     SPAlgoX and SPFilterX will use sptool::SPAData instance for evaluation.
  */
  class SPAShower : public TObject {
  public:

    /// default ctor
    SPAShower();

    /// default dtor
    virtual ~SPAShower(){}

    /// Reset function
    void Reset();

    /// Shower start 3D point
    ::geoalgo::Point _start;

    /// Shower 3D direction from the start point
    ::geoalgo::Point _dir;

    /// Radius of shower, either read from data product or computed
    double _radius;

    /// Opening angle of a shower cone, either read from data product or computed
    double _cone_angle;

    /// Shower total energy
    double _energy;

    /// dE/dX @ shower start
    double _dedx;

    ClassDef(SPAShower,1)
  };

  /**
     \class SPAData
     @brief Data holder class to contain full information, to be used by SPAlgoX and SPFilterX
     Contains a vector of showers, tracks, and vertex points. SPAManager will change the SPAShower
     array contents for each possible combination of showers while tracks and vertex points are
     left unchanged. This class instance is provided to SPAlgoX and SPFilterX.
  */
  class SPAData : public TObject {
    
  public:
    
    /// Default constructor
    SPAData();
    
    /// Default destructor
    virtual ~SPAData(){}
    
    /// Reset function
    void Reset();

    /// Showers
    std::vector<sptool::SPAShower>   _showers;
    /// Tracks
    std::vector<geoalgo::Trajectory> _tracks;
    /// Candidate event vertecies
    std::vector<geoalgo::Point>      _vtxs;

    ClassDef(SPAData,1)
  };
}
#endif
/** @} */ // end of doxygen group 

