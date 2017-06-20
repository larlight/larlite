/**
 * \file AlgoSingleMu.h
 *
 * \ingroup ERTool
 *
 * \brief This Algo returns a SPArticleSet of muons that are \n
          start-point isolated from other single muons.
 *
 * @author
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOSINGLEMU_H
#define ERTOOL_ALGOSINGLEMU_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include <algorithm> // for std::find
#include <utility>
#include <TTree.h>
#include <TH1.h>

namespace ertool {

    /**
       \class AlgoSinglemu
       User custom Algorithm class made by kazuhiro
     */
    class AlgoSinglemu : public AlgoBase {

    public:

        /// Default constructor
        AlgoSinglemu(const std::string& name = "Singlemu");

        /// Default destructor
        ~AlgoSinglemu() {};

        /// Reset function
        void Reset();

        /// What to do before event-loop begins
        void ProcessBegin();

        void ProcessEnd(TFile* fout);

        /// Override the ertool::SPTBase::LoadParams function
        void AcceptPSet(const ::fcllite::PSet& cfg);

        /// Function to reconstruct the start-point isolated electrons
        bool Reconstruct(const EventData &data, ParticleGraph& graph);

	bool isCosmic(NodeID_t node, ParticleGraph& graph);

        /// Set verbosity
        void SetVerbosity(msg::Level_t level) {
            MessageUtil::SetVerbosity(level);
            _findRel.SetVerbosity(level);
        }

        void setRejectShortTracks(bool on) { _rejectShortTracks = on; }
        void setVtxToTrkStartDist(double d) { _vtxToTrkStartDist = d; }
        void setVtxToTrkDist(double d) { _vtxToTrkDist = d; }
        void setVtxToShrStartDist(double d) { _vtxToShrStartDist = d; }
        void setMaxIP(double d) { _maxIP = d; }
        void setVtxProximityCut(double d) { _vtxProximityCut = d; }

    private:

        ::geoalgo::AABox fTPC;

        /// clear tree
        void ClearTree();

    protected:


        // Reject events with long primary tracks
        bool _rejectShortTracks;

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
        // Keep track of number of neutrinos found
        int _neutrinos;
        // Keep track of whether a sister track to the shower has
        // been found
        bool _hassister;

        // Other algorithms to use
        AlgoFindRelationship _findRel;
        // GeoAlgo Tool
        ::geoalgo::GeoAlgo _geoAlgo;

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
        double _distBackAlongTraj; // distance backwards from vertex to nearest wall
        double _distToTopWall; // distance backwards along traj from vertex to top wall(extended to infinity)

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
