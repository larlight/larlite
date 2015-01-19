/**
 * \file PdfFactory.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class PdfFactory
 *
 * @author David, Jeremy, Kazu
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_PDFFACTORY_H
#define ERTOOL_PDFFACTORY_H

#include <iostream>
#include <RooRealVar.h>
#include <RooExponential.h>
#include <RooGaussian.h>
#include <RooPolynomial.h>
#include <RooArgusBG.h>
#include <RooGenericPdf.h>
#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooAbsData.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooProdPdf.h>
#include <RooLandau.h>
#include <RooNumConvPdf.h>
#include "RooClassFactory.h"
#include "ERException.h"
#include "Env.h"

namespace ertool {

  typedef std::vector<RooRealVar> RooRealVars_t;

  /**
     \class PdfFactory
     @brief Factory class to instantiate specific PDFs (type RooAbsPdf) useful for shower selection. \n
     This utility class instantiates a various PDF useful for shower interaction selection.          \n
     In particular this factory class is used by AlgoX (ertool::AlgoBase children classes) but   \n
     it can be used in general outside ERTool package. \n
     A possible future extension includes an instantiation of RooRealVar_t with default values set for \n
     specific physics though this is not yet implemented. \n

     For questions, contact David Caratelli, Jeremy Hewes or Kazu where the last person is least knowledgeable \n
     about PDF definition :)
  */
  class PdfFactory{
    
  public:
    
    /// Default constructor
    PdfFactory(){};
    
    /// Default destructor
    virtual ~PdfFactory(){};
    
    /// Instantiate radiation-length PDF
    RooAbsPdf* RadLenPdf(RooRealVar &x, RooRealVar &l) const;

    RooAbsPdf* RadLenPdfMod(RooRealVar &t, RooRealVar &tau) const;

    /// Instantiate dEdx PDF
    RooAbsPdf* dEdxPdf(RooRealVar &x, RooRealVar &mu, RooRealVar &sigma) const;

    /// Instantiate dEdx PDF for gammas
    RooAbsPdf* dEdxPdf_gamma(RooRealVar &_g_dEdxVar, RooRealVar &f,
			     RooRealVar &mu1, RooRealVar &sigma1,
			     RooRealVar &mu2, RooRealVar &sigma2) const;
    
    /// Instantiate dEdx PDF for gauss convolved with landau
    RooAbsPdf* dEdxConv(RooRealVar &x,
			RooRealVar &meang, RooRealVar &sigmag,
			RooRealVar &meanl, RooRealVar &sigmal) const;

    /// Instantiate Angle-vs-Energy PDF
    RooAbsPdf* EvsAngPdf(RooRealVars_t &vars) const;

    /// Pi0 mass peak PDF
    //RooGaussian* Pi0Mass(RooRealVar& x,
    RooAbsPdf* Pi0Mass(RooRealVar& x,
		       RooRealVar& mu,
		       RooRealVar& sigma) const;

    /// Pi0 correlation between 2 shower distance from pi0 decay
    RooAbsPdf* Pi0ShrCorrelation(RooRealVar& x,
				 RooRealVar& l) const;

    /// Uniform distribution PDF : a 0th order polynomial
    RooAbsPdf* UniformDistrib(RooRealVar& x) const;

  };
}

#endif
/** @} */ // end of doxygen group 

