/**
 * \file CCSingleMuReco.h
 *
 * \ingroup CCSingleMu
 * 
 * \brief Class def header for a class CCSingleMuReco
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleMu

    @{*/

#ifndef LARLITE_CCSINGLEMURECO_H
#define LARLITE_CCSINGLEMURECO_H

#include "ERToolBackend/ERToolAnaBase.h"

namespace larlite {
  /**
     \class CCSingleMuReco
     User custom analysis class made by kazuhiro
   */
  class CCSingleMuReco : public ERToolAnaBase {
  
  public:

    /// Default constructor
    CCSingleMuReco();

    /// Default destructor
    virtual ~CCSingleMuReco(){};

    /** IMPLEMENT in CCSingleMuReco.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CCSingleMuReco.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CCSingleMuReco.cc! 
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
