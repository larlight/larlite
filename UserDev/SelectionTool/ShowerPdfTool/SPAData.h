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
#include "GeoHalfLine.h"
#include "GeoTrajectory.h"
#include "SPTEnv.h"
#include "SPAException.h"
namespace sptool {

  /**
     \class SPAShower
     @brief Data holder class to contain Shower data. This is contained within sptool::SPAData instance.
     This class instance is created per shower, and stored within sptool::SPAData.
     SPAlgoX and SPFilterX will use sptool::SPAData instance for evaluation.
  */
  class SPAShower : public TObject
		  , public ::geoalgo::HalfLine {

  public:

    /// default ctor
    SPAShower();

    /// default dtor
    virtual ~SPAShower(){}

    /// alternative ctor
    template <class T, class U>
    SPAShower(const T& pos, const U& dir) : HalfLine(pos,dir)
    {}

    /// Reset function
    void Reset();

    /// Radius of shower, either read from data product or computed
    double _radius;

    /// Opening angle of a shower cone, either read from data product or computed
    double _cone_angle;

    /// Shower total energy
    double _energy;

    /// dE/dX @ shower start
    double _dedx;

    /// Cosmic score
    double _cosmogenic;

    ClassDef(SPAShower,1)
  };

  /**
     \class SPATrack
     @brief Data holder class to contain 3D track information. This is contained within sptool::SPAData instance.
  */
  class SPATrack : public TObject,
		   public ::geoalgo::Trajectory {
    
  public:
    
    /// Default ctor
    SPATrack();

    /// Alternative ctor
    template <class T>
    SPATrack(const std::vector<T>& trk)
    {
      Reset();
      for(auto const& p : trk) Trajectory::push_back<T>(p);
      if(Trajectory::size() && (*this)[0].size() !=3)
	throw SPAException("SPATrack constructor takes only 3-dimentional trajectory!");
    }

    /// Reset 
    void Reset();

    /// Default dtor
    virtual ~SPATrack(){}

    /// Track total deposited energy
    double _energy;

    /// Cosmic score
    double _cosmogenic;

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
    std::vector<sptool::SPAShower> _showers;
    /// Tracks
    std::vector<sptool::SPATrack>  _tracks;
    /// Candidate event vertecies
    std::vector<geoalgo::Point_t>  _vtxs;

    ClassDef(SPAData,1)
  };
}
#endif
/** @} */ // end of doxygen group 

