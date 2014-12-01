/**
 * \file ShowerPdfFactory.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ShowerPdfFactory
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef LARLITE_SHOWERPDFFACTORY_H
#define LARLITE_SHOWERPDFFACTORY_H

#include <iostream>
#include <RooRealVar.h>
#include <RooExponential.h>
#include <RooGaussian.h>
#include <RooGenericPdf.h>
#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooAbsData.h>
#include "SPAException.h"
#include "SPTEnv.h"
namespace sptool {

  typedef std::vector<RooRealVar> RooRealVars_t;

  /**
     \class ShowerPdfFactory
     A factory class to instantiate specific PDFs (type RooAbsPdf) useful for shower selection.
  */
  class ShowerPdfFactory{
    
  public:
    
    /// Default constructor
    ShowerPdfFactory(){};
    
    /// Default destructor
    virtual ~ShowerPdfFactory(){};
    
    /// Instantiate radiation-length PDF
    RooExponential  RadLenPdf(RooRealVars_t &vars) const;

  protected:

    /// Instantiate a set of RooRealVar for RadLenPdf
    RooRealVars_t RadLenPdfVars() const;
    
  };
}

#endif
/** @} */ // end of doxygen group 

