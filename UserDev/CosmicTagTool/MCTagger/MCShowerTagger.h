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

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

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

    /// All muons tracks
    std::vector<geoalgo::Trajectory_t> _allMCTracks;
    std::vector<geoalgo::Trajectory_t> _allRecoTracks;

    double _X ;
    double _Y ;
    double _Z ;
    double _Px ;
    double _Py ;
    double _Pz ;
    
    double _minMuDist;
    double _minMuIP;
    
    // evaluate time-performance
    clock_t t;

	CosmicTagAlgo _tagger;
     

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
