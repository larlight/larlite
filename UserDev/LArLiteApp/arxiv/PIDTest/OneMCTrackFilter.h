/**
 * \file OneMCTrackFilter.h
 *
 * \ingroup PIDTest
 * 
 * \brief Class def header for a class OneMCTrackFilter
 *
 * @author elenag
 */

/** \addtogroup PIDTest

    @{*/

#ifndef LARLITE_ONEMCTRACKFILTER_H
#define LARLITE_ONEMCTRACKFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class OneMCTrackFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class OneMCTrackFilter : public ana_base{
  
  public:

    /// Default constructor
    OneMCTrackFilter(){ _name="OneMCTrackFilter"; _fout=0;}

    /// Default destructor
    virtual ~OneMCTrackFilter(){}

    /** IMPLEMENT in OneMCTrackFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in OneMCTrackFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in OneMCTrackFilter.cc! 
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
