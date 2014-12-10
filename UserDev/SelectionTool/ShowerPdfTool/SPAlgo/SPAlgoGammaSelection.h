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
    virtual ~SPAlgoGammaSelection(){};
    
    /// Function to initialise RooFit variables
    void Init(double xmin, double xmax, double lmin, double lmax);

    /// Function to evaluate input showers and determine a score
    virtual float Select(const SPAData &data);

    /// Provide data sample for fitting
    virtual void Fill(const SPAData &data);

    /// Evaluate parameters
    virtual void Fit(TFile* fout=nullptr);
    
  protected:
    
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
