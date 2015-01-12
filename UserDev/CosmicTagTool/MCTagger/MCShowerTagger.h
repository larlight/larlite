/**
 * \file MCShowerTagger.h
 *
 * \ingroup MCInfo
 * 
 * \brief Class def header for a class MCShowerTagger
 *
 * @author Ariana Hackenburg 
 */

/** \addtogroup MCInfo
    
    @{*/

#ifndef MCSHOWERTAGGER_H
#define MCSHOWERTAGGER_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "MCPartInfo/MCPartGetter/ShowerCutCalculator.h"
#include "CosmicTagAlgo.h"
#include <vector>
#include <string>

namespace larlite {
  /**
     \class MCShowerTagger
     User custom analysis class made by david
  */
  class MCShowerTagger : public ana_base{
    
  public:

/// Default constructor
    MCShowerTagger(){ _name="MCShowerTagger"; _fout=0; _verbose=false; };

    /// Default destructor
    virtual ~MCShowerTagger(){};

    /** IMPLEMENT in MCShowerTagger.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerTagger.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerTagger.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

    void resetTree();

    void addTrack(const mctrack& track);

    void addTrack(const track& track);

    void SetDataType(std::string dtype) { _dataType = dtype; }
        
    protected:

    std::string _dataType;

    /// verbose
    bool _verbose;

    /// double Energy cut
    double _Ecut;

    /// Event number
    int _evtN;

    /// Instance of ShowerCutCalculator
    ShowerCutCalculator _cutParamCalculator;

    /// All muons tracks
    std::vector<geoalgo::Trajectory_t> _allMCTracks;
    std::vector<geoalgo::Trajectory_t> _allRecoTracks;
    std::vector<int> _allTrackIDs;


    double _xmin, _xmax, _ymin, _ymax, _zmin, _zmax;


    //Cut Distance
    double _cutDist;
    
    std::string _process ;
    int _trackID ;
    
    double _X ;
    double _Y ;
    double _Z ;
    double _T ;
    
    double _Px ;
    double _Py ;
    double _Pz ;
    double _E ;
    
    int _inActiveVolume ;
    
    double _distAlongTraj ;
    double _distBackAlongTraj;
    double _distBackToY;
    
    double _minMuDist;
    double _minMuIP;
    double _distToIP;
    
    
    std::vector<std::vector<double> > ShowerTraj;
    std::vector<std::vector<double> > MotherTraj;
    std::vector<std::vector<double> > AncestorTraj;
    
    //histogram for muon track length
    TH1D *_hTrackTotLen;
    //histogram for number of tracks
    TH1I *_hNumTracks;

    // evaluate time-performance
    clock_t t;

	CosmicTagAlgo _tagger;
	geoalgo::AABox _tpcBox;
     

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
