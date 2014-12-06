/**
 * \file ExampleSPFit.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ExampleSPFit
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef LARLITE_EXAMPLESPFIT_H
#define LARLITE_EXAMPLESPFIT_H

#include "Analysis/ana_base.h"
#include "SPTAnaBase.h"
#include "SPTBase/SPFilterBase.h"
#include "SPTBase/SPAlgoBase.h"

namespace larlite {
  /**
     \class ExampleSPFit
     Example analysis unit for training an algorithm
   */
  class ExampleSPFit : public SPTAnaBase {
  
  public:

    /// Default constructor
    ExampleSPFit();

    /// Default destructor
    virtual ~ExampleSPFit(){};

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

    void SetFilter(::sptool::SPFilterBase* f) { _filter = f; }
    
    void SetAlgo(::sptool::SPAlgoBase* a) { _algo = a; }

  protected:
    
    ::sptool::SPFilterBase *_filter;
    ::sptool::SPAlgoBase *_algo;
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
