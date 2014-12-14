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
#ifndef SELECTIONTOOL_SPADATA_H
#define SELECTIONTOOL_SPADATA_H

#include <map>
#include <set>
#include <TObject.h>
#include "GeoAlgo/GeoHalfLine.h"
#include "GeoAlgo/GeoTrajectory.h"
#include "SPTEnv.h"
#include "SPAException.h"
namespace sptool {

  typedef std::vector<size_t> SPAComb_t;

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

    ClassDef(SPATrack,1)
  };

  /**
    @brief Struct used by sptool::SPFilter Select() function return to note what should be filtered out.
    This is really used as a communication tool between sptool::SPFilter and sptool::SPAManager.      \n
    sptool::SPFilter is expected to remove some tracks, showers, and vtxs from sptool::SPAData object \n
    while sptool::SPAData instance is owned by sptool::SPAManager. SPAOrder object is a used to       \n
    communicate between those two: sptool::SPFilter should register index number of tracks, showers,  \n
    and vtxs to be removed. sptool::SPAManager applies this change.
   */
  struct SPAOrder {

  public:

    std::set<size_t> _rm_tracks;
    std::set<size_t> _rm_showers;
    std::set<size_t> _rm_vtxs;

    SPAOrder() 
      : _rm_tracks()
      , _rm_showers()
      , _rm_vtxs()
    {}

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

    /// Filter out tracks/showers/vtxs based on the SPAOrder
    void ApplyOrder(const SPAOrder& order);

    /// Utility to make a combination of m-elements from the full set of n-elements
    std::vector<sptool::SPAComb_t> Combination(const size_t n, 
					       const size_t m) const;

    ClassDef(SPAData,1)
  };

}
#endif
/** @} */ // end of doxygen group 

