/**
 * \file AlgoSingleE.h
 *
 * \ingroup ERTool
 * 
 * \brief This Algo returns a SPArticleSet of single electrons that are \n
          start-point isolated from other single electrons.
 *
 * @author kazuhiro + davidkaleko + davidc
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOSINGLEE_H
#define ERTOOL_ALGOSINGLEE_H

#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include <algorithm> // for std::find
#include <utility>

namespace ertool {

  /**
     \class AlgoSingleE
     User custom Algorithm class made by kazuhiro
   */
  class AlgoSingleE : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoSingleE();

    /// Default destructor
    virtual ~AlgoSingleE(){};

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

    void setRejectLongTracks(bool on) { _rejectLongTracks = on; }

    void setVtxToTrkStartDist(double d) { _vtxToTrkStartDist = d; }
    void setVtxToTrkDist(double d) { _vtxToTrkDist = d; }
    void setVtxToShrStartDist(double d) { _vtxToShrStartDist = d; }
    void setMaxIP(double d) { _maxIP = d; }
    void setEThreshold(double E) { _Ethreshold = E; }
    void setVtxProximityCut(double d) { _vtxProximityCut = d; }


  private:

    ::geoalgo::AABox fTPC;

    /// clear tree
    void ClearTree();

  protected:


    /// Function to filter out events with long primary tracks 
    /// return true if a muon is found
    bool filterMuons(const EventData &data, const std::vector<int> &secondaryTracks);

    /// Function to check wether a shower is e- or gamma-like
    /// Returns true if gamma-like
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);

    // verbose flag
    bool _verbose;
    // electron mass
    double _e_mass;
    // flag to decide whether to use EMPart or not
    // if True -> use radiation length to calculate LL
    // if False -> use only dEdx
    bool _useRadLength;

    // Energy thershold. If shower has < _Ethreshold -> don't consider
    double _Ethreshold;

    // Reject events with long primary tracks
    bool _rejectLongTracks;

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
    // Vertex Proximity Cut:
    // if "candidate vertices" from the tracks are found
    // (needs to be a vertex in common to 2+ tracks in event)
    // require that a shower be within this distance to the
    // closest candidate vertex
    double _vtxProximityCut;

    // Keep track of whether a sister track to the shower has
    // been found
    bool _hassister;

    // Other algorithms to use
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;
    // GeoAlgo Tool
    ::geoalgo::GeoAlgo _geoAlgo;

    //debug histos
    TH1F* _e_ll_values;
    TH1F* _dedx_values;

    //Tree -> one entry per shower-other comparison
    // therefore possibly multiple entries for each shower
    TTree* _alg_tree;
    double _E; // energy of shower
    int    _PDG; // PDG code assigned by AlgoEMPart
    int    _VsTrack; // comparing vs track (==1)
    double _thatE; // Energy of other shower/track
    double _dEdx;
    double _IP; // Impact Paramter with other object
    double _IPthisStart; // distance from IP to this shower start point
    double _IPthatStart; // distance from IP to that shower/track start point
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
