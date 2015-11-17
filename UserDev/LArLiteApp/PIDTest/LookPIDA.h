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
      fPIDATree=nullptr;

      fPIDA_0.clear();
      fPIDA_1.clear();
      fPIDA_2.clear();
      
      fdEdx_0.clear();
      fdEdx_1.clear();
      fdEdx_2.clear();
      
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
    /// Function to prepare TTree
    void InitializeAnaTree();
    
    /// Function to clear TTree
    void ClearTreeVar();

    // My analysis TTree
    TTree *fPIDATree;
    std::vector<double> fPIDA_0;
    std::vector<double> fPIDA_1;
    std::vector<double> fPIDA_2;
	              
    std::vector<double> fdEdx_0;
    std::vector<double> fdEdx_1;
    std::vector<double> fdEdx_2;

    
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
