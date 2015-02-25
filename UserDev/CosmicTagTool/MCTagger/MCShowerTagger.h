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
#include "CosmicTagAlgo.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/track.h"
#include <TH1D.h>
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
    MCShowerTagger(){ _name="MCShowerTagger"; _fout=0; };

    /// Default destructor
    virtual ~MCShowerTagger(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

	void PrepareTTree();

	void Reset();

    void addTrack(const mctrack& track);

    void addTrack(const track& track);

    void SetDataType(std::string dtype) { _dataType = dtype; }
        
    protected:

    std::string _dataType;

	TTree * _tag_tree ;

    /// Event number
    int _evtN;

    /// All muons tracks
    std::vector<geoalgo::Trajectory_t> _allMCTracks;
    std::vector<geoalgo::Trajectory_t> _allRecoTracks;

	int _run ;
	int _subrun ;
	int _event ;

    double _X ;
    double _Y ;
    double _Z ;
    double _Px ;
    double _Py ;
    double _Pz ;
	double _E ;
	double _PDG ;
	std::string _process; 

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
    
    double _minMuDist;
    double _minMuIP;
	double _distToTopWall ;
	bool _inFV ;

	double _showerScore ; 
	double _trackScore ;

	double _containedE ;
    
	CosmicTagAlgo _tagger;

	TH1D* _numberOfEvents;
	TH1D* _numberOfShowers;
     

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
