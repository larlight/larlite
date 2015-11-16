/**
 * \file LookPIDA.h
 *
 * \ingroup PIDTest
 * 
 * \brief Class def header for a class LookPIDA
 *
 * @author elenag
 */

/** \addtogroup PIDTest

    @{*/

#ifndef LARLITE_LOOKPIDA_H
#define LARLITE_LOOKPIDA_H

#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class LookPIDA
     User custom analysis class made by SHELL_USER_NAME
   */
  class LookPIDA : public ana_base{
  
  public:

    /// Default constructor
    LookPIDA(){ 
      _name="LookPIDA"; 
      _fout=0;
      hPIDA = nullptr;
    }

    /// Default destructor
    virtual ~LookPIDA(){}

    /** IMPLEMENT in LookPIDA.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in LookPIDA.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in LookPIDA.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    /// Matching correctness
    TH1D *hPIDA;
    
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
