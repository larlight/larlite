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
#include "Base/FlashMatchManager.h"
#include <TTree.h>

namespace larlite {
  /**
     \class UBT0Finder
     User custom analysis class made by SHELL_USER_NAME
   */
  class UBT0Finder : public ana_base{
  
  public:

    /// Default constructor
    UBT0Finder(){ _name="UBT0Finder"; _fout=0;}

    /// Default destructor
    virtual ~UBT0Finder(){_use_mc=false;}

    /** IMPLEMENT in UBT0Finder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in UBT0Finder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in UBT0Finder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::flashana::FlashMatchManager& Manager() { return _mgr;}

    void UseMC(bool doit=true) { _use_mc = true; }

  protected:

    ::flashana::FlashMatchManager _mgr;

    bool _use_mc;
    
    TTree* _tree;
    double _npe;
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
