/**
 * \file ExampleERSelection.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ExampleERSelection
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef LARLITE_EXAMPLEERSELECTION_H
#define LARLITE_EXAMPLEERSELECTION_H

#include "Analysis/ana_base.h"
#include "ERTool/Base/Manager.h"
#include "ERToolAnaBase.h"
namespace larlite {
  /**
     \class ExampleERSelection
     Example analysis unit for running SPAManager
   */
  class ExampleERSelection : public ERToolAnaBase {
  
  public:

    /// Default constructor
    ExampleERSelection();

    /// Default destructor
    virtual ~ExampleERSelection(){}

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
    ::ertool::Manager _mgr;

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
