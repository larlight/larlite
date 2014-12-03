#ifndef SHOWERPDFFACTORY_CXX
#define SHOWERPDFFACTORY_CXX

#include "ShowerPdfFactory.h"

namespace sptool {

  RooExponential  ShowerPdfFactory::RadLenPdf(RooRealVars_t &vars) const
  {
    // If user provides no pre-defined variables, define here
    if(!vars.size()){
      vars.reserve(2);
      vars.push_back(RooRealVar("_RadLenx","Distance [cm]",kDOUBLE_MIN,kDOUBLE_MAX));
      vars.push_back(RooRealVar("_RadLenl","Radiation Length [cm]",kDOUBLE_MIN,kDOUBLE_MAX));
    }

    // Check # of variables
    if(vars.size()!=2) throw SPAException(Form("%s requires 2 RooRealVar!",__FUNCTION__));

    // Instantiate pdf and return
    return RooExponential("_RadLenPDF","Radiation Length",vars[0],vars[1]);
  }

}

#endif
