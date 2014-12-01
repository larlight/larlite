#ifndef SHOWERPDFFACTORY_CXX
#define SHOWERPDFFACTORY_CXX

#include "ShowerPdfFactory.h"

namespace sptool {

  RooExponential  ShowerPdfFactory::RadLenPdf(RooRealVars_t &vars) const
  {
    if(!vars.size()) vars = RadLenPdfVars();
    if(vars.size()!=2) throw SPAException(Form("%s requires 2 RooRealVar!",__FUNCTION__));

    RooExponential pdf("_RadLenPDF","Radiation Length",
		       vars[0],
		       vars[1]
		       );
    
    return pdf;
  }

  RooRealVars_t ShowerPdfFactory::RadLenPdfVars() const
  {
    RooRealVars_t vars;
    vars.reserve(2);
    vars.push_back(RooRealVar("_RadLenx","Distance [cm]",kDOUBLE_MIN,kDOUBLE_MAX));
    vars.push_back(RooRealVar("_RadLenl","Radiation Length [cm]",kDOUBLE_MIN,kDOUBLE_MAX));
    return vars;
  }

}

#endif
