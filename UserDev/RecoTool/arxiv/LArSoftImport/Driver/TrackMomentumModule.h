/**
 * \file TrackMomentumModule.h
 *
 * \ingroup Driver
 * 
 * \brief Class def header for a class TrackMomentumModule
 *
 * @author kazuhiro
 */

/** \addtogroup Driver

    @{*/

#ifndef LARLITE_TRACKMOMENTUMMODULE_H
#define LARLITE_TRACKMOMENTUMMODULE_H

#include "Analysis/ana_base.h"
#include "../RecoAlg/TrackMomentumCalculator.h"

namespace larlite {
  /**
     \class TrackMomentumModule
     User custom analysis class made by SHELL_USER_NAME
   */
  class TrackMomentumModule : public ana_base{
  
  public:

    /// Default constructor
    TrackMomentumModule(){ _name="TrackMomentumModule"; _fout=0;}

    /// Default destructor
    virtual ~TrackMomentumModule(){}

    /** IMPLEMENT in TrackMomentumModule.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TrackMomentumModule.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TrackMomentumModule.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void AddMCTrackModule(const std::string& name)
    { _mctrack_module_v.push_back(name); }

    void AddTrackModule(const std::string& name)
    { _track_module_v.push_back(name); }

  protected:

    std::vector<std::string> _track_module_v;
    std::vector<std::string> _mctrack_module_v;

    ::trkf::TrackMomentumCalculator _alg;
    
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
