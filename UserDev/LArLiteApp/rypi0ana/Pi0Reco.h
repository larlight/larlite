/**
 * \file Pi0Reco.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class Pi0Reco
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef LARLITE_PI0RECO_H
#define LARLITE_PI0RECO_H


#include "ERToolBackend/ERToolAnaBase.h"

namespace larlite {
  /**
     \class Pi0Reco
     Example analysis unit for running SPAManager
   */
  class Pi0Reco : public ERToolAnaBase {
  
  public:

    /// Default constructor
    Pi0Reco();

    /// Default destructor
    virtual ~Pi0Reco(){}

    /**
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /**
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /**
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

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
