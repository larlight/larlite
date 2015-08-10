 /*
AlgoSingleGamma, July 2015

authors: Brooke Russell, brooke.russell@yale.edu
         Bobby Murrells, robertmurrells@gmail.com
         Joseph Zennamo, jzennamo@uchicago.edu
*/

#ifndef ERTOOL_ALGOSINGLEGAMMA_H
#define ERTOOL_ALGOSINGLEGAMMA_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERTool/Algo/AlgoPrimaryFinder.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include <algorithm> // for std::find
#include <utility>
#include <TTree.h>
namespace ertool {

  /**
     \class AlgoSingleGamma
     User custom Algorithm class made by kazuhiro
   */
  class AlgoSingleGamma : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoSingleGamma(const std::string& name="SingleGamma");

    /// Default destructor
    ~AlgoSingleGamma(){};

    /// Reset function
    void Reset();

    void ProcessBegin();

    void ProcessEnd(TFile* fout);

    /// Configuration setter
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

		// Set verbosity
    void setVerbose(bool on) { 
          _verbose = on; 
	  _findRel.setDebug(on); 
	  _primaryFinder.setVerbose(on);
    }
    
       /// Use EMPart
    void useRadLength(bool on) { _useRadLength = on; }

    void setRejectLongTracks(bool on) { _rejectLongTracks = on; }

    void setVtxToTrkStartDist(double d){
      _vtxToTrkStartDist = d;
      _primaryFinder.setVtxToTrkStartDist(d);
    }
    void setVtxToTrkDist(double d){
      _vtxToTrkDist = d;
      _primaryFinder.setVtxToTrkDist(d);
    }
    void setVtxToShrStartDist(double d){
      _vtxToShrStartDist = d;
      _primaryFinder.setVtxToShrStartDist(d); 
    }
    void setMaxIP(double d){
      _maxIP = d;
      _primaryFinder.setMaxIP(d); 
    }
    void setEThreshold(double E){ _Ethreshold = E; }

    void setVtxProximityCut(double d) { _vtxProximityCut = d; }
    
  private:

    ::geoalgo::AABox fTPC;

    /// clear tree
    void ClearTree();

  protected:
    
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
    double _vtxToElecShrStartDist;
    double _vtxToGammShrStartDist;
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
    // Backwards Distance to Wall Cut:
    // if "single" shower vertex is near to wall, it is likely due to
    // cosmic.  This becomes more important cut with small/no fid
    // volume cut. Default set off. 
    double _BDtW ;
    // Backwards Distance to Top Wall Cut:
    // if "single" shower vertex is extended backwards along its
    // trajectory, when does it hit the top wall of TPC (assuming
    // top wall is extended to infinity)? This param will be
    // negative for upwards facing showers.
    double _BDtTW ;

    // Keep track of whether a sister track to the shower has
    // been found
   bool _hassister;
   bool _track_assister;
   bool _elec_assister;
   bool _gam_assister;
   bool _vtx_assister;
   bool _muon_assister;
   
    // Other algorithms to use
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;
    AlgoPrimaryFinder _primaryFinder;
    // GeoAlgo Tool
    ::geoalgo::GeoAlgo _geoAlgo;

    //debug histos
    TH1F* _e_ll_values;
    TH1F* _dedx_values;

    //Tree -> one entry for every time EMPart LL function
    // is called using both dEdx and rad-length
    TTree* _empart_tree;
    double _dedx;
    double _radlen;
    int    _pdg;

    //Tree -> one entry per shower-other comparison
    // therefore possibly multiple entries for each shower
    TTree* _alg_tree;

    //For every Shower
    double _E; // energy of shower
    int    _PDG; // PDG code assigned by AlgoEMPart

    //==========================================

    /* Parameters related to this shower and this track */
    double _ThsShwrThsTrk;
    double _ThsShwrE;
    double _dedxThsShwr;
    double _ThsTrkE;
    double _IPst;
    double _IPstThsShwrSt;
    double _IPstThsTrkSt;
    double _IPstThsTrkBody;

    /* Parameters related to this track and other track */
    double _ThsTrkOthTrk;
    double _OthTrkE;
    double _IPtt;
    double _IPttOthTrkSt;
    double _IPttOthTrkBody;

    /* Parameters related to this shower and other shower */
    double _ThsShwrOthShwr;
    double _OthShwrE;
    double _dedxOthShwr;
    double _IPss;
    double _IPssOthShwrSt;

    //=============================================
    
    double _distst; // distance between shower start point and impact parameter mid point
    
    double _IPsot;
    double _distsot; // original shower and other track
    double _disto; // distan
    //==========================================

    
    int    _VsTrack; // comparing vs track (==1)
    double _thatE; // Energy of other shower/track
    double _thisShwrE;
    double _thisTrkE;
    double _dEdx;
    double _IP; // Impact Paramter with other object
    double _Impa;
    double _IPthisShwrStrt;
    double _IPthisTrkStrt;
    double _IPthisStart;
    double _IPthatStart; // distance from IP to that shower/track start point
    double _IPathatStart;
    double _IPtrkBody; // distance from IP to body of track (if comparing with track)
    double _distBackAlongTraj; // distance backwards from vertex to nearest wall
    double _distToTopWall; // distance backwards along traj from vertex to top wall(extended to infinity)
    double _vtx_min_radius; // minimum radius to claim a vertex point
    
    //At the algo level what is tied to an event
    int _Ngamma; // Number of photons matched to an event 
    int _Ntrks; // Number of tracks matched to an event
    int _Nmu; // Number of muons matched to an event

    int track_gamma;
    int track_elec;
    int track_aelec;

    TH1D* _hRadius; // radius histogram
    //TH1D* _IPst; // impact parameter
    TH1D* _IPj; // impact parameter "this" track with "other" track
    TH1D* _IPsn; // impact parameter "other" shower with "this" shower
  };
}
#endif

/** @} */ // end of doxygen group 
