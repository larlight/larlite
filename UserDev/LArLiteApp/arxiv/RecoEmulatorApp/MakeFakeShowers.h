/**
 * \file MakeFakeShowers.h
 *
 * \ingroup RecoEmulatorApp
 * 
 * \brief Class def header for a class MakeFakeShowers
 *
 * @author david
 */

/** \addtogroup RecoEmulatorApp

    @{*/

#ifndef LARLITE_MAKEFAKESHOWERS_H
#define LARLITE_MAKEFAKESHOWERS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MakeFakeShowers
     User custom analysis class made by SHELL_USER_NAME
   */
  class MakeFakeShowers : public ana_base{
  
  public:

    /// Default constructor
    MakeFakeShowers(){ _name="MakeFakeShowers"; _fout=0;}

    /// Default destructor
    virtual ~MakeFakeShowers(){}

    /** IMPLEMENT in MakeFakeShowers.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MakeFakeShowers.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MakeFakeShowers.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
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
