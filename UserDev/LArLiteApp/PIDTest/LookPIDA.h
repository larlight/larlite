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
      
      fRecoPdg_0.clear();
      fRecoPdg_1.clear();
      fRecoPdg_2.clear();
      fTruePdg.clear();
      
      fdEdxReco_0.clear();
      fdEdxReco_1.clear();
      fdEdxReco_2.clear();
      
      fdEdxTrue.clear();
      
      fdEdxDelta_0.clear();
      fdEdxDelta_1.clear();
      fdEdxDelta_2.clear();

      fStepEnergyReco.clear();
      fStepEnergyTrue.clear();
      
      fRangeReco.clear();
      fRangeTrue.clear();
      
      fResRangeReco.clear();
      fResRangeTrue.clear();
      
      fKInitReco.clear();
      fKInitTrue.clear();
      
      fHitsNumbReco_0.clear();
      fHitsNumbReco_1.clear();
      fHitsNumbReco_2.clear();
      fHitsNumbTrue.clear();
      
      fTracksNumbReco.clear();
      fTracksNumbTrue.clear();      
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
    std::vector<double> fPIDA_0        ; // Reco PIDA on plan 0?
    std::vector<double> fPIDA_1        ; // Reco PIDA on plan 1?
    std::vector<double> fPIDA_2        ; // Reco PIDA on plan 2?

    std::vector<double> fRecoPdg_0     ; // Reco pdg 0
    std::vector<double> fRecoPdg_1     ; // Reco pdg 1
    std::vector<double> fRecoPdg_2     ; // Reco pdg 2
    std::vector<double> fTruePdg       ; // True pdg
	              
    std::vector<double> fdEdxReco_0    ; // Reco dEdx on plan 0?
    std::vector<double> fdEdxReco_1    ; // Reco dEdx on plan 1?
    std::vector<double> fdEdxReco_2    ; // Reco dEdx on plan 2?

    std::vector<double> fdEdxTrue      ; // True dEdx

    std::vector<double> fdEdxDelta_0   ; // Delta Reco - True dEdx on plan 0?
    std::vector<double> fdEdxDelta_1   ; // Delta Reco - True dEdx on plan 1?
    std::vector<double> fdEdxDelta_2   ; // Delta Reco - True dEdx on plan 2?

    std::vector<double> fStepEnergyReco; // Reco Step Energy
    std::vector<double> fStepEnergyTrue; // True Step Energy

    std::vector<double> fRangeReco     ; // Reco Range 
    std::vector<double> fRangeTrue     ; // Reco Range 

    std::vector<double> fResRangeReco  ; // Reco Residual Range 
    std::vector<double> fResRangeTrue  ; // True Residual Range 

    std::vector<double> fKInitReco     ; // Initial Reco Kinetic Energy 
    std::vector<double> fKInitTrue     ; // Initial True Kinetic Energy 

    std::vector<double> fHitsNumbReco_0; // Number of Reco hits per track 0
    std::vector<double> fHitsNumbReco_1; // Number of Reco hits per track 1
    std::vector<double> fHitsNumbReco_2; // Number of Reco hits per track 2
    std::vector<double> fHitsNumbTrue  ; // Number of True hits per track

    std::vector<double> fTracksNumbReco; // Number of Reco tracks per event
    std::vector<double> fTracksNumbTrue; // Number of True hits per track

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
