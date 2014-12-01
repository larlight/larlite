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
#include "SPAManager.h"

namespace larlite {
  /**
     \class ExampleSPSelection
     User custom analysis class made by kazuhiro
   */
  class ExampleSPSelection : public ana_base{
  
  public:

    /// Default constructor
    ExampleSPSelection(){ _name="ExampleSPSelection"; _fout=0; _shower_producer=""; }

    /// Default destructor
    virtual ~ExampleSPSelection(){}

    /** IMPLEMENT in ExampleSPSelection.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ExampleSPSelection.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ExampleSPSelection.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Selection manager class instance
    ::sptool::SPAManager _mgr;

    /// Shower producer module's label
    std::string _shower_producer;

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
