#ifndef SHOWERPDFFACTORY_CXX
#define SHOWERPDFFACTORY_CXX

#include "ShowerPdfFactory.h"

namespace sptool {

  RooExponential*  ShowerPdfFactory::RadLenPdf(RooRealVar &x, RooRealVar &l) const
  {
    // Instantiate pdf and return
    return new RooExponential("_RadLenPDF","Radiation Length",x,l);
  }

  RooExponential*  ShowerPdfFactory::RadLenPdfMod(RooRealVar &t, RooRealVar &tau) const
  {
    // Instantiate pdf and return
    return new RooExponential("_RadLenPDF","Radiation Length",t,tau);
  }

  
  RooGaussian* ShowerPdfFactory::dEdxPdf(RooRealVar &x, RooRealVar &mu, RooRealVar &sigma) const
  {
    // vars[0] is the mean. vars[1] the spread
    return new RooGaussian("_dEdxPdf","dEdx Pdf", x, mu, sigma);
  }


  RooExponential*  ShowerPdfFactory::EvsAngPdf(RooRealVars_t &vars) const
  {
    // If user provides no pre-defined variables, define here
    if(!vars.size()){
      vars.reserve(2);
      vars.push_back(RooRealVar("_AngRange","Opening Angle [Deg]",0.,180.));
      vars.push_back(RooRealVar("_AngConst","Opening Angle [Deg]",0.,180.));
    }

    // Check # of variables
    if(vars.size()!=2) throw SPAException(Form("%s requires 2 RooRealVar!",__FUNCTION__));

    // Instantiate pdf and return
    return new RooExponential("_EvsAngPDF","Opening Angle PDF",vars[0],vars[1]);
  }

}

#endif
