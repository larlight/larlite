/**
 * \file UBT0Finder.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class UBT0Finder
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_UBT0FINDER_H
#define LARLITE_UBT0FINDER_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "MCQCluster.h"
#include "OpT0Finder/Base/FlashMatchManager.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <TTree.h>
#include <TH2D.h>


namespace larlite {
  /**
     \class UBT0Finder
     User custom analysis class made by SHELL_USER_NAME
   */
  class UBT0Finder : public ana_base{
  
  public:

    UBT0Finder () ;
      

    /// Default destructor
    virtual ~UBT0Finder(){_use_mc=false;}

    /** IMPLEMENT in UBT0Finder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    void Configure(const ::fcllite::PSet &pset);
    /** IMPLEMENT in UBT0Finder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in UBT0Finder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::flashana::FlashMatchManager& Manager() { return _mgr;}

    const std::vector<flashana::FlashMatch_t> MatchResult() const { return _result; }

    void SetConfigFile(std::string name) { _config_file = name; }

  protected:

    ::flashana::FlashMatchManager _mgr;
    std::string _config_file;
    double _shift_flash_time;

    std::vector<flashana::FlashMatch_t> _result;

    // Configurable params
    bool _use_mc;
    std::string _opflash_producer_beam;
    std::string _opflash_producer_cosmic;
    std::string _trigger_producer;
    std::string _track_producer;
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
