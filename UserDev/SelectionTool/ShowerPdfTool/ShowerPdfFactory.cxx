#ifndef SHOWERPDFFACTORY_CXX
#define SHOWERPDFFACTORY_CXX

#include "ShowerPdfFactory.h"

namespace sptool {

  RooAbsPdf*  ShowerPdfFactory::RadLenPdf() const
  {
    auto vars = RadLenPdfVars();
    return new RooExponential("_RadLenPDF","Radiation Length",*(vars[0]),*(vars[1]));
  }

  std::vector<RooRealVar*> ShowerPdfFactory::RadLenPdfVars() const
  {
    std::vector<RooRealVar*> vars;
    vars.reserve(2);
    vars.push_back(new RooRealVar("_RadLenx","Distance [cm]",kDOUBLE_MIN,kDOUBLE_MAX));
    vars.push_back(new RooRealVar("_RadLenl","Radiation Length [cm]",kDOUBLE_MIN,kDOUBLE_MAX));
    return vars;
  }

}

#endif
