/**
 * \file MCTopo_BG_ccpi0_Sig.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class MCTopo_BG_ccpi0_Sig
 *
 * @author ryan
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef LARLITE_MCTOPO_BG_CCPI0_H
#define LARLITE_MCTOPO_BG_CCPI0_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mcpart.h"

#include "LArUtil/Geometry.h"


#include <string>


namespace larlite {
  /**
     \class MCTopo_BG_ccpi0_Sig
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCTopo_BG_ccpi0 : public ana_base{
  
  public:

    /// Default constructor
    MCTopo_BG_ccpi0(){ _name="MCTopo_BG_ccpi0"; _fout=0;_bothccnc=false; _ccnc=1; _sig = true;  _topo=2; _fid=0.0; _showerenergy=0; _showerdetenergy=0; _showercontain=0;}

    /// Default destructor
    virtual ~MCTopo_BG_ccpi0(){}

    /** IMPLEMENT in MCTopo_BG_ccpi0_Sig.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCTopo_BG_ccpi0_Sig.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCTopo_BG_ccpi0_Sig.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

//--------
//Setters for various types of options
//--------
	// set for both ccnc or just a specific type 
	// if not set for both nc will be assumed
    void SetBothCCNC(bool bothccnc) { _bothccnc = bothccnc ; }
        // CC = 0 
        // NC = 1
    void SetCCNC(int ccnc) { _ccnc = ccnc ; }
	// if we are looking for signal
    void SetSignal(bool sig) { _sig = sig ; }
        // change this to string or something as time goes on... or keep a list
        // 0 = inclusive ==> any >1pi0 that is a final state particle (aka Jonathan's Definition)
        // 1 = inclusive ==> any  1 pi0 that is a final state particle
        // 2 = exclusive no cmeson ==> any  1pi0 that is a final state particle and has no final state pi+/-
    void SetTopology(int Topo) { _topo = Topo ; }
	// Set a fiducial cut volume
    void SetFiducial(double fid) { _fid = fid ; }
	// Set the MC Truth shower energy threshold
    void SetShowerEnergy(double shenergy) { _showerenergy = shenergy ; }
	// Set the Deposit shower energy threshold
    void SetDetShowerEnergy(double dshenergy) { _showerdetenergy = dshenergy ; }
	// Set the shower containment 
    void SetShowerContainment(double shcontain) { _showercontain = shcontain ; }
//--------
//--------
//--------

  protected:

        int _PassedEvt = 0;
        int _nEvt = 0;	
        int _nEvtinvol = 0;	
        int _Lost_Shower = 0;	
        int _Lost_Dalitz = 0;	

	bool _bothccnc;
	int _ccnc;
	bool _sig;
	int _topo;
	double _fid;
	double _showerenergy;
	double _showerdetenergy;
	double _showercontain;
    
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
