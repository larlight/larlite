/**
 * \file AlgoEMPart.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoEMPart
 *
 * @author jhewes15 + davidc1
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOGAMMASELECTION_H
#define ERTOOL_ALGOGAMMASELECTION_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Base/PdfFactory.h"
#include "ERTool/Base/RangeVar.h"
#include "ERTool/Base/Track.h"
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/UtilFunc.h"
#include "AlgoFindRelationship.h"
#include "TDatabasePDG.h"
#include <RooPlot.h>
#include <RooProdPdf.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace ertool {

  /**
     \class AlgoEMPart
     User custom SPAFilter class made by jhewes15
   */
  class AlgoEMPart : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoEMPart(const std::string& name="EMPart");

    /// Default destructor
    ~AlgoEMPart(){}

    /// Called only once by the constructor in its lifespan
    void SetDefaultParams();

    /// Resetter
    void Reset(){}

    /// Implement AcceptPSet
    void AcceptPSet(const ::fcllite::PSet& cfg);
    
    /// Switch e- (false) / gamma (true) mode
    void SetMode(const bool gamma) { _mode = gamma; }

    /// What to do before event-loop begins
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// What to do once event-loop is over
    void ProcessEnd(TFile* fout);

    /// Likelihood for a particle being gamma/electron
    double LL(bool is_electron, double dedx, double rad_length = -1.);

    /// Set bounds for dEdx PDF fitting
    void SetFitRange_dEdx   (double min, double max, bool gamma=true);

    /// Set bounds for radiation length PDF fitting
    void SetFitRange_RadLen (double min, double max, bool gamma=true);

    /// Set verbosity mode
    void setVerbose(bool on) { _verbose = on; }

    /// Set Plot mode (save output plots)
    void setPlot(bool on) { _plot = on; }

    /// Set whether to load parameters stored in config
    void setLoadParams(bool on) { _loadParams = on; }

  protected:

    PdfFactory _factory; ///< P.D.F. factory class instance

    AlgoFindRelationship _findRel;

    double _e_mass; ///< Electron's mass
    double _g_mass; ///< Gamma's mass

    bool _verbose; ///< verbosity mode for debug
    bool _plot;    ///< True: save plots with PDF information
    bool _mode;    ///< e-/gamma mode (true: gamma, false: e-)
    bool _loadParams; ///< whether to load params or not from config
    
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

    TH1D* _hradLen_e;
    TH1D* _hradLen_g;

  };
}
#endif

/** @} */ // end of doxygen group 
