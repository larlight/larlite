/**
 * \file AlgoEMPart.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoEMPart
 *
 * @author jhewes15
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOGAMMASELECTION_H
#define ERTOOL_ALGOGAMMASELECTION_H

#include "Base/AlgoBase.h"
#include "Base/PdfFactory.h"
#include "Base/RangeVar.h"
#include "TDatabasePDG.h"
#include <RooPlot.h>
#include <RooProdPdf.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <cstdlib>
#include <ctime>

namespace ertool {

  /**
     \class AlgoEMPart
     User custom SPAFilter class made by jhewes15
   */
  class AlgoEMPart : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoEMPart();

    /// Default destructor
    virtual ~AlgoEMPart(){}

    /// Called only once by the constructor in its lifespan
    void SetDefaultParams();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);
    
    /// Switch e- (false) / gamma (true) mode
    void SetMode(const bool gamma) { _mode = gamma; }

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    virtual ParticleSet Reconstruct(const EventData &data);

    /// What to do once event-loop is over
    virtual void ProcessEnd(TFile* fout);

    /// Likelihood for a particle being gamma/electron
    double LL(bool is_electron, double dedx, double rad_length = -1.);

    /// Set bounds for dEdx PDF fitting
    void SetFitRange_dEdx   (double min, double max, bool gamma=true);

    /// Set bounds for radiation length PDF fitting
    void SetFitRange_RadLen (double min, double max, bool gamma=true);

    /// Set verbosity mode
    void setVerbose(bool on) { _verbose = on; }

  protected:

    PdfFactory _factory; ///< P.D.F. factory class instance

    double _e_mass; ///< Electron's mass
    double _g_mass; ///< Gamma's mass

    bool _verbose; ///< verbosity mode for debug
    bool _mode;    ///< e-/gamma mode (true: gamma, false: e-)
    
    // Variables
    RangeVar _e_dedx_fit_range;   ///< electron dE/dx [MeV/cm] range for fit
    RangeVar _e_radlen_fit_range; ///< electron radiation length [cm] range for fit
    RangeVar _g_dedx_fit_range;   ///< gamma dE/dx [MeV/cm] range for fit
    RangeVar _g_radlen_fit_range; ///< gamma radiation length [cm] range for fit

    // Variables
    RooRealVar* _dEdxVar;      ///< dE/dx [MeV/cm] range
    RooRealVar* _radLenVar;    ///< radiation length [cm] range

    // e- RadLen PDF 
    RooAbsPdf*  _e_radLenPdf;  ///< e- RadLen PDF
    RooDataSet* _e_radLenData; ///< e- pdf RadLen data set

    // e- dEdx PDF
    RooAbsPdf*  _e_dEdxPdf;    ///< e- dEdx PDF
    RooDataSet* _e_dEdxData;   ///< e- dEdx data set

    // gamma RadLen PDF
    RooAbsPdf*  _g_radLenPdf;  ///< gamma RadLen Pdf
    RooDataSet* _g_radLenData; ///< gamma RadLen data set

    // gamma dEdx PDF
    RooAbsPdf*  _g_dEdxPdf;    ///< gamma dEdx Pdf
    RooDataSet* _g_dEdxData;   ///< gamma dEdx data set

    RooRealVar *_tmp_mean, *_tmp_sigma;

  };
}
#endif

/** @} */ // end of doxygen group 
