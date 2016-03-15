/**
 * \file AlgoNCPi0Finder.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class AlgoNCPi0Finder
 *
 * @author ryan
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ALGONCPI0FINDER_H
#define ERTOOL_ALGONCPI0FINDER_H

#include "ERTool/Base/AlgoBase.h"
#include "AlgoPrimaryFinder.h"

#include "GeoAlgo/GeoAlgo.h"
#include <TTree.h>



namespace ertool {

  /**
     \class AlgoNCPi0Finder
     User custom Algorithm class made by kazuhiro
   */
  class AlgoNCPi0Finder : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoNCPi0Finder(const std::string& name="AlgoNCPi0Finder");

    /// Default destructor
    virtual ~AlgoNCPi0Finder(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

	// Setters 
    void SetVtxSphere_Radius( double vtxsphere_radius ) { _vtxsphere_radius = vtxsphere_radius ; }
    void SetMaxTrackLength( double maxtrackLength ) { _maxtrackLength = maxtrackLength ; }

    double DistanceToTPC(::geoalgo::Point_t point , ::geoalgo::Vector direction );


    protected :
	::geoalgo::GeoAlgo _geoAlgo;

	double _vtxsphere_radius;
	double _maxtrackLength;


	// TTree 
	TTree *_OPtree;
	double _NExitingOPTracks;
	double _Length_OPTracks;
	double _EDistToTPC_OPTracks;
	double _SDistToTPC_OPTracks;
	double _NTracks_OPTracks;

	double _pi0X;
	double _pi0Y;
	double _pi0Z;


	TTree *_tracktree;
	double _LongestTrack;
	double _SecondLongestTrack;

	TTree *_fulltracktree;
	double _LongestPrimaryTrack;
	double _PrimaryMuonTrackLength=-999;
	double _PrimaryPionTrackLength=-999;
	double _PrimaryKaonTrackLength=-999;
	double _PrimaryProtonTrackLength=-999;

	TTree *_largesttracktree;
	double _LargestPrimaryMuonTrackLength=-999;
	double _LargestPrimaryPionTrackLength=-999;
	double _LargestPrimaryKaonTrackLength=-999;
	double _LargestPrimaryProtonTrackLength=-999;
	double _nPrimaryTracks =-999;

	TTree *_bcontracktree;
	int _btrackcount;
	double _blongesttrack;

	TTree *_hcontracktree;
	int _htrackcount;
	double _hlongesttrack;

	TTree *_ncontracktree;
	int _ntrackcount;
	double _nlongesttrack;


  };
}
#endif

/** @} */ // end of doxygen group 
