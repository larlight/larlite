/**
 * \file ClusterInfo.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class ClusterInfo
 *
 * @author jhewes15
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_CLUSTERINFO_H
#define LARLITE_CLUSTERINFO_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class ClusterInfo
     User custom analysis class made by SHELL_USER_NAME
   */
  class ClusterInfo : public ana_base{
  
  public:

    /// Default constructor
    ClusterInfo(){ _name="ClusterInfo"; _fout=0;}

    /// Default destructor
    virtual ~ClusterInfo(){}

    /** IMPLEMENT in ClusterInfo.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterInfo.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterInfo.cc! 
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
