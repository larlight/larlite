/**
 * \file SPAlgoEMPart.h
 *
 * \ingroup SPAlgo
 * 
 * \brief Class def header for a class SPAlgoEMPart
 *
 * @author jhewes15
 */

/** \addtogroup SPAlgo

    @{*/

#ifndef SELECTIONTOOL_SPALGOGAMMASELECTION_H
#define SELECTIONTOOL_SPALGOGAMMASELECTION_H

#include "SPTBase/SPAlgoBase.h"
#include "SPTBase/ShowerPdfFactory.h"
#include <RooPlot.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <cstdlib>
#include <ctime>

namespace sptool {

  /**
     \class SPAlgoEMPart
     User custom SPAFilter class made by jhewes15
   */
  class SPAlgoEMPart : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoEMPart();

    /// Default destructor
    virtual ~SPAlgoEMPart(){}

    /// Called only once by the constructor in its lifespan
    void Init();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);
    
    /// Switch e- (false) / gamma (true) mode
    void SetMode(const bool gamma) { _mode = gamma; }

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data);

    /// What to do once event-loop is over
    virtual void ProcessEnd(TFile* fout);

    /// Likelihood for a particle being gamma/electron
    double Likelihood(bool is_electron, double dedx, double rad_length = -1.);

  protected:

    bool _mode; ///< e-/gamma mode (true: gamma, false: e-)
    
    // Fit parameters
    double _xmin;   ///< rad length fit range min [cm]
    double _xmax;   ///< rad length fit range max [cm]
    
    double _dedxmin;///< dEdx fit range min [MeV]
    double _dedxmax;///< dEdx fit range max [MeV]

    double _e_lval;    ///< electron rad length param mean [cm]
    double _e_lmin;    ///< electron rad length param range min [cm]
    double _e_lmax;    ///< electron rad length param range max [cm]
    double _e_dedxmu; ///< electron dEdx param mean [MeV]
    double _e_dedxsigma; ///< electron dEdx param sigma [MeV]
    double _e_dedxmin; ///< electron dEdx param range min [MeV]
    double _e_dedxmax; ///< electron dEdx param range max [MeV]

    double _g_lval;    ///< gamma rad length param mean [cm]
    double _g_lmin;    ///< gamma rad length param range min [cm]
    double _g_lmax;    ///< gamma rad length param range max [cm]
    double _g_dedxmu; ///< gamma dEdx param mean [MeV]
    double _g_dedxsigma; ///< gamma dEdx param sigma [MeV]
    double _g_dedxmin; ///< gamma dEdx param range min [MeV]
    double _g_dedxmax; ///< gamma dEdx param range max [MeV]

    // e- RadLen PDF 
    RooExponential* _e_radLenPdf; ///< e- RadLen PDF
    RooDataSet*     _e_radLenData;      ///< e- pdf data set
    RooRealVar*     _e_radLenVar;      ///< e- pdf cariable set
    RooRealVar*     _e_radLenVal;

    // e- dEdx PDF
    RooGaussian*    _e_dEdxPdf;   ///< e- dEdx PDF
    RooDataSet*     _e_dEdxData;
    RooRealVar*     _e_dEdxVar;
    RooRealVar*     _e_dEdxMu;
    RooRealVar*     _e_dEdxSigma;

    // gamma RadLen PDF
    RooExponential* _g_radLenPdf;     ///< gamma RadLen Pdf
    RooDataSet*     _g_radLenData;    ///< gamma pdf data set
    RooRealVar*     _g_radLenVar;    ///< gamma pdf cariable set
    RooRealVar*     _g_radLenVal;

    // gamma dEdx PDF
    RooGaussian*    _g_dEdxPdf; ///< gamma dEdx Pdf
    RooDataSet*     _g_dEdxData;
    RooRealVar*     _g_dEdxVar;
    RooRealVar*     _g_dEdxMu;
    RooRealVar*     _g_dEdxSigma;

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
