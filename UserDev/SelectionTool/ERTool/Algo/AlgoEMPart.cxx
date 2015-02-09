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

    _dEdxVar   = new RooRealVar("empart_dedx","dE/dx [MeV/cm] Variable",0.,10.);
    _radLenVar = new RooRealVar("empart_radlen","Radiation Length [cm] Variable",0,100);

    _e_mass = TDatabasePDG().GetParticle(11)->Mass();
    _g_mass = TDatabasePDG().GetParticle(22)->Mass();

    //if training mode clear params
    //if (_training_mode)
    //  _params.clear_data();

    std::string part_name;

    //
    // Construct electron PDFs
    //
    part_name = "e";
    _e_dEdxPdf   = _factory.dEdxGaus(part_name,*_dEdxVar);
    _e_radLenPdf = _factory.RadiationLength(part_name,*_radLenVar);

    part_name = "g";
    _g_dEdxPdf   = _factory.dEdxDGaus(part_name,*_dEdxVar);
    _g_radLenPdf = _factory.RadiationLength(part_name,*_radLenVar);

    SetDefaultParams();
  }

  void AlgoEMPart::SetFitRange_dEdx(double min, double max, bool gamma)
  {
    if(gamma)
      _g_dedx_fit_range.Set(min+(max-min)/2.,min,max);
    else
      _e_dedx_fit_range.Set(min+(max-min)/2.,min,max);
  }

  void AlgoEMPart::SetFitRange_RadLen(double min, double max, bool gamma)
  {
    if(gamma)
      _g_radlen_fit_range.Set(min+(max-min)/2.,min,max);
    else
      _e_radlen_fit_range.Set(min+(max-min)/2.,min,max);
  }

  void AlgoEMPart::SetDefaultParams()
  {

    RooRealVar *var;

    // Fit range
    SetFitRange_RadLen(0,100,true);
    SetFitRange_RadLen(0,100,false);
    SetFitRange_dEdx(0,10,true);
    SetFitRange_dEdx(0,10,false);

    // Electron PDFs
    if (!_mode){
      var = (RooRealVar*)(_e_radLenPdf->getVariables()->find("e_radLen_tau"));
      var->setVal   ( -2.e3        );
      var->setRange ( -1.e4, -1.e2 );
      
      var  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_dEdxGaus_mean"));
      var->setVal   ( 2.0      );
      var->setRange ( 1.0, 3.0 );
      
      var = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_dEdxGaus_sigma"));
      var->setVal   ( 1.0      );
      var->setRange ( 0.0, 2.0 );
    }
    if (_mode){
      // Gamma PDFs
      var = (RooRealVar*)(_g_radLenPdf->getVariables()->find("g_radLen_tau"));
      var->setVal   ( -0.05        );
      var->setRange ( -0.10, -0.01 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_mean_high"));
      var->setVal   ( 4.0      );
      var->setRange ( 2.0, 6.0 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_mean_low"));
      var->setVal   ( 4.0      );
      var->setRange ( 1.0, 6.0 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_sigma_high"));
      var->setVal   ( 0.1      );
      var->setRange ( 0.0, 2.0 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_sigma_low"));
      var->setVal   ( 0.1      );
      var->setRange ( 0.0, 2.0 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_fraction"));
      var->setVal   ( 1.0      );
      var->setRange ( 0.0, 1.0 );
    }
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

    if(_params.exist_darray("g_params")) {
      auto darray = _params.get_darray("g_params");
      RooRealVar *tau, *meanlow, *meanhigh, *sigmalow, *sigmahigh, *frac;

      tau = (RooRealVar*)(_g_radLenPdf->getVariables()->find("g_radLen_tau"));
      tau->setVal   ( (*darray)[0]               );
      tau->setRange ( (*darray)[1], (*darray)[2] );

      meanhigh  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_mean_high"));
      meanhigh->setVal  ( (*darray)[3] );
      std::cout << "Mean high: " << meanhigh->getVal() << std::endl;
      sigmahigh = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_sigma_high"));
      sigmahigh->setVal ( (*darray)[4] );
      std::cout << "Sigma high: " << sigmahigh->getVal() << std::endl;

      meanlow  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_mean_low"));
      meanlow->setVal  ( (*darray)[5] );
      std::cout << "Mean low: " << meanlow->getVal() << std::endl;
      sigmalow = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_sigma_low"));
      sigmalow->setVal ( (*darray)[6] );
      std::cout << "Sigma low: " << sigmalow->getVal() << std::endl;

      frac = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_dEdxGaus_fraction"));
      frac->setVal ( (*darray)[7] );
      
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded gamma parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length: " << tau->getVal() << " ["<< tau->getMin() <<" => "<< tau->getMax() <<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean low: " << meanlow->getVal() << " Sigma high: " << sigmalow->getVal() << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean high: " << meanhigh->getVal() << " Sigma high: " << sigmahigh->getVal() << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"Frac: " << frac->getVal() <<std::endl;
    }

    if(_params.exist_darray("e_params")) {
      auto darray = _params.get_darray("e_params");
      RooRealVar *tau, *mean, *sigma;
      tau = (RooRealVar*)(_e_radLenPdf->getVariables()->find("e_radLen_tau"));
      tau->setVal   ( (*darray)[0]               );
      tau->setRange ( (*darray)[1], (*darray)[2] );

      mean  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_dEdxGaus_mean"));
      mean->setVal  ( (*darray)[3] );
      sigma = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_dEdxGaus_sigma"));
      sigma->setVal ( (*darray)[4] );

      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded electron parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length: " << tau->getVal() << " ["<< tau->getMin() <<" => "<< tau->getMax() <<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx: Mean1: " << mean->getVal() << " Sigma1: " << sigma->getVal() << std::endl;
    }
  }

  void AlgoEMPart::ProcessBegin()
  {
    std::string part_letter;
    
    part_letter = "e";
    delete _e_dEdxData;
    delete _e_radLenData;
    _e_dEdxData   = new RooDataSet(Form("%s_dEdxData",part_letter.c_str()),
				   "EMPart dE/dx data sample",
				   RooArgSet(*_dEdxVar));
    _e_radLenData = new RooDataSet(Form("%s_radLenData",part_letter.c_str()),
				   "EMPart radiation length data sample",
				   RooArgSet(*_radLenVar));

    part_letter = "g";
    delete _g_dEdxData;
    delete _g_radLenData;
    _g_dEdxData   = new RooDataSet(Form("%s_dEdxData",part_letter.c_str()),
				   "EMPart dE/dx data sample",
				   RooArgSet(*_dEdxVar));
    _g_radLenData = new RooDataSet(Form("%s_radLenData",part_letter.c_str()),
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

      int pdg_code = ( g_like > e_like ? 22 : 11 );
      double mass  = ( pdg_code == 11 ? _e_mass : _g_mass );
      Particle p(pdg_code,mass);
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

    std::string part_letter = "g";
    if(!_mode) part_letter = "e";
    
    if (_training_mode){
      // Fit and maybe save parameters

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
					     RooFit::Range(_e_radlen_fit_range.Min(), _e_radlen_fit_range.Max()),
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
      
      if (!_mode){
	RooRealVar *tau, *mean, *sigma;
	std::cout << "["<<__FUNCTION__<<"] " << Form("Extracted %s_params... ",part_letter.c_str()) << std::endl;
	// radLen
	tau = (RooRealVar*)(fit_res_radLen->floatParsFinal().find(Form("%s_radLen_tau",part_letter.c_str())));
	_params.append(Form("%s_params",part_letter.c_str()),tau->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),tau->getVal()+tau->getErrorLo());
	_params.append(Form("%s_params",part_letter.c_str()),tau->getVal()+tau->getErrorHi());
	std::cout << "["<<__FUNCTION__<<"] "
		  << "RadLen: "<< tau->getVal() << " [" << tau->getErrorLo() + tau->getVal()
		  << " => " << tau->getErrorHi() + tau->getVal() << "]" << std::endl;
	// dEdx
	mean  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_mean",part_letter.c_str())));
	sigma = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_sigma",part_letter.c_str())));
	_params.append(Form("%s_params",part_letter.c_str()),mean->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),sigma->getVal());
	std::cout << "["<<__FUNCTION__<<"] "
		  << "dEdx: Mean: " << mean->getVal() << " Sigma: " << sigma->getVal() << std::endl;
      }
      if (_mode){
	RooRealVar *tau, *meanhigh, *sigmahigh, *meanlow, *sigmalow;
	// in case od double-gaussian
	tau = (RooRealVar*)(fit_res_radLen->floatParsFinal().find(Form("%s_radLen_tau",part_letter.c_str())));
	meanhigh  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_mean_high",part_letter.c_str())));
	sigmahigh = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_sigma_high",part_letter.c_str())));
	meanlow  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_mean_low",part_letter.c_str())));
	sigmalow = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_sigma_low",part_letter.c_str())));
	RooRealVar* frac = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_dEdxGaus_fraction",part_letter.c_str())));
	_params.append(Form("%s_params",part_letter.c_str()),tau->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),tau->getVal()+tau->getErrorLo());
	_params.append(Form("%s_params",part_letter.c_str()),tau->getVal()+tau->getErrorHi());
	_params.append(Form("%s_params",part_letter.c_str()),meanhigh->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),sigmahigh->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),meanlow->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),sigmalow->getVal());
	_params.append(Form("%s_params",part_letter.c_str()),frac->getVal());
	std::cout << "["<<__FUNCTION__<<"] "
		  << "RadLen: "<< tau->getVal() << " [" << tau->getErrorLo() + tau->getVal()
		  << " => " << tau->getErrorHi() + tau->getVal() << "]" << std::endl;
	std::cout << "["<<__FUNCTION__<<"] "
		  << "dEdx: High Mean: " << meanhigh->getVal() << " Sigma: " << sigmahigh->getVal() << std::endl;
	std::cout << "["<<__FUNCTION__<<"] "
		  << "dEdx: Low Mean: " << meanlow->getVal() 
		  << " Low Sigma: "     << sigmalow->getVal() 
		  << " ... Fraction = " << 1. - frac->getVal() << std::endl;
      }
      
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
      c->SaveAs(Form("RadLength_Selected_%s.png", part_letter.c_str()));
      c->SetTitle("dEdx Selection");
      frame_dEdx->Draw();
      c->SaveAs(Form("dEdx_Selected_%s.png", part_letter.c_str()));

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
