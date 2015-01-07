/**
 * \file MCShowerTagger.h
 *
 * \ingroup MCInfo
 * 
 * \brief Class def header for a class MCShowerTagger
 *
 * @author David Caratelli
 */

/** \addtogroup MCInfo
    
    @{*/

#ifndef MCSHOWERTAGGER_H
#define MCSHOWERTAGGER_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "MCPartInfo/MCPartGetter/ShowerCutCalculator.h"
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

    /// prepare TTree
    void prepareTree();

    void resetTree();

    double addTrack(const mctrack& track);

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
    std::vector<geoalgo::Trajectory_t> _allTracks;
    std::vector<int> _allTrackIDs;


    double _xmin, _xmax, _ymin, _ymax, _zmin, _zmax;


    //Cut Distance
    double _cutDist;
    
    
    TTree * _ana_tree ;
    
    int _run ;
    int _subrun ;
    int _event ;
    
    std::string _process ;
    int _PDG ;
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
    
    double _minMuDistExceptAncestor;
    double _minMuIPExceptAncestor;
    double _distToIPExceptAncestor;
    
    double _ancDist;
    double _ancIP;
    double _ancToIP;
    
    //Save info about parent as well
    int _parentPDG ;
    double _parentX ;
    double _parentY ;
    double _parentZ ;
    double _parentT ;
    
    double _parentPx;
    double _parentPy;
    double _parentPz ; 
    double _parentE ; 

    int _parentInActiveVolume ;
    
    
    //Save info about ancestor as well
    int _ancestorPDG ;
    double _ancestorX ;
    double _ancestorY ;
    double _ancestorZ ;
    double _ancestorT ;
    
    double _ancestorPx;
    double _ancestorPy;
    double _ancestorPz ; 
    double _ancestorE ; 
    
    int _ancestorInActiveVolume ;
    
    std::vector<std::vector<double> > ShowerTraj;
    std::vector<std::vector<double> > MotherTraj;
    std::vector<std::vector<double> > AncestorTraj;
    
    //histogram for muon track length
    TH1D *_hTrackTotLen;
    //histogram for number of tracks
    TH1I *_hNumTracks;
    //histogram for number of tracks
    TH1I *_hTrackPDG;

    // evaluate time-performance
    clock_t t;
     

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
