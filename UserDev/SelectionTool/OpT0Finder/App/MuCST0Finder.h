/**
 * \file MuCST0Finder.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class MuCST0Finder
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_MuCST0FINDER_H
#define LARLITE_MuCST0FINDER_H

#include "Analysis/ana_base.h"
#include "MCQCluster.h"
#include "OpT0Finder/Base/FlashMatchManager.h"
#include <TTree.h>
#include <TH2D.h>

namespace larlite {
  /**
     \class MuCST0Finder
     User custom analysis class made by SHELL_USER_NAME
   */
  class MuCST0Finder : public ana_base{
  
  public:

    MuCST0Finder();

    /// Default destructor
    virtual ~MuCST0Finder(){}

    /** IMPLEMENT in MuCST0Finder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MuCST0Finder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MuCST0Finder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::flashana::FlashMatchManager& Manager() { return _mgr;}

    const std::vector<flashana::FlashMatch_t> MatchResult() const { return _result; }

    void SetConfigFile(std::string name) { _config_file = name; }

  protected:

    ::flashana::FlashMatchManager _mgr;

    std::vector<flashana::FlashMatch_t> _result;

    // Configurable params
    std::string _config_file;
    std::string _track_producer;
    std::string _opflash_producer_beam;
    std::string _opflash_producer_cosmic;
    std::string _trigger_producer;
    double _flash_trange_start;
    double _flash_trange_end;
    size_t _num_tracks;
    std::vector<double> _gain_correction;
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
