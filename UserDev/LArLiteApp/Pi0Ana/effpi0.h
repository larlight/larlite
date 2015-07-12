/**
 * \file effpi0.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class effpi0
 *
 * @author ryan
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef LARLITE_EFFPI0_H
#define LARLITE_EFFPI0_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcpart.h"

namespace larlite {
  /**
     \class effpi0
     User custom analysis class made by SHELL_USER_NAME
   */
  class effpi0 : public ana_base{
  
  public:

    /// Default constructor
    effpi0(){ _name="effpi0"; _fout=0; } //_Topo=0; _isSignal=false; _ccnc=1;}

    /// Default destructor
    virtual ~effpi0(){}

    /** IMPLEMENT in effpi0.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in effpi0.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in effpi0.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

	// change this to string or something as time goes on... or keep a list
	// 0 = inclusive ==> any pi0 that is a final state particle
	// 1 = exclusive no cmeson ==> any pi0 that is a final state particle and has no final state pi+/-
    void SetTopology(int yesOrNo) { _isExclusive = yesOrNo ; }

	// If looking for signal(TRUE)
	// If looking at background(FALSE) 
    void SignalTopology(bool Sig) { _isSignal = Sig ; }

	// CC = 0 
	// NC = 1
    void SetCCNC(int ccnc) { _ccnc = ccnc ; }

    void SetFVCut(float fv) { _fv = fv ; }

  protected:

	int _total_events; 
	int _passed_events; 
	int _misID ;
	int _fidLoss ;
	float _fv ;

	int _nNCpi0	  ;
	int _nSingleNCpi0 ;
	int _nCCpi0 	  ;

	int _isExclusive;
	bool _isSignal;
	int _ccnc;
	double _energy ;
	int _n_E;
	int _event ;
    
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
