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
    AlgoPrimaryFinder();

    /// Default destructor
    virtual ~AlgoPrimaryFinder(){};

    /// Reset function
    virtual void Reset();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    virtual void ProcessEnd(TFile* fout);

    /// Override the ertool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to reconstruct the start-point isolated electrons
    virtual ParticleSet Reconstruct(const EventData &data);

    /// Set verbosity
    void setVerbose(bool on) { _verbose = on; _findRel.setDebug(on); }
    
    /// Use EMPart
    void useRadLength(bool on) { _useRadLength = on; }

    /// Setter for cut values to make decision on whether an object
    /// is primary or not
    void setVtxToTrkStartDist(double d) { _vtxToTrkStartDist = d; }
    void setVtxToTrkDist(double d) { _vtxToTrkDist = d; }
    void setVtxToShrStartDist(double d) { _vtxToShrStartDist = d; }
    void setMaxIP(double d) { _maxIP = d; }


  protected:

    /// Function to check wether a shower is e- or gamma-like
    /// Returns true if gamma-like
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);

    // verbose flag
    bool _verbose;
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
    double _E; // energy of shower
    int    _PDG; // PDG code assigned by AlgoEMPart
    int    _VsTrack; // comparing vs track (==1)
    double _thatE; // Energy of other shower/track
    double _dEdx;
    double _IP; // Impact Paramter with other object
    double _IPthisStart; // distance from IP to this shower start point
    double _IPthatStart; // distance from IP to that shower/track start point
    double _IPthatTrunk; // distance from IP to taht shower's trunk
    double _IPtrkBody; // distance from IP to body of track (if comparing with track)

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
