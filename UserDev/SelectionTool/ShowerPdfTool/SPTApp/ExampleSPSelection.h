/**
 * \file ExampleSPSelection.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ExampleSPSelection
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef LARLITE_EXAMPLESPSELECTION_H
#define LARLITE_EXAMPLESPSELECTION_H

#include "Analysis/ana_base.h"
#include "SPTBase/SPAManager.h"
#include "SPTAnaBase.h"
namespace larlite {
  /**
     \class ExampleSPSelection
     Example analysis unit for running SPAManager
   */
  class ExampleSPSelection : public SPTAnaBase {
  
  public:

    /// Default constructor
    ExampleSPSelection();

    /// Default destructor
    virtual ~ExampleSPSelection(){}

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

    /// Selection manager class instance
    ::sptool::SPAManager _mgr;

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
