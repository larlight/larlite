/**
 * \file AlgoPrimaryFinder.h
 *
 * \ingroup ERTool
 * 
 * \brief This Algo returns a SPArticleSet of single electrons that are \n
          start-point isolated from other single electrons.
 *
 * @author davidc
 */

/** \addtogroup ERTool
    
    @{*/

#ifndef ERTOOL_PRIMARYFINDER_H
#define ERTOOL_PRIMARYFINDER_H

#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include <algorithm> // for std::find
#include <utility>

namespace ertool {

  /**
     \class AlgoPrimaryFinder
     User custom Algorithm class made by david caratelli
     This algorithm should search for primary tracks and
     showers and return a ParticleSet of the primary
     objects.
     To determine if an object is primary or not
     AlgoFindRelationship utilities are used and
     decisions are mased based on user-settable
     parameters such as minimum impact paramter, etc...
   */
  class AlgoPrimaryFinder : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoPrimaryFinder(const std::string& name="PrimaryFinder");

    /// Default destructor
    ~AlgoPrimaryFinder(){};

    /// Configuration setter
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Reset function
    void Reset();

    /// What to do before event-loop begins
    void ProcessBegin();

    void ProcessEnd(TFile* fout);

    /// Function to reconstruct the start-point isolated electrons
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Use EMPart
    void useRadLength(bool on) { _useRadLength = on; }

    /// Setter for cut values to make decision on whether an object
    /// is primary or not
    void setVtxToTrkStartDist(double d) { _vtxToTrkStartDist = d; }
    void setVtxToTrkDist(double d) { _vtxToTrkDist = d; }
    void setVtxToShrStartDist(double d) { _vtxToShrStartDist = d; }
    void setMaxIP(double d) { _maxIP = d; }

    /// Functions that returns if object given as 1st 
    ///argument comes from object given as second argument
    bool From(const ::geoalgo::Trajectory& thisTrack,
	      const ::geoalgo::Trajectory& thatTrack) const;

    bool From(const ::geoalgo::Cone& thisShower,
	      const ::geoalgo::Cone& thatShower) const;

    bool From(const ::geoalgo::Cone& thisShower,
	      const ::geoalgo::Trajectory& thatTrack) const;

    bool From(const ::geoalgo::Trajectory& thisTrack,
	      const ::geoalgo::Cone& thatShower) const;

  protected:

    /// Function to check wether a shower is e- or gamma-like
    /// Returns true if gamma-like
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);

    // flag to decide whether to use EMPart or not
    // if True -> use radiation length to calculate LL
    // if False -> use only dEdx
    bool _useRadLength;

    // constants to be used for decision-making
    // minimum distance that a reco-vertex must be away from the
    // start of a track for the vtx to be considered "on the track"
    // and the shower to come from the track, instead of the shower
    // and track being siblings.
    double _vtxToTrkStartDist;
    // maximum distance from the entire track for the reco-vertex
    // for the vertex to be considered on the track and the shower
    // to be considered as coming from the track
    double _vtxToTrkDist;
    // Max distance from shower start that the vertex can be
    // this value should be a few radiation lengths: the cut 
    // is placed to remove cases where the shower is meters
    // away from the vertex. Probably an accidental correlation
    double _vtxToShrStartDist;
    // Maximum impact parameter allowed between the two objects
    // if larger it means that the two lines/segments do not come
    // within this value at any point and they therefore are 
    // assumed to not be correlated
    double _maxIP;

    // Other algorithms to use
    AlgoFindRelationship _findRel;
    // GeoAlgo Tool
    ::geoalgo::GeoAlgo _geoAlgo;

    // quantities calculated for correlation determination:
    // double _IP; // Impact Paramter with other object
    // double _IPthisStart; // distance from IP to this shower start point
    // double _IPthatStart; // distance from IP to that shower/track start point
    // double _IPthatTrunk; // distance from IP to taht shower's trunk
    // double _IPtrkBody; // distance from IP to body of track (if comparing with track)

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
