#ifndef ERTOOL_PDFFACTORY_CXX
#define ERTOOL_PDFFACTORY_CXX

#include "PdfFactory.h"

namespace ertool {

  void PdfFactory::Register(RooRealVar* var) {

    if( _vars.find(var->GetName()) != _vars.end() )
      throw ERException(Form("RooRealVar with a name \"%s\" already created!",var->GetName()));
    _vars[var->GetName()] = var;

  }

  void PdfFactory::Register(RooAbsPdf* pdf) {

    if( _pdfs.find(pdf->GetName()) != _pdfs.end() )
      throw ERException(Form("RooAbsPdf with a name \"%s\" already created!",pdf->GetName()));
    _pdfs[pdf->GetName()] = pdf;

  }

  RooAbsPdf*  PdfFactory::RadiationLength(const std::string& name,
					  RooRealVar &x)
  {

    auto tau = new RooRealVar(Form("%s_radLen_tau",name.c_str()),
			      Form("Radiation length [cm] for %s", name.c_str()),
			      -20.,-1.e2,0.);
    Register(tau);
    std::cout<<"tau ptr: "<<tau<<std::endl;
    auto res = new RooExponential(Form("%s_radLen_pdf",name.c_str()),
				  Form("Radiation length PDF for %s",name.c_str()),
				  x,*tau);
    Register(res);
    return res;
  }


  RooAbsPdf* PdfFactory::Gaus(const std::string& name,
			      RooRealVar &x)
  {
    auto mean = new RooRealVar(Form("%s_Gaus_mean",  name.c_str()),
			       Form("Single gaussian mean for %s",name.c_str()),
			       1.,0.,10.);
    Register(mean);

    auto sigma = new RooRealVar(Form("%s_Gaus_sigma", name.c_str()),
				Form("Single gaussian sigma for %s",name.c_str()),
				0.1,0.,1.);
    Register(sigma);

    auto res = new RooGaussian(Form("%s_Gaus_pdf",   name.c_str()),
			       Form("Gauss Pdf for %s",name.c_str()),
			       x, *mean, *sigma);

    Register(res);
    
    return res;
  }

  RooAbsPdf* PdfFactory::Landau(const std::string& name,
				RooRealVar &x)
  {
    auto mean = new RooRealVar(Form("%s_Landau_mean",  name.c_str()),
			       Form("Single landau mean for %s",name.c_str()),
			       1.,0.,10.);
    Register(mean);

    auto sigma = new RooRealVar(Form("%s_Landau_sigma", name.c_str()),
				Form("Single landau sigma for %s",name.c_str()),
				0.1,0.,1.);
    Register(sigma);

    auto res = new RooLandau(Form("%s_Landau_pdf",   name.c_str()),
			     Form("Landau Pdf for %s",name.c_str()),
			     x, *mean, *sigma);

    Register(res);
    
    return res;
  }


  RooAbsPdf* PdfFactory::LandauPlusGauss(const std::string& name,
				RooRealVar &x)
  {
    auto meanL = new RooRealVar(Form("%s_Landau_mean",  name.c_str()),
				Form("Single landau mean for %s",name.c_str()),
				1.,0.,10.);
    Register(meanL);
    
    auto sigmaL = new RooRealVar(Form("%s_Landau_sigma", name.c_str()),
				 Form("Single landau sigma for %s",name.c_str()),
				 0.1,0.,1.);
    Register(sigmaL);

    auto landau = new RooLandau(Form("%s_Landau_pdf",   name.c_str()),
				Form("Landau Pdf for %s",name.c_str()),
				x, *meanL, *sigmaL);
    Register(landau);

    auto meanG = new RooRealVar(Form("%s_Gaus_mean",  name.c_str()),
				Form("Single gauss mean for %s",name.c_str()),
				1.,0.,10.);
    Register(meanG);
    
    auto sigmaG = new RooRealVar(Form("%s_Gaus_sigma", name.c_str()),
				 Form("Single gauss sigma for %s",name.c_str()),
				 0.1,0.,1.);
    Register(sigmaG);

    auto gauss = new RooGaussian(Form("%s_Gaus_pdf",   name.c_str()),
				 Form("Gauss Pdf for %s",name.c_str()),
				 x, *meanG, *sigmaG);
    Register(gauss);

    // need a fraction
    auto frac = new RooRealVar(Form("%s_fraction",name.c_str()),
			       "Fractional amplitude of gaussian peak",
			       1.0,0.0,1.0);
    Register(frac);

    // add the two
    auto res = new RooAddPdf(Form("%s_LandauPlusGaus_pdf",name.c_str()),
			     Form("Sum of Landau and Gaussian %s",name.c_str()),
			     *landau, *gauss, *frac);
    Register(res);
    
    return res;
  }


  // Landau + Landau (for gamma PDF to simulate 2 MeV/cm distrib)
  RooAbsPdf* PdfFactory::LandauPlusLandau(const std::string& name,
				RooRealVar &x)
  {
    auto meanL1 = new RooRealVar(Form("%s_Landau1_mean",  name.c_str()),
				Form("Single landau1 mean for %s",name.c_str()),
				1.,0.,10.);
    Register(meanL1);
    
    auto sigmaL1 = new RooRealVar(Form("%s_Landau1_sigma", name.c_str()),
				 Form("Single landau1 sigma for %s",name.c_str()),
				 0.1,0.,1.);
    Register(sigmaL1);

    auto landau1 = new RooLandau(Form("%s_Landau1_pdf",   name.c_str()),
				 Form("Landau1 Pdf for %s",name.c_str()),
				 x, *meanL1, *sigmaL1);
    Register(landau1);

    auto meanL2 = new RooRealVar(Form("%s_Landau2_mean",  name.c_str()),
				Form("Single landau2 mean for %s",name.c_str()),
				1.,0.,10.);
    Register(meanL2);
    
    auto sigmaL2 = new RooRealVar(Form("%s_Landau2_sigma", name.c_str()),
				 Form("Single landau2 sigma for %s",name.c_str()),
				 0.1,0.,1.);
    Register(sigmaL2);

    auto landau2 = new RooLandau(Form("%s_Landau2_pdf",   name.c_str()),
				 Form("Landau2 Pdf for %s",name.c_str()),
				 x, *meanL2, *sigmaL2);
    Register(landau2);

    // need a fraction
    auto frac = new RooRealVar(Form("%s_fraction",name.c_str()),
			       "Fractional amplitude of gaussian peak",
			       1.0,0.0,1.0);
    Register(frac);

    // add the two
    auto res = new RooAddPdf(Form("%s_LandauPlusGaus_pdf",name.c_str()),
			     Form("Sum of Landau and Gaussian %s",name.c_str()),
			     *landau1, *landau2, *frac);
    Register(res);
    
    return res;
  }


  RooAbsPdf* PdfFactory::LandauConvGauss(const std::string& name,
					 RooRealVar &x)
  {

    // Create Gaussian PDF
    auto meanG = new RooRealVar(Form("%s_Gaus_mean",  name.c_str()),
			       Form("Single gaussian mean for %s",name.c_str()),
			       1.,0.,10.);
    Register(meanG);

    auto sigmaG = new RooRealVar(Form("%s_Gaus_sigma", name.c_str()),
				Form("Single gaussian sigma for %s",name.c_str()),
				0.1,0.,1.);
    Register(sigmaG);

    auto gauss = new RooGaussian(Form("%s_Gaus_pdf",   name.c_str()),
				 Form("Gauss Pdf for %s",name.c_str()),
				 x, *meanG, *sigmaG);

    Register(gauss);

    // Create Landau PDF
    auto meanL = new RooRealVar(Form("%s_Landau_mean",  name.c_str()),
			       Form("Single landau mean for %s",name.c_str()),
			       1.,0.,10.);
    Register(meanL);

    auto sigmaL = new RooRealVar(Form("%s_Landau_sigma", name.c_str()),
				Form("Single landau sigma for %s",name.c_str()),
				0.1,0.,1.);
    Register(sigmaL);

    auto landau = new RooLandau(Form("%s_Landau_pdf",   name.c_str()),
				Form("Landau Pdf for %s",name.c_str()),
				x, *meanL, *sigmaL);

    Register(landau);

    auto res = new RooNumConvPdf("model", "model", x, *landau, *gauss);
    
    return res;
  }



  RooAbsPdf* PdfFactory::dEdxGaus(const std::string& name,
				  RooRealVar &x)
  {
    auto mean = new RooRealVar(Form("%s_dEdxGaus_mean",  name.c_str()),
			       Form("Single gaussian dE/dx mean [MeV/cm] for %s",name.c_str()),
			       2.,1.,3.);
    Register(mean);

    auto sigma = new RooRealVar(Form("%s_dEdxGaus_sigma", name.c_str()),
				Form("Single gaussian dE/dx sigma [MeV/cm] for %s",name.c_str()),
				0.6,0.,2.);
    Register(sigma);

    auto res = new RooGaussian(Form("%s_dEdxGaus_pdf",   name.c_str()),
			       Form("dE/dx Pdf for %s",name.c_str()),
			       x, *mean, *sigma);

    Register(res);

    return res;
  }

  RooAbsPdf* PdfFactory::dEdxDGaus(const std::string& name,
				   RooRealVar &x)
  {
    auto himean = new RooRealVar(Form("%s_dEdxGaus_mean_high",name.c_str()),
				 Form("Double gaussian dE/dx hi mean [MeV/cm] for %s",name.c_str()),
				 4.,1.,6.);
    Register(himean);
    auto hisigma = new RooRealVar(Form("%s_dEdxGaus_sigma_high",name.c_str()),
				  Form("Double gaussian dE/dx hi mean sigma [MeV/cm] for %s",name.c_str()),
				  1.,0.,2.);
    Register(hisigma);
    auto lowmean = new RooRealVar(Form("%s_dEdxGaus_mean_low",name.c_str()),
				  Form("Double gaussian dE/dx low mean [MeV/cm] for %s",name.c_str()),
				  4.,1.,6.);
    Register(lowmean);
    auto lowsigma = new RooRealVar(Form("%s_dEdxGaus_sigma_low",name.c_str()),
				   Form("Double gaussian dE/dx low mean sigma [MeV/cm] for %s",name.c_str()),
				   1.,0.,2.);
    Register(lowsigma);
    auto frac = new RooRealVar(Form("%s_dEdxGaus_fraction",name.c_str()),
			       "Fractional amplitude of a higher gaussian peak",
			       1.0,0.0,1.0);
    Register(frac);

    auto res = new RooAddPdf(Form("%s_dEdxDoubleGaus_pdf",name.c_str()),
			     Form("dE/dx Pdf for %s",name.c_str()),
			     *(new RooGaussian(Form("%s_dEdxPdf_lowGaus",name.c_str()),
					       Form("Lower peak gaussian for %s",name.c_str()),
					       x, *lowmean, *lowsigma)),
			     *(new RooGaussian(Form("%s_dEdxPdf_hiGaus",name.c_str()),
					       Form("Higher peak gaussian for %s",name.c_str()),
					   x, *himean, *hisigma)),
			     *frac);
    Register(res);
    return res;
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


  RooAbsPdf* PdfFactory::UniformDistrib(const std::string& name,
					RooRealVar &x)
  {
    return new RooPolynomial("uniform_Pdf","Uniform Distribution", x);
  }

}

#endif
