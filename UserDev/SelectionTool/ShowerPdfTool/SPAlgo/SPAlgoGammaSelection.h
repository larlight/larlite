/**
 * \file SPAlgoGammaSelection.h
 *
 * \ingroup SPAlgo
 * 
 * \brief Class def header for a class SPAlgoGammaSelection
 *
 * @author jhewes15
 */

/** \addtogroup SPAlgo

    @{*/

#ifndef SELECTIONTOOL_SPALGOGAMMASELECTION_H
#define SELECTIONTOOL_SPALGOGAMMASELECTION_H

#include "SPTBase/SPAlgoBase.h"
#include "SPTBase/ShowerPdfFactory.h"

namespace sptool {

  /**
     \class SPAlgoGammaSelection
     User custom SPAFilter class made by jhewes15
   */
  class SPAlgoGammaSelection : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoGammaSelection();

    /// Default destructor
    virtual ~SPAlgoGammaSelection(){}
    
    /// Function to initialise RooFit variables
    virtual void Reset();

    /// Functions to set active mode
    void SetElectronMode(){ _name = "PDF_electron"; xmin = 0; xmax = 100; lmin = -0.1; lmax = -0.01; }

    void SetGammaMode(){ _name = "PDF_gamma"; xmin = 0; xmax = 0.1; lmin = -100; lmax = -10; }

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Select(const SPAData &data);

    /// Provide data sample for fitting
    virtual void Fill(const SPAData &data);
    
  protected:
    
    std::string _name;
    double xmin = 0;
    double xmax = 0;
    double lmin = 0;
    double lmax = 0;
    ShowerPdfFactory* _factory;
    RooRealVars_t _vars;
    RooExponential _pdf;
    RooDataSet* _data;
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
