/**
 * \file Track.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ertool::Track and 
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_TRACK_H
#define ERTOOL_TRACK_H

#include "GeoAlgo/GeoTrajectory.h"
#include "RecoObjBase.h"
#include "ERException.h"
namespace ertool {

  /**
     \class Track
     @brief Data holder class to contain 3D track information. This is contained within ertool::EventData instance.
  */
  class Track : public RecoObjBase
	      , public ::geoalgo::Trajectory {
    
  public:
    
    enum TrackPartID_t {
      kUnknown,
      kProton,
      kKaon,
      kPion,
      kMuon,
      kPIDA,
      kTrackPartIDMax
    };

    /// Default ctor
    Track();

    /// Alternative ctor
    template <class T>
    Track(const std::vector<T>& trk) : _pid_score(kTrackPartIDMax,0)
    {
      Reset();
      for(auto const& p : trk) Trajectory::push_back<T>(p);
      if(Trajectory::size() && (*this)[0].size() !=3)
	throw ERException("Track constructor takes only 3-dimentional trajectory!");
    }

    /// Reset 
    void Reset();

    /// Default dtor
    virtual ~Track(){}

    /// Particle id
    TrackPartID_t _pid;

    /// Score for particle id
    std::vector<double> _pid_score;

    /// Total deposited energy
    double _energy;
    double _time;

  };

}
#endif
/** @} */ // end of doxygen group 

