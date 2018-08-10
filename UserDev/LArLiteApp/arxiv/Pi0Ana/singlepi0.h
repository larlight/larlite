/**
 * \file singlepi0.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class singlepi0
 *
 * @author ryan
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef LARLITE_SINGLEPI0_H
#define LARLITE_SINGLEPI0_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcpart.h"

namespace larlite {
  /**
     \class singlepi0
     User custom analysis class made by SHELL_USER_NAME
   */
  class singlepi0 : public ana_base{
  
  public:

    /// Default constructor
    singlepi0(){ _name="singlepi0"; _fout=0; _Topo=0; _Sig=false; _ccnc=1;}

    /// Default destructor
    virtual ~singlepi0(){}

    /** IMPLEMENT in singlepi0.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in singlepi0.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in singlepi0.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetDefaultParams() { std::cout<< "Fill this is .cxx" <<std::endl ; } 


	// change this to string or something as time goes on... or keep a list
	// 0 = inclusive ==> any pi0 that is a final state particle
	// 1 = exclusive no cmeson ==> any pi0 that is a final state particle and has no final state pi+/-
    void SetTopology(int Topo) { _Topo = Topo ; }

	// If looking for signal(TRUE)
	// If looking at background(FALSE) 
    void SignalTopology(bool Sig) { _Sig = Sig ; }

	// CC = 0 
	// NC = 1
    void SetCCNC(int ccnc) { _ccnc = ccnc ; }


  protected:

	int _total_events; 
	int _passed_events; 
	
	int _Topo;
	bool _Sig;
	int _ccnc;
	double _Energy ;
	int _n_E;
    
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
