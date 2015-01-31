/**
 * \file ERAnaSingleE.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ERAnaSingleE
 *
 * @author David C.
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef LARLITE_ERANASINGLEE_H
#define LARLITE_ERANASINGLEE_H


#include "ERToolAnaBase.h"
namespace larlite {
  /**
     \class ERAnaSingleE
     Example analysis unit for running SPAManager
   */
  class ERAnaSingleE : public ERToolAnaBase {
  
  public:

    /// Default constructor
    ERAnaSingleE();

    /// Default destructor
    virtual ~ERAnaSingleE(){}

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

  private:

    int _numEvts;
    int _singleE;
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
