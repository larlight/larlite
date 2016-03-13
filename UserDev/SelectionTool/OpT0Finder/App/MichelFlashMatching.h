/**
 * \file MichelFlashMatching.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class MichelFlashMatching
 *
 * @author david caratelli
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_MICHELFLASHMATCHING_H
#define LARLITE_MICHELFLASHMATCHING_H

#include "Analysis/ana_base.h"
#include "MCQCluster.h"
#include "OpT0Finder/Base/FlashMatchManager.h"
#include <TTree.h>
#include <TH2D.h>

namespace larlite {
  /**
     \class MichelFlashMatching
     User custom analysis class made by SHELL_USER_NAME
   */
  class MichelFlashMatching : public ana_base{
  
  public:

    MichelFlashMatching();

    /// Default destructor
    virtual ~MichelFlashMatching(){}

    /** IMPLEMENT in MichelFlashMatching.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MichelFlashMatching.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MichelFlashMatching.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::flashana::FlashMatchManager& Manager() { return _mgr;}

    void SetROStart(double t) { _RO_start = t; }
    void SetROEnd(double t)   { _RO_end = t; }

    void SetTrigTime(double t) { _Trig_time = t; }

    void SetEDiff(double e) { _e_diff = e ; }

    void UseAbsolutePE(bool tof) { _useAbsPE = tof ; }

    void SetStepLength(double step){ _step_len = step ; }

    void SetMinPE(double pe) { _min_PE = pe; }

    // set producer names
    void setOpFlashProducer(std::string s) { _opflash_producer = s; }
    void setTrackProducer  (std::string s) { _track_producer   = s; }
    void setPFPartProducer (std::string s) { _pfpart_producer  = s; }

  protected:

    // set producers
    std::string _opflash_producer;
    std::string _track_producer;
    std::string _pfpart_producer;

    ::flashana::FlashMatchManager _mgr;
    ::flashana::MCQCluster _mcqclustering;

    // set minimum PE required for a flash to be used
    double _min_PE;

    // readout start : 
    // time before the trigger when the RO start
    double _RO_start; // us
    // readout end :
    // time after the trigger when the RO ends
    double _RO_end; // us
    // trigger time
    // the time at which, within the TPC readout
    // the trigger should arrive
    // for MC this is generally 0
    double _Trig_time; // us

    // Configurable params
    bool _useAbsPE ;
    double _step_len ;
    double _e_diff ; //Energy difference cut between start and end of track.  

    TTree* _flashmatch_tree;
    double _npe;
    double _npts;
    double _flash_x;
    double _flash_y;
    double _flash_z;
    double _tpc_x;
    double _tpc_y;
    double _tpc_z;
    double _score;
    double _flash_time;
    double _mc_time;
    double _mc_x;
    double _mc_y;
    double _mc_z;
    double _mc_dx;

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
