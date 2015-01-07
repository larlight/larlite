#ifndef SELECTIONTOOL_SPALGOEMPART_CXX
#define SELECTIONTOOL_SPALGOEMPART_CXX

#include "SPAlgoEMPart.h"

namespace sptool {

  SPAlgoEMPart::SPAlgoEMPart() 
    : SPAlgoBase()
    , _e_radLenPdf(nullptr)
    , _e_radLenData(nullptr)
    , _e_radLenVar(nullptr)
    , _e_radLenVal(nullptr)
    , _e_dEdxPdf(nullptr)
    , _e_dEdxData(nullptr)
    , _e_dEdxVar(nullptr)
    , _e_dEdxMu(nullptr)
    , _e_dEdxSigma(nullptr)
    , _g_radLenPdf(nullptr)
    , _g_radLenData(nullptr)
    , _g_radLenVar(nullptr)
    , _g_radLenVal(nullptr)
    , _g_dEdxPdf(nullptr)
    , _g_dEdxData(nullptr)
    , _g_dEdxVar(nullptr)
    , _g_dEdxFrac(nullptr)
    , _g_dEdxMu1(nullptr)
    , _g_dEdxSigma1(nullptr)
    , _g_dEdxMu2(nullptr)
    , _g_dEdxSigma2(nullptr)
  { 
    _name = "SPAlgoEMPart"; 
    Init();
  }

  void SPAlgoEMPart::Init()
  {
    _xmin = 0.;
    _xmax = 100.;
    _dedxmin = 0.;
    _dedxmax = 8.;
    _e_dedx_fitMin = _dedxmin;
    _e_dedx_fitMax = _dedxmax;
    _g_dedx_fitMin = _dedxmin;
    _g_dedx_fitMax = _dedxmax;
    _e_lval = -2000;
    _e_lmin = -10000;
    _e_lmax = -100;
    _e_dedxmu = 2.;
    _e_dedxsigma = _e_dedxmu*0.3;
    _e_dedxmin = _dedxmin;
    _e_dedxmax = _dedxmax;
    _g_lval = -1./20;
    _g_lmin = -1./10.;
    _g_lmax = -1./100.;
    _g_dedxfrac = 0.05;
    _g_dedxmu1 = 1.;
    _g_dedxsigma1 = _g_dedxmu1*0.2;
    _g_dedxmu2 = 2.;
    _g_dedxsigma2 = _g_dedxmu2*0.2;
    _g_dedxmin = _dedxmin;
    _g_dedxmax = _dedxmax;

  }

  void SPAlgoEMPart::LoadParams(std::string fname,size_t version)
  {
    // Load user_info
    SPTBase::LoadParams(fname,version);
    // Extract if parameters found
    if(_params.exist_darray("rad_range")) {
      auto darray = _params.get_darray("rad_range");
      _xmin = (*darray)[0];
      _xmax = (*darray)[1];
      std::cout<<"["<<__FUNCTION__<<"] "
	       << "Loaded distance fit range : "
	       << _xmin << " => " << _xmax
	       << std::endl;
    }
    if(_params.exist_darray("dedx_range")) {
      auto darray = _params.get_darray("dedx_range");
      _dedxmin = (*darray)[0];
      _dedxmax = (*darray)[1];
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded dEdx range : "
	       << _dedxmin << " => " << _dedxmax
		       << std::endl;
    }
    if(_params.exist_darray("gamma_params")) {
      auto darray = _params.get_darray("gamma_params");
      _g_lval = (*darray)[0];
      _g_lmin = (*darray)[1];
      _g_lmax = (*darray)[2];
      _g_dedxmu1 = (*darray)[3];
      _g_dedxmu2 = (*darray)[5];
      _g_dedxsigma1 = (*darray)[4];
      _g_dedxsigma2 = (*darray)[6];
      _g_dedxfrac   = (*darray)[7];
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded gamma parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length: " << _g_lval << " ["<<_g_lmin<<" => "<<_g_lmax<<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean1: " << _g_dedxmu1 << " Sigma1: " << _g_dedxsigma1 << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean2: " << _g_dedxmu2 << " Sigma2: " << _g_dedxsigma2 << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"Frac: " << _g_dedxfrac <<std::endl;
    }
    if(_params.exist_darray("electron_params")) {
      auto darray = _params.get_darray("electron_params");
      _e_lval = (*darray)[0];
      _e_lmin = (*darray)[1];
      _e_lmax = (*darray)[2];
      _e_dedxmu = (*darray)[3];
      _e_dedxsigma = (*darray)[4];
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded electron parameters... " << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length: " << _e_lval << " ["<<_e_lmin<<" => "<<_e_lmax<<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mu:" << _e_dedxmu << " Sigma: "<<_e_dedxsigma
	       <<std::endl;
    }
  }

  void SPAlgoEMPart::ProcessBegin()
  {
    ShowerPdfFactory factory;
    std::string part_name = "gamma";
    if(!_mode) part_name = "electron";

    // electron RadLen vars
    delete _e_radLenVal;
    _e_radLenVal   = new RooRealVar("_e_l","e Radiation length [cm]",_e_lval,_e_lmin,_e_lmax);
    delete _e_radLenVar;
    _e_radLenVar   = new RooRealVar("_e_rl","e Radiation length [cm]", _xmin, _xmax);
    // electron dEdx vars
    delete _e_dEdxMu;
    _e_dEdxMu    = new RooRealVar("_e_dEdxMu","e dEdx Mean",_e_dedxmu,_e_dedxmin,_e_dedxmax);
    delete _e_dEdxSigma;
    _e_dEdxSigma = new RooRealVar("_e_dEdxSigma","e dEdx Sigma",_e_dedxsigma,0.,_e_dedxmax);
    delete _e_dEdxVar;
    _e_dEdxVar = new RooRealVar("_e_dEdx","e dEdx", _e_dedxmin, _e_dedxmax);
    delete _e_radLenPdf;
    _e_radLenPdf = factory.RadLenPdf(*_e_radLenVar, *_e_radLenVal);
    delete _e_radLenData;
    _e_radLenData = new RooDataSet(Form("%s_radLenData",part_name.c_str()),"RooFit distance data set",RooArgSet(*_e_radLenVar));
    delete _e_dEdxPdf;
    _e_dEdxPdf = factory.dEdxPdf(*_e_dEdxVar, *_e_dEdxMu, *_e_dEdxSigma);
    delete _e_dEdxData;
    _e_dEdxData = new RooDataSet(Form("%s_dEdxData",part_name.c_str()),"RooFit dEdx set",RooArgSet(*_e_dEdxVar));

    // gamma RadLen vars
    delete _g_radLenVal;
    _g_radLenVal   = new RooRealVar("_g_l","g Radiation length [cm]",_g_lval,_g_lmin,_g_lmax);
    delete _g_radLenVar;
    _g_radLenVar   = new RooRealVar("_g_rl","g Radiation length [cm]", _xmin, _xmax);
    // gamma dEdx vars
    delete _g_dEdxFrac;
    _g_dEdxFrac = new RooRealVar("_g_dedxfrac","g dEdx frac",_g_dedxfrac,0.,1.0);
    delete _g_dEdxMu1;
    _g_dEdxMu1    = new RooRealVar("_g_dedxmu1","g dEdx Mean",_g_dedxmu1,_g_dedxmin,_g_dedxmax);
    delete _g_dEdxSigma1;
    _g_dEdxSigma1 = new RooRealVar("_g_dedxsigma1","g dEdx Sigma",_g_dedxsigma1,0.,_g_dedxmax);
    delete _g_dEdxMu2;
    _g_dEdxMu2    = new RooRealVar("_g_dedxmu2","g dEdx Mean",_g_dedxmu2,_g_dedxmin,_g_dedxmax);
    delete _g_dEdxSigma2;
    _g_dEdxSigma2 = new RooRealVar("_g_dedxsigma2","g dEdx Sigma",_g_dedxsigma2,0.,_g_dedxmax);
    delete _g_dEdxVar;
    _g_dEdxVar = new RooRealVar("_g_dEdx","g dEdx", _g_dedxmin, _g_dedxmax);
    delete _g_radLenPdf;
    _g_radLenPdf = factory.RadLenPdf(*_g_radLenVar, *_g_radLenVal);
    delete _g_radLenData;
    _g_radLenData = new RooDataSet(Form("%s_radLenData",part_name.c_str()),"RooFit distance data set",RooArgSet(*_g_radLenVar));
    delete _g_dEdxPdf;
    _g_dEdxPdf = factory.dEdxPdf_gamma(*_g_dEdxVar, *_g_dEdxFrac, *_g_dEdxMu1, *_g_dEdxSigma1, *_g_dEdxMu2, *_g_dEdxSigma2);
    
    delete _g_dEdxData;
    _g_dEdxData = new RooDataSet(Form("%s_dEdxData",part_name.c_str()),"RooFit dEdx set",RooArgSet(*_g_dEdxVar));

  }

  double SPAlgoEMPart::LL(bool is_electron, double dedx, double rad_length)
  {

    _g_dEdxVar->setVal(dedx);
    _e_dEdxVar->setVal(dedx);

    if(rad_length<0) {

      if(is_electron)

	return log(_e_dEdxPdf->getVal(*_e_dEdxVar)/ ( _g_dEdxPdf->getVal(*_g_dEdxVar) + _e_dEdxPdf->getVal(*_e_dEdxVar) ));

      else

	return log(_g_dEdxPdf->getVal(*_g_dEdxVar)/ ( _g_dEdxPdf->getVal(*_g_dEdxVar) + _e_dEdxPdf->getVal(*_e_dEdxVar) ));

    }else{

      _g_radLenVar->setVal(rad_length);
      _e_radLenVar->setVal(rad_length);
      
      if(is_electron)
	
	return log((_e_dEdxPdf->getVal(*_e_dEdxVar) * _e_radLenPdf->getVal(*_e_radLenVar) ) / 
		   ( _g_dEdxPdf->getVal(*_g_dEdxVar) * _g_radLenPdf->getVal(*_g_radLenVar) + 
		     _e_dEdxPdf->getVal(*_e_dEdxVar) * _e_radLenPdf->getVal(*_e_radLenVar) ));
      
      else
	
	return log((_g_dEdxPdf->getVal(*_g_dEdxVar) * _g_radLenPdf->getVal(*_g_radLenVar) ) / 
		   ( _g_dEdxPdf->getVal(*_g_dEdxVar) * _g_radLenPdf->getVal(*_g_radLenVar) + 
		     _e_dEdxPdf->getVal(*_e_dEdxVar) * _e_radLenPdf->getVal(*_e_radLenVar) ));
    }
  }

  SPArticleSet SPAlgoEMPart::Reconstruct(const SPAData &data)
  {
    SPArticleSet res;

    // No! -> Reconstructed quantities might not have vertex at this stage
    // LL function automatically takes care of case in which dist < 0 (only uses dEdx for LL calculation)
    // if(!data._vtxs.size()) return res;
    // David Caratelli

    for(auto const& s : data._showers) {

      double dist   = -1.;
      double dEdx   = s._dedx;

      // skip if dEdx out of bounds
      if ( (dEdx <= _dedxmin) or (dEdx >= _dedxmax) ) return res;

      if(data._vtxs.size())
	dist = s.Start().Dist(data._vtxs[0]);

      double e_like = LL(true,  dEdx, dist);
      double g_like = LL(false, dEdx, dist);

      SPArticle p;
      p.pdg_code( ( g_like > e_like ? 22 : 11 ) );

      p.pos(s.Start());
      p.mom(s.Dir() * s._energy);
      p.energy(s._energy);      
      res.push_back(p);

      if(_mode) {
	if(!(dist<0)) {
	  _g_radLenVar->setVal(dist);
	  _g_radLenData->add(RooArgSet(*_g_radLenVar));
	}
	_g_dEdxVar->setVal(dEdx);
	_g_dEdxData->add(RooArgSet(*_g_dEdxVar));
      }else {
	if(!(dist<0)) {
	  _e_radLenVar->setVal(dist);
	  _e_radLenData->add(RooArgSet(*_e_radLenVar));
	}
	_e_dEdxVar->setVal(dEdx);
	_e_dEdxData->add(RooArgSet(*_e_dEdxVar));
      }

    }
    
    return res;
  }

  void SPAlgoEMPart::ProcessEnd(TFile* fout)
  {

    TCanvas *c = new TCanvas("c","",1000,500);


    // Plot a bunch of stuff!
    RooPlot* frame_radLen = nullptr;
    RooPlot* frame_dEdx   = nullptr;
    RooFitResult* fit_res_radLen = nullptr;
    RooFitResult* fit_res_dEdx   = nullptr;
    std::string part_name = "gamma";
    if(!_mode) part_name = "electron";

    RooMsgService::instance().setSilentMode(true);
    
    if(_mode) {
      frame_radLen = _g_radLenVar->frame();
      frame_dEdx   = _g_dEdxVar->frame();
      _g_radLenData->plotOn(frame_radLen);
      _g_dEdxData->plotOn(frame_dEdx);
      // Fit only if in traning mode.
      if (_training_mode) {
	fit_res_radLen = _g_radLenPdf->fitTo(*_g_radLenData,RooFit::Save(),RooFit::PrintLevel(-1));
	std::cout << "Fit results for gamma radLength." << std::endl;
	fit_res_radLen->Print();
	fit_res_dEdx   = _g_dEdxPdf->fitTo(*_g_dEdxData, RooFit::Range(_g_dedx_fitMin, _g_dedx_fitMax), RooFit::Save(),RooFit::PrintLevel(-1));
	fit_res_dEdx->Print();
	_g_radLenPdf->plotOn(frame_radLen);
	_g_dEdxPdf->plotOn(frame_dEdx);
      }
      // if not overlay loaded fit results normalized to integral of events
      else{
	_g_dEdxPdf->plotOn(frame_dEdx);//,RooFit::Normalization(_g_dEdxData->sumEntries()));
      }
    }
    else{
      part_name = "electron";
      frame_radLen = _e_radLenVar->frame();
      frame_dEdx   = _e_dEdxVar->frame();
      _e_radLenData->plotOn(frame_radLen);
      _e_dEdxData->plotOn(frame_dEdx);
      // Fit only if in training mode
      if (_training_mode) {
	fit_res_radLen = _e_radLenPdf->fitTo(*_e_radLenData,RooFit::Save(),RooFit::PrintLevel(-1));
	fit_res_radLen->Print();
	fit_res_dEdx   = _e_dEdxPdf->fitTo(*_e_dEdxData, RooFit::Range(_e_dedx_fitMin, _e_dedx_fitMax), RooFit::Save(),RooFit::PrintLevel(-1));
	fit_res_dEdx->Print();
	_e_radLenPdf->plotOn(frame_radLen);
	_e_dEdxPdf->plotOn(frame_dEdx);
      }
      // if not overlay loaded fit results normalized to integral of events
      else {
	_e_dEdxPdf->plotOn(frame_dEdx);//,RooFit::Normalization(_e_dEdxData->sumEntries()));
      }
    }
    


    c->SetTitle(Form("Distance PDF for %s",part_name.c_str()));
    frame_radLen->Draw();
    c->SaveAs(Form("RadLength_%s.png",part_name.c_str()));

    c->SetTitle("Selection Likelihood");
    frame_dEdx->Draw();
    c->SaveAs(Form("dEdx_%s.png",part_name.c_str()));


    if (!_training_mode) return;

    RooRealVar* res_value_radLen = nullptr;
    RooRealVar* res_value_dEdxFrac = nullptr;
    RooRealVar* res_value_dEdxMu = nullptr;
    RooRealVar* res_value_dEdxSigma = nullptr;
    RooRealVar* res_value_dEdxMu1 = nullptr;
    RooRealVar* res_value_dEdxSigma1 = nullptr;
    RooRealVar* res_value_dEdxMu2 = nullptr;
    RooRealVar* res_value_dEdxSigma2 = nullptr;
    if(_mode){
      res_value_radLen = (RooRealVar*)(fit_res_radLen->floatParsFinal().find("_g_l"));
      res_value_dEdxFrac   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_g_dedxfrac"));
      res_value_dEdxMu1   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_g_dedxmu1"));
      res_value_dEdxSigma1   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_g_dedxsigma1"));
      res_value_dEdxMu2   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_g_dedxmu2"));
      res_value_dEdxSigma2   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_g_dedxsigma2"));
      std::cout << "["<<__FUNCTION__<<"] " << Form("Extracted %s_params... ",part_name.c_str()) << std::endl;
      std::cout << "["<<__FUNCTION__<<"] "
		<< "RadLen: "<< res_value_radLen->getVal() << " [" << res_value_radLen->getErrorLo() + res_value_radLen->getVal()
		<< " => " << res_value_radLen->getErrorHi() + res_value_radLen->getVal() << "]" << std::endl;
      std::cout << "["<<__FUNCTION__<<"] "
		<< "dEdx: Mu1: " << res_value_dEdxMu1->getVal() << " Sigma1: " << res_value_dEdxSigma1->getVal() << std::endl
		<< "["<<__FUNCTION__<<"] "
		<< "dEdx: Mu2: " << res_value_dEdxMu2->getVal() << " Sigma2: " << res_value_dEdxSigma2->getVal() << std::endl
		<< "["<<__FUNCTION__<<"] "
		<< "Frac: " << res_value_dEdxFrac->getVal() << std::endl;
      _params.append(Form("%s_params",part_name.c_str()),res_value_radLen->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_radLen->getVal()+res_value_radLen->getErrorLo());
      _params.append(Form("%s_params",part_name.c_str()),res_value_radLen->getVal()+res_value_radLen->getErrorHi());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxMu1->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxSigma1->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxMu2->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxSigma2->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxFrac->getVal());
    }
    else{
      res_value_radLen = (RooRealVar*)(fit_res_radLen->floatParsFinal().find("_e_l"));
      res_value_dEdxMu   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_e_dEdxMu"));
      res_value_dEdxSigma   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find("_e_dEdxSigma"));
      std::cout << "["<<__FUNCTION__<<"] " << Form("Extracted %s_params... ",part_name.c_str()) << std::endl;
      std::cout << "["<<__FUNCTION__<<"] "
		<< "RadLen: "<< res_value_radLen->getVal() << " [" << res_value_radLen->getErrorLo() + res_value_radLen->getVal()
		<< " => " << res_value_radLen->getErrorHi() + res_value_radLen->getVal() << "]" << std::endl;
      std::cout << "["<<__FUNCTION__<<"] "
		<< "dEdx: Mu: " << res_value_dEdxMu->getVal() << " Sigma: " << res_value_dEdxSigma->getVal() << std::endl;
      _params.append(Form("%s_params",part_name.c_str()),res_value_radLen->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_radLen->getVal()+res_value_radLen->getErrorLo());
      _params.append(Form("%s_params",part_name.c_str()),res_value_radLen->getVal()+res_value_radLen->getErrorHi());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxMu->getVal());
      _params.append(Form("%s_params",part_name.c_str()),res_value_dEdxSigma->getVal());
    }
    
    //_params.dump_contents();
    if(_params.exist_darray("rad_range"))
      _params.get_darray("rad_range")->clear();
    _params.append("rad_range",_xmin);
    _params.append("rad_range",_xmax);
    if(_params.exist_darray("dedx_range"))
      _params.get_darray("dedx_range")->clear();
    _params.append("dedx_range",_dedxmin);
    _params.append("dedx_range",_dedxmax);

    // Rad Length likelyhood
    TH1D *h11_radLen = new TH1D("h11_radLen","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",100,0,0.1);
    TH1D *h22_radLen = new TH1D("h22_radLen","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",100,0,0.1);
    
    for(size_t i=0; i<100; ++i) {
      _e_radLenVar->setVal(0.1*i/100.);
      _g_radLenVar->setVal(0.1*i/100.);

      h11_radLen->SetBinContent(i,_e_radLenPdf->getVal(*_e_radLenVar) / (_e_radLenPdf->getVal(*_e_radLenVar) + _g_radLenPdf->getVal(*_g_radLenVar)));
      h22_radLen->SetBinContent(i,_g_radLenPdf->getVal(*_g_radLenVar) / (_e_radLenPdf->getVal(*_e_radLenVar) + _g_radLenPdf->getVal(*_g_radLenVar)));
    }
    
    h11_radLen->SetLineWidth(2);
    h11_radLen->SetLineColor(kBlue);
    h11_radLen->SetFillStyle(3004);
    h11_radLen->SetFillColor(kBlue);
    h11_radLen->GetYaxis()->SetRangeUser(0.,1.);
    h22_radLen->SetLineWidth(2);
    h22_radLen->SetLineColor(kRed);
    h22_radLen->SetFillStyle(3003);
    h22_radLen->SetFillColor(kRed);
    h22_radLen->GetYaxis()->SetRangeUser(0.,1.);
    
    h11_radLen->Draw();
    h22_radLen->Draw("sames");
    c->SaveAs("Likelihood_radLen.png");


    TH1D *h11_dEdx = new TH1D("h11_dEdx","Electron vs. Gamma Likelihood; dEdx [MeV/cm]; Likelihood",100,0,8);
    TH1D *h22_dEdx = new TH1D("h22_dEdx","Electron vs. Gamma Likelihood; dEdx [MeV/cm]; Likelihood",100,0,8);
    
    for(size_t i=0; i<100; ++i) {
      _e_dEdxVar->setVal(8*i/100.);
      _g_dEdxVar->setVal(8*i/100.);

      h11_dEdx->SetBinContent(i,_e_dEdxPdf->getVal(*_e_dEdxVar) / (_e_dEdxPdf->getVal(*_e_dEdxVar) + _g_dEdxPdf->getVal(*_g_dEdxVar)));
      h22_dEdx->SetBinContent(i,_g_dEdxPdf->getVal(*_g_dEdxVar) / (_e_dEdxPdf->getVal(*_e_dEdxVar) + _g_dEdxPdf->getVal(*_g_dEdxVar)));
    }
    
    h11_dEdx->SetLineWidth(2);
    h11_dEdx->SetLineColor(kBlue);
    h11_dEdx->SetFillStyle(3004);
    h11_dEdx->SetFillColor(kBlue);
    h11_dEdx->GetYaxis()->SetRangeUser(0.,1.);
    h22_dEdx->SetLineWidth(2);
    h22_dEdx->SetLineColor(kRed);
    h22_dEdx->SetFillStyle(3003);
    h22_dEdx->SetFillColor(kRed);
    h22_dEdx->GetYaxis()->SetRangeUser(0.,1.);
    
    h11_dEdx->Draw();
    h22_dEdx->Draw("sames");
    c->SaveAs("Likelihood_dEdx.png");
    
    delete h11_dEdx;
    delete h22_dEdx;


    // for fun make a ProdPdf to plot 2D Pdf surface
    RooProdPdf ePDF("ePDF","ePDF",RooArgSet(*_e_radLenPdf,*_e_dEdxPdf));
    RooProdPdf gPDF("gPDF","gPDF",RooArgSet(*_g_radLenPdf,*_g_dEdxPdf));

    // data set
    //RooDataSet* data2D = gPDF.generate(RooArgSet(*_g_radLenVar,*_g_dEdxVar),10000);
    // Histo
    //TH1* hh_data = data2D->createHistogram("hh_data",*_g_radLenVar,RooFit::Binning(100,0,1000),
    //					   RooFit::YVar(*_g_dEdxVar,RooFit::Binning(100,0,8)) );

    TH1* h_2D = gPDF.createHistogram("gamma radLen vs. dEdx",*_g_radLenVar,RooFit::Binning(100,0,30),
				    RooFit::YVar(*_g_dEdxVar,RooFit::Binning(100,0,8)) ); 
    h_2D->Draw("SURF3");
    c->SaveAs("radLen_vs_dEdx_2DPDF_gamma.png");

    delete h_2D;

    h_2D = ePDF.createHistogram("electron radLen vs. dEdx",*_e_radLenVar,RooFit::Binning(100,0,0.002),
				    RooFit::YVar(*_e_dEdxVar,RooFit::Binning(100,0,8)) ); 
    h_2D->Draw("SURF3");
    c->SaveAs("radLen_vs_dEdx_2DPDF_electron.png");

    delete h_2D;
    delete c;

  
  }
  
}

#endif
