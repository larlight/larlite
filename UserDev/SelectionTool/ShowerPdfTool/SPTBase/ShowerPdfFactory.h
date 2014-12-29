/**
 * \file ShowerPdfFactory.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ShowerPdfFactory
 *
 * @author David, Jeremy, Kazu
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef LARLITE_SHOWERPDFFACTORY_H
#define LARLITE_SHOWERPDFFACTORY_H

#include <iostream>
#include <RooRealVar.h>
#include <RooExponential.h>
#include <RooGaussian.h>
#include <RooArgusBG.h>
#include <RooGenericPdf.h>
#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooAbsData.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include "SPAException.h"
#include "SPTEnv.h"
namespace sptool {

  typedef std::vector<RooRealVar> RooRealVars_t;

  /**
     \class ShowerPdfFactory
     @brief Factory class to instantiate specific PDFs (type RooAbsPdf) useful for shower selection. \n
     This utility class instantiates a various PDF useful for shower interaction selection.          \n
     In particular this factory class is used by SPAlgoX (sptool::SPAlgoBase children classes) but   \n
     it can be used in general outside SPT package. \n
     A possible future extension includes an instantiation of RooRealVar_t with default values set for \n
     specific physics though this is not yet implemented. \n

     For questions, contact David Caratelli, Jeremy Hewes or Kazu where the last person is least knowledgeable \n
     about PDF definition :)
  */
  class ShowerPdfFactory{
    
  public:
    
    /// Default constructor
    ShowerPdfFactory(){};
    
    /// Default destructor
    virtual ~ShowerPdfFactory(){};
    
    /// Instantiate radiation-length PDF
    RooAbsPdf* RadLenPdf(RooRealVar &x, RooRealVar &l) const;

    RooAbsPdf* RadLenPdfMod(RooRealVar &t, RooRealVar &tau) const;

    /// Instantiate dEdx PDF
    RooAbsPdf* dEdxPdf(RooRealVar &x, RooRealVar &mu, RooRealVar &sigma) const;

    /// Instantiate Angle-vs-Energy PDF
    RooAbsPdf* EvsAngPdf(RooRealVars_t &vars) const;

    /// Pi0 mass peak PDF
    //RooGaussian* Pi0Mass(RooRealVar& x,
    RooAbsPdf* Pi0Mass(RooRealVar& x,
		       RooRealVar& mu,
		       RooRealVar& sigma) const;

  };
}

#endif
/** @} */ // end of doxygen group 

