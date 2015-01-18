#ifndef ERTOOL_ALGOEMPART_CXX
#define ERTOOL_ALGOEMPART_CXX

#include "AlgoEMPart.h"

namespace ertool {

  AlgoEMPart::AlgoEMPart() 
    : AlgoBase()
    , _dEdxVar(nullptr)
    , _radLenVar(nullptr)
    , _e_radLenPdf(nullptr)
    , _e_radLenData(nullptr)
    , _e_radLenVal(nullptr)
    , _e_dEdxPdf(nullptr)
    , _e_dEdxData(nullptr)
    , _e_dEdxMu(nullptr)
    , _e_dEdxSigma(nullptr)
    , _g_radLenPdf(nullptr)
    , _g_radLenData(nullptr)
    , _g_radLenVal(nullptr)
    , _g_dEdxPdf(nullptr)
    , _g_dEdxData(nullptr)
    , _g_dEdxMu(nullptr)
    , _g_dEdxSigma(nullptr)
  { 
    _name = "AlgoEMPart"; 
    Init();
    _verbose = false;
  }

  void AlgoEMPart::Init()
  {
    _xmin = 0.;
    _xmax = 100.;
    _dedxmin = 0.01;
    _dedxmax = 20.;
    _e_dedx_fitMin = _dedxmin;
    _e_dedx_fitMax = _dedxmax;
    _g_dedx_fitMin = _dedxmin;
    _g_dedx_fitMax = _dedxmax;
    _e_lval = -2000;
    _e_lmin = -10000;
    _e_lmax = -100;
    _e_dedxmu = 2.;
    _e_dedxsigma = _e_dedxmu*0.1;
    _e_dedxmin = _dedxmin;
    _e_dedxmax = _dedxmax;
    _g_lval = -1./20;
    _g_lmin = -1./10.;
    _g_lmax = -1./100.;
    _g_dedxmu = 4.;
    _g_dedxsigma = _g_dedxmu*0.1;
    _g_dedxmin = _dedxmin;
    _g_dedxmax = _dedxmax;
  }

  void AlgoEMPart::LoadParams(std::string fname,size_t version)
  {
    // Load user_info
    Record::LoadParams(fname,version);
    // Extract if parameters found
    if(_params.exist_darray("rad_range")) {
      auto darray = _params.get_darray("rad_range");
      _xmin = (*darray)[0];
      _xmax = (*darray)[1];
      if (_verbose) { std::cout<<"["<<__FUNCTION__<<"] Loaded distance fit range : " << _xmin << " => " << _xmax << std::endl; }
    }
    if(_params.exist_darray("dedx_range")) {
      auto darray = _params.get_darray("dedx_range");
      _dedxmin = (*darray)[0];
      _dedxmax = (*darray)[1];
      if (_verbose) { std::cout<<"["<<__FUNCTION__<<"] Loaded dEdx range : " << _dedxmin << " => " << _dedxmax << std::endl; }
    }
    if(_params.exist_darray("gamma_params")) {
      auto darray = _params.get_darray("gamma_params");
      _g_lval = (*darray)[0];
      _g_lmin = (*darray)[1];
      _g_lmax = (*darray)[2];
      _g_dedxmu = (*darray)[3];
      _g_dedxsigma = (*darray)[4];
      if (_verbose) {
	std::cout<<"["<<__FUNCTION__<<"] Loaded gamma parameters..." << std::endl;
	std::cout<<"["<<__FUNCTION__<<"] Rad Length: " << _g_lval << " ["<<_g_lmin<<" => "<<_g_lmax<<"]" << std:: endl;
	std::cout<<"["<<__FUNCTION__<<"] dEdx: Mean: " << _g_dedxmu << " Sigma: " << _g_dedxsigma << std::endl;
      }
    }
    if(_params.exist_darray("electron_params")) {
      auto darray = _params.get_darray("electron_params");
      _e_lval = (*darray)[0];
      _e_lmin = (*darray)[1];
      _e_lmax = (*darray)[2];
      _e_dedxmu = (*darray)[3];
      _e_dedxsigma = (*darray)[4];
      if (_verbose) {
	std::cout<<"["<<__FUNCTION__<<"] Loaded electron parameters... " << std::endl;
	std::cout<<"["<<__FUNCTION__<<"] Rad Length: " << _e_lval << " ["<<_e_lmin<<" => "<<_e_lmax<<"]" << std:: endl;
	std::cout<<"["<<__FUNCTION__<<"] dEdx: Mu:" << _e_dedxmu << " Sigma: "<<_e_dedxsigma << std::endl;
      }
    }
  }

  void AlgoEMPart::ProcessBegin()
  {
    PdfFactory factory;
    std::string part_name = "gamma";
    if(!_mode) part_name = "electron";

    // Variables used: dEdx & radLen
    delete _dEdxVar;
    _dEdxVar = new RooRealVar("_dEdx","dEdx", _dedxmin, _dedxmax);
    delete _radLenVar;
    _radLenVar   = new RooRealVar("_rl","Radiation length [cm]", _xmin, _xmax);


    // electron RadLen Stuff
    delete _e_radLenVal;
    _e_radLenVal   = new RooRealVar("_e_l","e Radiation length [cm]",_e_lval,_e_lmin,_e_lmax);
    delete _e_radLenPdf;
    _e_radLenPdf = factory.RadLenPdf(*_radLenVar, *_e_radLenVal);
    delete _e_radLenData;
    _e_radLenData = new RooDataSet(Form("%s_radLenData",part_name.c_str()),"RooFit distance data set",RooArgSet(*_radLenVar));
    // electron dEdx stuff
    delete _e_dEdxMu;
    _e_dEdxMu    = new RooRealVar("_e_dEdxMu","e dEdx Mean",_e_dedxmu,_e_dedxmin,_e_dedxmax);
    delete _e_dEdxSigma;
    _e_dEdxSigma = new RooRealVar("_e_dEdxSigma","e dEdx Sigma",_e_dedxsigma,0.,_e_dedxmax);
    delete _e_dEdxPdf;
    _e_dEdxPdf = factory.dEdxPdf(*_dEdxVar, *_e_dEdxMu, *_e_dEdxSigma);
    delete _e_dEdxData;
    _e_dEdxData = new RooDataSet(Form("%s_dEdxData",part_name.c_str()),"RooFit dEdx set",RooArgSet(*_dEdxVar));
    // gamma RadLen stuff
    delete _g_radLenVal;
    _g_radLenVal   = new RooRealVar("_g_l","g Radiation length [cm]",_g_lval,_g_lmin,_g_lmax);
    delete _g_radLenPdf;
    _g_radLenPdf = factory.RadLenPdf(*_radLenVar, *_g_radLenVal);
    delete _g_radLenData;
    _g_radLenData = new RooDataSet(Form("%s_radLenData",part_name.c_str()),"RooFit distance data set",RooArgSet(*_radLenVar));
    // gamma dEdx stuff
    delete _g_dEdxMu;
    _g_dEdxMu    = new RooRealVar("_g_dEdxMu","g dEdx Mean",_g_dedxmu,_g_dedxmin,_g_dedxmax);
    delete _g_dEdxSigma;
    _g_dEdxSigma = new RooRealVar("_g_dEdxSigma","g dEdx Sigma",_g_dedxsigma,0.,_g_dedxmax);
    delete _g_dEdxPdf;
    _g_dEdxPdf = factory.dEdxPdf(*_dEdxVar, *_g_dEdxMu, *_g_dEdxSigma);
    delete _g_dEdxData;
    _g_dEdxData = new RooDataSet(Form("%s_dEdxData",part_name.c_str()),"RooFit dEdx set",RooArgSet(*_dEdxVar));

  }

  double AlgoEMPart::LL(bool is_electron, double dedx, double rad_length)
  {

    _dEdxVar->setVal(dedx);

    if(rad_length<0) {

      if(is_electron)
	
	return log(_e_dEdxPdf->getVal(*_dEdxVar)/ ( _g_dEdxPdf->getVal(*_dEdxVar) + _e_dEdxPdf->getVal(*_dEdxVar) ));

      else

	return log(_g_dEdxPdf->getVal(*_dEdxVar)/ ( _g_dEdxPdf->getVal(*_dEdxVar) + _e_dEdxPdf->getVal(*_dEdxVar) ));

    }else{

      _radLenVar->setVal(rad_length);
      
      if(is_electron)
	
	return log((_e_dEdxPdf->getVal(*_dEdxVar) * _e_radLenPdf->getVal(*_radLenVar) ) / 
		   ( _g_dEdxPdf->getVal(*_dEdxVar) * _g_radLenPdf->getVal(*_radLenVar) + 
		     _e_dEdxPdf->getVal(*_dEdxVar) * _e_radLenPdf->getVal(*_radLenVar) ));
      
      else
	
	return log((_g_dEdxPdf->getVal(*_dEdxVar) * _g_radLenPdf->getVal(*_radLenVar) ) / 
		   ( _g_dEdxPdf->getVal(*_dEdxVar) * _g_radLenPdf->getVal(*_radLenVar) + 
		     _e_dEdxPdf->getVal(*_dEdxVar) * _e_radLenPdf->getVal(*_radLenVar) ));
    }
  }

  ParticleSet AlgoEMPart::Reconstruct(const EventData &data)
  {
    ParticleSet res;

    for(auto const& s : data.Shower()) {

      double dist   = -1.;
      double dEdx   = s->_dedx;

      // skip if dEdx out of bounds
      if ( (dEdx <= _dedxmin) or (dEdx >= _dedxmax) ) return res;

      if(data.Vertex().size())
	dist = s->Start().Dist((*data.Vertex()[0]));

      double e_like = LL(true,  dEdx, dist);
      double g_like = LL(false, dEdx, dist);

      /*
      Particle p(( g_like > e_like ? 22 : 11 ) );
      p.Vertex(s->Start());
      p.Momentum(s->Dir() * (s->_energy - p.Mass()));
      p.RecoObjInfo(s->ID(),Particle::RecoObjType_t::kShower);
      res.push_back(p);
      */

      _dEdxVar->setVal(dEdx);
      if (!(dist<0)) { _radLenVar->setVal(dist); }

      // If in training mode, fill RooDataSet
      // of the particle we are training
      // If in Selection mode, fill RooDataSet
      // of particle that is more likely
      if (!_training_mode){

	if ( g_like > e_like ){
	  if(!(dist<0)) { _g_radLenData->add(RooArgSet(*_radLenVar)); }
	  _g_dEdxData->add(RooArgSet(*_dEdxVar));
	}
	else{
	  if(!(dist<0)) { _e_radLenData->add(RooArgSet(*_radLenVar)); }
	  _e_dEdxData->add(RooArgSet(*_dEdxVar));
	}
      }// if selection mode

      else{
	if(_mode) {
	  if(!(dist<0)) { _g_radLenData->add(RooArgSet(*_radLenVar)); }
	  _g_dEdxData->add(RooArgSet(*_dEdxVar));
	}else {
	  if(!(dist<0)) { _e_radLenData->add(RooArgSet(*_radLenVar)); }
	  _e_dEdxData->add(RooArgSet(*_dEdxVar));
	}
      }// if training mode

    }// for all showers
    
    return res;
  }

  void AlgoEMPart::ProcessEnd(TFile* fout)
  {



    RooMsgService::instance().setSilentMode(true);

    // if in training mode make a plot of dEdx & radLen
    // for particle being trained
    // if in selection mode make plots for both electrons
    // and gammas, with the RooDataSets of both that were
    // produced during the selection process.
    
    RooFitResult* fit_res_radLen = nullptr;
    RooFitResult* fit_res_dEdx   = nullptr;
    
    if (_training_mode){

      // Fit and maybe save parameters
      std::string part_name = "gamma";
      std::string part_letter = "g";
      if(!_mode){
	part_name  = "electron";
	part_letter = "e";
      }

      if(_mode) {
	fit_res_radLen = _g_radLenPdf->fitTo(*_g_radLenData,RooFit::Save(),RooFit::PrintLevel(-1));
	if (_verbose) { fit_res_radLen->Print(); }
	fit_res_dEdx   = _g_dEdxPdf->fitTo(*_g_dEdxData, RooFit::Range(_g_dedx_fitMin, _g_dedx_fitMax), RooFit::Save(),RooFit::PrintLevel(-1));
	if (_verbose) { fit_res_dEdx->Print(); }
      }
      else{
	part_name = "electron";
	fit_res_radLen = _e_radLenPdf->fitTo(*_e_radLenData,RooFit::Save(),RooFit::PrintLevel(-1));
	if (_verbose) { fit_res_radLen->Print(); }
	fit_res_dEdx   = _e_dEdxPdf->fitTo(*_e_dEdxData, RooFit::Range(_e_dedx_fitMin, _e_dedx_fitMax), RooFit::Save(),RooFit::PrintLevel(-1));
	if (_verbose) { fit_res_dEdx->Print(); }

      }

      // Save parameters
      RooRealVar* res_value_radLen = nullptr;
      RooRealVar* res_value_dEdxMu = nullptr;
      RooRealVar* res_value_dEdxSigma = nullptr;
      res_value_radLen = (RooRealVar*)(fit_res_radLen->floatParsFinal().find(Form("_%s_l",part_letter.c_str())));
      res_value_dEdxMu   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("_%s_dEdxMu",part_letter.c_str())));
      res_value_dEdxSigma   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("_%s_dEdxSigma",part_letter.c_str())));
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



    }// if in traning mode
      
    // Plot the likelyhoods if in verbose mode
    if (_verbose){

      TCanvas *c = new TCanvas("c","",1000,500);
      
      // Rad Length likelyhood
      TH1D *h11_radLen = new TH1D("h11_radLen","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",100,0,0.1);
      TH1D *h22_radLen = new TH1D("h22_radLen","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",100,0,0.1);
      
      for(size_t i=0; i<100; ++i) {
	_radLenVar->setVal(0.1*i/100.);
	
	h11_radLen->SetBinContent(i,_e_radLenPdf->getVal(*_radLenVar)/(_e_radLenPdf->getVal(*_radLenVar)+_g_radLenPdf->getVal(*_radLenVar)));
	h22_radLen->SetBinContent(i,_g_radLenPdf->getVal(*_radLenVar)/(_e_radLenPdf->getVal(*_radLenVar)+_g_radLenPdf->getVal(*_radLenVar)));
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
	_dEdxVar->setVal(8*i/100.);
	
	h11_dEdx->SetBinContent(i,_e_dEdxPdf->getVal(*_dEdxVar) / (_e_dEdxPdf->getVal(*_dEdxVar) + _g_dEdxPdf->getVal(*_dEdxVar)));
	h22_dEdx->SetBinContent(i,_g_dEdxPdf->getVal(*_dEdxVar) / (_e_dEdxPdf->getVal(*_dEdxVar) + _g_dEdxPdf->getVal(*_dEdxVar)));
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
      TH1* h_2D = gPDF.createHistogram("gamma radLen vs. dEdx",*_radLenVar,RooFit::Binning(100,0,30),
				       RooFit::YVar(*_dEdxVar,RooFit::Binning(100,0,8)) ); 
      h_2D->Draw("SURF3");
      c->SaveAs("radLen_vs_dEdx_2DPDF_gamma.png");
      delete h_2D;
      h_2D = ePDF.createHistogram("electron radLen vs. dEdx",*_radLenVar,RooFit::Binning(100,0,0.002),
				  RooFit::YVar(*_dEdxVar,RooFit::Binning(100,0,8)) ); 
      h_2D->Draw("SURF3");
      c->SaveAs("radLen_vs_dEdx_2DPDF_electron.png");
      delete h_2D;
      
      
      // Plot a bunch of stuff!
      RooPlot* frame_radLen = nullptr;
      RooPlot* frame_dEdx   = nullptr;
      
      frame_radLen = _radLenVar->frame();
      frame_dEdx   = _dEdxVar->frame();
      
      _g_radLenData->plotOn(frame_radLen,RooFit::MarkerColor(kRed),RooFit::LineColor(kRed));
      _g_dEdxData->plotOn(frame_dEdx,RooFit::MarkerColor(kRed),RooFit::LineColor(kRed));
      _g_dEdxPdf->plotOn(frame_dEdx,RooFit::LineColor(kRed));
      _g_radLenPdf->plotOn(frame_radLen,RooFit::LineColor(kRed));

      _e_radLenData->plotOn(frame_radLen,RooFit::MarkerColor(kBlue),RooFit::LineColor(kBlue));
      _e_dEdxData->plotOn(frame_dEdx,RooFit::MarkerColor(kBlue),RooFit::LineColor(kBlue));
      _e_dEdxPdf->plotOn(frame_dEdx,RooFit::LineColor(kBlue));
      _e_radLenPdf->plotOn(frame_radLen,RooFit::LineColor(kBlue));

      c->SetTitle("Distance PDF");
      frame_radLen->Draw();
      c->SaveAs("RadLength_Selected.png");
      c->SetTitle("dEdx Selection");
      frame_dEdx->Draw();
      c->SaveAs("dEdx_Selected.png");

      delete c;
	
    } // if verbose then plot the likelyhoods
    
    // Print How many of each kind were found
    if (_verbose) {
      std::cout << "******************************************" << std::endl
		<< "Total Showers scanned            : " << _e_dEdxData->numEntries() + _g_dEdxData->numEntries() << std::endl
		<< "Total Electron Showers identified: " << _e_dEdxData->numEntries() << std::endl
		<< "Total Photon Showers identified  : " << _g_dEdxData->numEntries() << std::endl
		<< "******************************************" << std::endl;
    }
    
  }
  
}

#endif
