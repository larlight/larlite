#ifndef SHOWERPDFFACTORY_CXX
#define SHOWERPDFFACTORY_CXX

#include "ShowerPdfFactory.h"

namespace sptool {

  RooAbsPdf*  ShowerPdfFactory::RadLenPdf(RooRealVar &x, RooRealVar &l) const
  {
    // Instantiate pdf and return
    return new RooExponential("_RadLenPDF","Radiation Length",x,l);
  }

  RooAbsPdf*  ShowerPdfFactory::RadLenPdfMod(RooRealVar &t, RooRealVar &tau) const
  {
    // Instantiate pdf and return
    return new RooExponential("_RadLenPDF","Radiation Length",t,tau);
  }

  
  RooAbsPdf* ShowerPdfFactory::dEdxPdf(RooRealVar &x, RooRealVar &mu, RooRealVar &sigma) const
  {
    // vars[0] is the mean. vars[1] the spread
    return new RooGaussian("_dEdxPdf","dEdx Pdf", x, mu, sigma);
  }


  RooAbsPdf*  ShowerPdfFactory::EvsAngPdf(RooRealVars_t &vars) const
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

  //RooGaussian* ShowerPdfFactory::Pi0Mass(RooRealVar& x,
  RooAbsPdf* ShowerPdfFactory::Pi0Mass(RooRealVar& x,
				       RooRealVar& mu,
				       RooRealVar& sigma) const
  {
    return new RooGaussian("_Pi0MassPdf","#Pi0 Mass Pdf",x,mu,sigma);
    /*
    RooGaussian gauss("_Pi0Gauss","pi0 mass gaussian",x,mu,sigma);
    RooRealVar argpar("_Pi0ArgusPar","Argus parameter",-20.,-100.,0.);
    RooArgusBG argus("_Pi0Argus","pi0 mass bg argus",x,RooFit::RooConst(134.9766/2.),argpar);
    RooRealVar nsig("nsig","",200.,0.,10000.);
    RooRealVar nbkg("nbkg","",800.,0.,10000.);
    return new RooAddPdf("_Pi0MassPdf","Pi0 Mass Pdf",RooArgList(gauss,argus),RooArgList(nsig,nbkg));
    */
  }

}

#endif
