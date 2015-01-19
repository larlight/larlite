#ifndef ERTOOL_PDFFACTORY_CXX
#define ERTOOL_PDFFACTORY_CXX

#include "PdfFactory.h"

namespace ertool {

  RooAbsPdf*  PdfFactory::RadLenPdf(RooRealVar &x, RooRealVar &l) const
  {
    // Instantiate pdf and return
    return new RooExponential("_RadLenPDF","Radiation Length",x,l);
  }

  RooAbsPdf*  PdfFactory::RadLenPdfMod(RooRealVar &t, RooRealVar &tau) const
  {
    // Instantiate pdf and return
    return new RooExponential("_RadLenPDF","Radiation Length",t,tau);
  }

  
  RooAbsPdf* PdfFactory::dEdxPdf(RooRealVar &x, RooRealVar &mu, RooRealVar &sigma) const
  {
    return new RooGaussian("_dEdxPdf","dEdx Pdf", x, mu, sigma);
  }

  RooAbsPdf* PdfFactory::dEdxPdf_gamma(RooRealVar &x, RooRealVar &f,
				       RooRealVar &mu1, RooRealVar &sigma1,
				       RooRealVar &mu2, RooRealVar &sigma2) const
  {
    RooGaussian* _g1 = new RooGaussian("_dEdxPdf_gamma1","dEdx Pdf", x, mu1, sigma1);
    RooGaussian* _g2 = new RooGaussian("_dEdxPdf_gamma2","dEdx Pdf", x, mu2, sigma2);

    // weird error when doing RooAddPdf: 
    // [#0] ERROR:LinkStateMgmt -- RooAbsArg::recursiveCheckObservables(_dEdxPdf_gamma):
    // ERROR: one or more servers of node _dEdxPdf_gamma no longer exists!
    //return new RooAddPdf("_dEdxPdf_gamma","dEdx PDF Gamma",RooArgList(*_g1,*_g2),f);
    return new RooAddPdf("_dEdxPdf_gamma","dEdx PDF Gamma",*_g1,*_g2,f);
    /*
    RooAbsPdf* genpdf = RooClassFactory::makePdfInstance("dEdxGamma",
							 "_g_dedxfrac * 1./(sqrt(2*3.14*pow(_g_dedxsigma1,2)))*exp(-(pow((_dEdx-_g_dedxmu1),2))/(2*pow(_g_dedxsigma1,2))) + (1-_g_dedxfrac) * 1./(sqrt(2*3.14*pow(_g_dedxsigma2,2)))*exp(-(pow((_dEdx-_g_dedxmu2),2))/(2*pow(_g_dedxsigma2,2)))",
							 RooArgSet(x,mu1,sigma1,mu2,sigma2,f));
    */    
    //return genpdf;
  }

  RooAbsPdf* PdfFactory::dEdxConv(RooRealVar &x,
				  RooRealVar &meang, RooRealVar &sigmag,
				  RooRealVar &meanl, RooRealVar &sigmal) const
  {
    std::cout<<"called dEdxConv"<<std::endl; 
    static size_t ctr=0;
    ctr++;
    RooLandau* landau = new RooLandau(Form("landau_%zu",ctr),"landau", x, meanl, sigmal);
    RooGaussian* gauss = new RooGaussian(Form("gaussian_%zu",ctr),"gaussian", x, meang, sigmag);

    return new RooNumConvPdf("model", "model", x, *landau, *gauss);
  }


  RooAbsPdf*  PdfFactory::EvsAngPdf(RooRealVars_t &vars) const
  {
    // If user provides no pre-defined variables, define here
    if(!vars.size()){
      vars.reserve(2);
      vars.push_back(RooRealVar("_AngRange","Opening Angle [Deg]",0.,180.));
      vars.push_back(RooRealVar("_AngConst","Opening Angle [Deg]",0.,180.));
    }

    // Check # of variables
    if(vars.size()!=2) throw ERException(Form("%s requires 2 RooRealVar!",__FUNCTION__));

    // Instantiate pdf and return
    return new RooExponential("_EvsAngPDF","Opening Angle PDF",vars[0],vars[1]);
  }

  //RooGaussian* PdfFactory::Pi0Mass(RooRealVar& x,
  RooAbsPdf* PdfFactory::Pi0Mass(RooRealVar& x,
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

  //
  RooAbsPdf* PdfFactory::Pi0ShrCorrelation(RooRealVar& x,
						 RooRealVar& l) const
  {
    
    RooExponential* shr1 = new RooExponential("_radLenShr1","Rad. Length 1st Shower", x, l);
    RooExponential* shr2 = new RooExponential("_radLenShr2","Rad. Length 2nd Shower", x, l);

    return new RooProdPdf("radLenCorrelation","Correlation Rad length",RooArgSet(*shr1,*shr2));
  }


  RooAbsPdf* PdfFactory::UniformDistrib(RooRealVar& x) const
  {

    return new RooPolynomial("uniform","Uniform Distribution", x);
  }

}

#endif
