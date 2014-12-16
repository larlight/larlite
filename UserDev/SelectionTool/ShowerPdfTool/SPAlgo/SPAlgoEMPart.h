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
    
    /// Function to initialise RooFit variables
    virtual void Reset();

    /// Switch e- (false) / gamma (true) mode
    void SetMode(const bool gamma) { _mode = gamma; }

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Select(const SPAData &data);

    /// Provide data sample for fitting
    virtual void Fill(const SPAData &data);

    virtual void ProcessEnd(TFile* fout);
    
  protected:

    bool _mode; ///< e-/gamma mode (true: gamma, false: e-)
    
    // Fit parameters
    double _xmin;   ///< rad length fit range min [cm]
    double _xmax;   ///< rad length fit range max [cm]

    double _e_lval; ///< electron rad length param mean [cm]
    double _e_lmin; ///< electron rad length param range min [cm]
    double _e_lmax; ///< electron rad length param range max [cm]

    double _g_lval; ///< gamma rad length param mean [cm]
    double _g_lmin; ///< gamma rad length param range min [cm]
    double _g_lmax; ///< gamma rad length param range max [cm]

    // e- pdf 
    RooExponential* _e_pdf;  ///< e- pdf
    RooDataSet*     _e_data; ///< e- pdf data set
    RooRealVars_t   _e_vars; ///< e- pdf cariable set
    // gamma pdf
    RooExponential* _g_pdf;  ///< e- pdf
    RooDataSet*     _g_data; ///< e- pdf data set
    RooRealVars_t   _g_vars; ///< e- pdf cariable set

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
