/**
 * \file AlgoLonelyE.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoLonelyE
 *
 * @author david
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOLONELYE_H
#define ERTOOL_ALGOLONELYE_H

#include "Base/AlgoBase.h"
#include "Algo/AlgoSingleE.h"
// include GeoAlgo functions
#include "GeoAlgo/GeoAlgoConstants.h"
#include "GeoAlgo/GeoAlgo.h"

namespace ertool {
  
  /**
     \class AlgoLonelyE
     User custom SPAFilter class made by david
  */
  class AlgoLonelyE : public AlgoBase {
    
  public:
    
    /// Default constructor
    AlgoLonelyE();

    /// Default destructor
    virtual ~AlgoLonelyE(){};

    /// Reset function
    virtual void Reset();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Override the ertool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);
    
    /// Function to set debug mode on or off
    void setDebug(bool on) { _debug = on; _alg_singleE.setVerbose(on); }

  private:

    /* 
       Function where shower is compared to other objects in evt
       to decide if the shower originates from other trks/shrs
    */
    bool isLonely(const Shower& s, const EventData& data) const;

    /// Check if the shower likely comes from a track
    double isShowerFromTrack(const geoalgo::HalfLine_t& shr, const geoalgo::Trajectory_t& trk) const;

    /// Check if the shower likely comes from another shower
    double isShowerFromShower(const geoalgo::HalfLine_t& islonely, const geoalgo::HalfLine_t& isparent) const;

    // debug mode
    bool _debug;

    // instance of GeoAlgo
    geoalgo::GeoAlgo _geoAlgo;

    // instance of AlgoSingleE
    AlgoSingleE _alg_singleE;

    // Constants to keep track of
    // min distance that the IP on a trk needs to be from the trk start
    double _minDistFromTrkStart;
    // distance the shower start point can be from it is supposed origin (calculated with an Impact Parameter, for example)
    double _radLenCut;
    // minimum IP between shower and track
    double _minIPShrTrk;
    // shower-trak cut distance
    double _cutDistToTrk;
    // shower-shower cut distance
    double _cutDistToShr;
    
  };
}
#endif

/** @} */ // end of doxygen group 
