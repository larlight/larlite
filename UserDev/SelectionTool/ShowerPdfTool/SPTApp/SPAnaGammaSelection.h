/**
 * \file SPAnaGammaSelection.h
 *
 * \ingroup SPTApp
 * 
 * \brief Class def header for a class SPAnaGammaSelection
 *
 * @author jhewes15
 */

/** \addtogroup SPTApp

    @{*/

#ifndef LARLITE_SPANAGAMMASELECTION_H
#define LARLITE_SPANAGAMMASELECTION_H

#include "SPTAnaBase.h"
#include "SPTBase/SPAlgoBase.h"

namespace larlite {
  /**
     \class SPAnaGammaSelection
     User custom analysis class made by jhewes15
   */
  class SPAnaGammaSelection : public SPTAnaBase{
  
  public:

    /// Default constructor
    SPAnaGammaSelection();

    /// Default destructor
    virtual ~SPAnaGammaSelection(){};

    /** IMPLEMENT in SPAnaGammaSelection.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in SPAnaGammaSelection.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in SPAnaGammaSelection.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    void SetAlgo(sptool::SPAlgoBase* a) { _algo = a; }

    protected:
    
      sptool::SPAlgoBase *_algo;
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
