#ifndef ERTOOL_ALGOEMPART_CXX
#define ERTOOL_ALGOEMPART_CXX

#include "AlgoEMPart.h"

namespace ertool {

  AlgoEMPart::AlgoEMPart() 
    : AlgoBase()
    , _e_radLenData(nullptr)
    , _e_dEdxData(nullptr)
    , _g_radLenData(nullptr)
    , _g_dEdxData(nullptr)
  { 

    _name = "AlgoEMPart";

    _dEdxVar   = new RooRealVar("empart_dedx","dE/dx [MeV/cm] Variable",0.,50.);
    _radLenVar = new RooRealVar("empart_radlen","Radiation Length [cm] Variable",-10000.,0.);

    PdfFactory factory;

    std::string part_name;

    part_name = "electron";
    _e_dEdxPdf    = factory.dEdxGaus(part_name,*_dEdxVar);
    _e_radLenPdf  = factory.RadiationLength(part_name,*_radLenVar);

    part_name = "gamma";
    _g_dEdxPdf    = factory.dEdxDoubleGaus(part_name,*_dEdxVar);
    _g_radLenPdf  = factory.RadiationLength(part_name,*_radLenVar);

    SetDefaultParams();
  }

  void AlgoEMPart::SetFitRange_dEdx(double min, double max, bool electron)
  {
    if(electron)
      _e_dedx_fit_range.Set((max-min)/2.,min,max);
    else
      _g_dedx_fit_range.Set((max-min)/2.,min,max);
  }

  void AlgoEMPart::SetFitRange_RadLen(double min, double max, bool electron)
  {
    if(electron)
      _e_radlen_fit_range.Set((max-min)/2.,min,max);
    else
      _g_radlen_fit_range.Set((max-min)/2.,min,max);
  }

  void AlgoEMPart::SetDefaultParams()
  {

    RooRealVar *var;
    // Electron PDFs
    var = (RooRealVar*)(_e_radLenPdf->getVariables()->find("electron_radLen"));
    var->setVal   ( -2.e3        );
    var->setRange ( -1.e4, -1.e2 );

    var  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("electron_dEdxGaus_mean"));
    var->setVal   ( 2.0      );
    var->setRange ( 1.0, 3.0 );

    var = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("electron_dEdxGaus_sigma"));
    var->setVal   ( 1.0      );
    var->setRange ( 0.0, 2.0 );

    // Gamma PDFs
    var = (RooRealVar*)(_e_radLenPdf->getVariables()->find("gamma_radLen"));
    var->setVal   ( -0.05        );
    var->setRange ( -0.10, -0.01 );

    var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_hiMean"));
    var->setVal   ( 4.0      );
    var->setRange ( 3.0, 5.0 );

    var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_lowMean"));
    var->setVal   ( 2.0      );
    var->setRange ( 1.0, 3.0 );

    var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_hiSigma"));
    var->setVal   ( 1.0      );
    var->setRange ( 0.0, 2.0 );

    var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_lowSigma"));
    var->setVal   ( 1.0      );
    var->setRange ( 0.0, 2.0 );

  }

  void AlgoEMPart::LoadParams(std::string fname,size_t version)
  {
    // Load user_info
    Record::LoadParams(fname,version);
    // Extract if parameters found
    if(_params.exist_darray("rad_range")) {
      auto darray = _params.get_darray("rad_range");
      _dEdxVar->setRange( (*darray)[0], (*darray)[1] );
      std::cout<<"["<<__FUNCTION__<<"] "
	       << "Loaded distance fit range : "
	       << _dEdxVar->getMin() << " => " << _dEdxVar->getMax()
	       << std::endl;
    }

    if(_params.exist_darray("dedx_range")) {
      auto darray = _params.get_darray("dedx_range");
      _radLenVar->setRange( (*darray)[0], (*darray)[1] );
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded dEdx range : "
	       << _dEdxVar->getMin() << " => " << _dEdxVar->getMax()
	       << std::endl;
    }

    if(_params.exist_darray("gamma_params")) {
      auto darray = _params.get_darray("gamma_params");
      RooRealVar *tau, *mean1, *mean2, *sigma1, *sigma2, *frac;
      tau = (RooRealVar*)(_e_radLenPdf->getVariables()->find("gamma_radLen"));
      tau->setVal   ( (*darray)[0]               );
      tau->setRange ( (*darray)[1], (*darray)[2] );

      mean1  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_lowMean"));
      mean1->setVal  ( (*darray)[3] );
      sigma1 = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_lowSigma"));
      sigma1->setVal ( (*darray)[4] );

      mean2  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_hiMean"));
      mean2->setVal  ( (*darray)[5] );
      sigma2 = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_hiSigma"));
      sigma2->setVal ( (*darray)[6] );

      frac = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("gamma_dEdxGaus_Fraction"));
      frac->setVal ( (*darray)[7] );
      
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded gamma parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length: " << tau->getVal() << " ["<< tau->getMin() <<" => "<< gau->getMax() <<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean1: " << mean1->getVal() << " Sigma1: " << sigma1->getVal() << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean2: " << mean2->getVal() << " Sigma2: " << sigma2->getVal() << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"Frac: " << frac->getVal() <<std::endl;
    }

    if(_params.exist_darray("electron_params")) {
      auto darray = _params.get_darray("electron_params");
      RooRealVar *tau, *mean, *sigma;
      tau = (RooRealVar*)(_e_radLenPdf->getVariables()->find("electron_radLen"));
      tau->setVal   ( (*darray)[0]               );
      tau->setRange ( (*darray)[1], (*darray)[2] );

      mean  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("electron_dEdxGaus_lowMean"));
      mean->setVal  ( (*darray)[3] );
      sigma = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("electron_dEdxGaus_lowSigma"));
      sigma->setVal ( (*darray)[4] );

      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded electron parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length: " << tau->getVal() << " ["<< tau->getMin() <<" => "<< gau->getMax() <<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean1: " << mean1->getVal() << " Sigma1: " << sigma1->getVal() << std::endl;
    }
  }

  void AlgoEMPart::ProcessBegin()
  {

    std::string part_name;
    
    part_name = "electron";
    delete _e_dEdxData;
    delete _e_radLenData;
    _e_dEdxData   = new RooDataSet(Form("%s_dEdxData",part_name.c_str()),
				   "EMPart dE/dx data sample",
				   RooArgSet(*_dEdxVar));
    _e_radLenData = new RooDataSet(Form("%s_radLenData",part_name.c_str()),
				   "EMPart radiation length data sample",
				   RooArgSet(*_radLenVar));

    part_name = "gamma";
    delete _g_dEdxData;
    delete _g_radLenData;
    _g_dEdxData   = new RooDataSet(Form("%s_dEdxData",part_name.c_str()),
				   "EMPart dE/dx data sample",
				   RooArgSet(*_dEdxVar));
    _g_radLenData = new RooDataSet(Form("%s_radLenData",part_name.c_str()),
				   "EMPart radiation length data sample",
				   RooArgSet(*_radLenVar));
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
      if ( !_dEdxVar->inRange( dEdx, 0 ) ) continue;

      if(data.Vertex().size())
	dist = s->Start().Dist((*data.Vertex()[0]));
      
      double e_like = LL(true,  dEdx, dist);
      double g_like = LL(false, dEdx, dist);

      Particle p(( g_like > e_like ? 22 : 11 ) );
      p.Vertex(s->Start());
      p.Momentum(s->Dir() * (s->_energy - p.Mass()));
      p.RecoObjInfo(s->ID(),Particle::RecoObjType_t::kShower);
      res.push_back(p);

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
      std::string part_name   = "gamma";
      std::string part_letter = "g";
      if(!_mode){
	part_name  = "electron";
	part_letter = "e";
      }

      if(_mode) {
	fit_res_radLen = _g_radLenPdf->fitTo(*_g_radLenData,
					     RooFit::Range(_g_radlen_fit_range.Min(),_g_radlen_fit_range.Max()),
					     RooFit::Save(),
					     RooFit::PrintLevel(-1)
					     );
	if (_verbose) { fit_res_radLen->Print(); }
	fit_res_dEdx   = _g_dEdxPdf->fitTo(*_g_dEdxData, 
					   RooFit::Range(_g_dedx_fit_range.Min(), _g_dedx_fit_range.Max()), 
					   RooFit::Save(),
					   RooFit::PrintLevel(-1)
					   );
	if (_verbose) { fit_res_dEdx->Print(); }
      }
      else{
	fit_res_radLen = _e_radLenPdf->fitTo(*_e_radLenData,
					     RooFit::Range(_e_dedx_fit_range.Min(), _e_dedx_fit_range.Max()),
					     RooFit::Save(),
					     RooFit::PrintLevel(-1)
					     );
	if (_verbose) { fit_res_radLen->Print(); }
	fit_res_dEdx   = _e_dEdxPdf->fitTo(*_e_dEdxData, 
					   RooFit::Range(_e_dedx_fit_range.Min(), _e_dedx_fit_range.Max()), 
					   RooFit::Save(),
					   RooFit::PrintLevel(-1)
					   );
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
