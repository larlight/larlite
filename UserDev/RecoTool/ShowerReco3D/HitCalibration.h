/**
 * \file HitCalibration.hh
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class HitCalibration
 *
 * @author David Caratelli
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef HITCALIBRATION_H
#define HITCALIBRATION_H

#include "Analysis/ana_base.h"
#include "AnalysisAlg/CalorimetryAlg.h"
#include "DataFormat/simch.h"
#include "DataFormat/hit.h"
#include "TF1.h"
#include "TH2D.h"
#include "TGraph.h"

namespace larlite {
  /**
     \class HitCalibration
     User custom analysis class made by david
   */
  class HitCalibration : public ana_base{
  
  public:

    /// Default constructor
    HitCalibration(){ _name="HitCalibration"; _fout=0; _verbose=false; };

    /// Default destructor
    virtual ~HitCalibration(){};


    virtual bool initialize();


    virtual bool analyze(storage_manager* storage);


    virtual bool finalize();

    void SetVerbose(bool on) { _verbose = on; }

    //get vector of all Q's deposited and all Energies deposited
    std::vector<float> getQs() { return _allQs; }
    std::vector<float> getEs() { return _allEs; }

    protected:

    std::map<UShort_t, ::larlite::simch> _simch_map;

    std::vector<float> _allQs;
    std::vector<float> _allEs;

    ::calo::CalorimetryAlg _CaloAlg;

    std::vector<float> _ADCs_Pl0;
    std::vector<float> _IDEs_Pl0;
    std::vector<float> _ADCs_Pl1;
    std::vector<float> _IDEs_Pl1;
    std::vector<float> _ADCs_Pl2;
    std::vector<float> _IDEs_Pl2;

    TH2D *_hADCsVSIDEs_Pl0;
    TH2D *_hADCsVSIDEs_Pl1;
    TH2D *_hADCsVSIDEs_Pl2;

    TH2D *_hRecoVSMcIDEs_Pl0;
    TH2D *_hRecoVSMcIDEs_Pl1;
    TH2D *_hRecoVSMcIDEs_Pl2;

    TH1D *_hRecoDivMcIDEs_Pl0;
    TH1D *_hRecoDivMcIDEs_Pl1;
    TH1D *_hRecoDivMcIDEs_Pl2;

    TH1D *_hRecoHitElectrons;

    TH2D *_hSimchQvsE;
    TH2D *_hSimchQvsE_noLifetime;
    TH2D *_hHitQvsE;
    TH1D *_hHitRecoEvsE;

    bool _verbose;

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
