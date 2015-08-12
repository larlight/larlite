#ifndef ERTOOL_ALGOEMPART_CXX
#define ERTOOL_ALGOEMPART_CXX

#include "AlgoEMPart.h"

namespace ertool {

  AlgoEMPart::AlgoEMPart(const std::string& name) 
    : AlgoBase(name)
    , _e_radLenData(nullptr)
    , _e_dEdxData(nullptr)
    , _g_radLenData(nullptr)
    , _g_dEdxData(nullptr)
  { 
    _dEdxVar   = new RooRealVar("empart_dedx","dE/dx [MeV/cm] Variable",0.,10.);
    _radLenVar = new RooRealVar("empart_radlen","Radiation Length [cm] Variable",0,100);

    _e_mass = ParticleMass(11);
    _g_mass = ParticleMass(22);

    srand (time(NULL));

    _hradLen_e = new TH1D("_hradLen_e","rad len electrons",100,0,100);
    _hradLen_g = new TH1D("_hradLen_g","rad len gammas",100,0,100);

    _hdEdx_e = new TH1D("_hdEdx_e","dedx electrons",100,0,10);
    _hdEdx_g = new TH1D("_hdEdx_g","dedx gammas",100,0,10);

    // default plot mode is false
    _plot = false;
    // default setting for loading params
    _loadParams = true;

    std::string part_name;

    //
    // Construct electron PDFs
    //
    part_name = "e";
    //_e_dEdxPdf   = _factory.Landau(part_name,*_dEdxVar);
    //_e_dEdxPdf   = _factory.LandauConvGauss(part_name,*_dEdxVar);
    _e_dEdxPdf   = _factory.LandauPlusGauss(part_name,*_dEdxVar);
    _e_radLenPdf = _factory.RadiationLength(part_name,*_radLenVar);

    part_name = "g";
    //_g_dEdxPdf   = _factory.LandauConvGauss(part_name,*_dEdxVar);
    _g_dEdxPdf   = _factory.LandauPlusGauss(part_name,*_dEdxVar);
    //_g_dEdxPdf   = _factory.LandauPlusLandau(part_name,*_dEdxVar);
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
      var->setVal   ( -2        );
      var->setRange ( -10, -0.1 );
      
      var  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Landau_mean"));
      var->setVal   ( 2.0      );
      var->setRange ( 1.0, 3.0 );
      
      var = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Landau_sigma"));
      var->setVal   ( 0.12     );
      var->setRange ( 0.0, 1.0 );

      var  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Gaus_mean"));
      var->setVal   ( 3.0      );
      var->setRange ( 1.0, 4.0 );
      
      var = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Gaus_sigma"));
      var->setVal   ( 3.0     );
      var->setRange ( 0.5, 5.0 );

      var = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_fraction"));
      var->setVal   ( 0.05     );
      var->setRange ( 0.0, 1.0 );
    }
    if (_mode){
      // Gamma PDFs
      var = (RooRealVar*)(_g_radLenPdf->getVariables()->find("g_radLen_tau"));
      var->setVal   ( -0.05        );
      var->setRange ( -0.10, -0.01 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Landau_mean"));
      var->setVal   ( 4.0      );
      var->setRange ( 3.0, 5.0 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Landau_sigma"));
      var->setVal   ( 0.23     );
      var->setRange ( 0.0, 1.0 );

      var  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Gaus_mean"));
      var->setVal   ( 2.0      );
      var->setRange ( 1.0, 2.0 );
      
      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Gaus_sigma"));
      var->setVal   ( 0.20     );
      var->setRange ( 0.0, 0.8 );

      var = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_fraction"));
      var->setVal   ( 0.20     );
      var->setRange ( 0.0, 1.0 );
      
    } 
  }

  void AlgoEMPart::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    
    if (!_loadParams)
      return;
    
    auto p = cfg.get_pset(Name());
    
    // Extract if parameters found
    if(p.contains_value("rad_range")) {
      auto darray = p.get<std::vector<double> >("rad_range");
      _radLenVar->setRange( darray[0], darray[1] );
      std::cout<<"["<<__FUNCTION__<<"] "
	       << "Loaded distance fit range : "
	       << _radLenVar->getMin() << " => " << _radLenVar->getMax()
	       << std::endl;
    }

    if(p.contains_value("dedx_range")) {
      auto darray = p.get<std::vector<double> >("dedx_range");
      _dEdxVar->setRange( darray[0], darray[1] );
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded dEdx range : "
	       << _dEdxVar->getMin() << " => " << _dEdxVar->getMax()
	       << std::endl;
    }

    if(p.contains_value("g_params")) {
      auto darray = p.get<std::vector<double> >("g_params");
      RooRealVar *tau, *meanL, *sigmaL, *meanG, *sigmaG, *frac;

      tau = (RooRealVar*)(_g_radLenPdf->getVariables()->find("g_radLen_tau"));
      tau->setVal   ( darray[0]               );
      tau->setRange ( darray[1], darray[2] );

      meanL  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Landau_mean"));
      meanL->setVal  ( darray[3] );
      sigmaL = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Landau_sigma"));
      sigmaL->setVal ( darray[4] );

      meanG  = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Gaus_mean"));
      meanG->setVal  ( darray[5] );
      sigmaG = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_Gaus_sigma"));
      sigmaG->setVal ( darray[6] );

      frac = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_fraction"));
      frac->setVal ( darray[7] );
      
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded gamma parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length       : " << -1./tau->getVal() << " ["<< -1./tau->getMax() <<" => "<< -1./tau->getMin() <<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx Landau mean : " << meanL->getVal() << " sigma : " << sigmaL->getVal() << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"dEdx Gaus mean   : " << meanG->getVal() << " sigma : " << sigmaG->getVal() << std::endl
	       <<"["<<__FUNCTION__<<"] "
	       <<"Frac: " << frac->getVal() <<std::endl;
    }

    if(p.contains_value("e_params")) {
      auto darray = p.get<std::vector<double> >("e_params");
      RooRealVar *tau, *meanL, *sigmaL, *meanG, *sigmaG, *frac;
      tau = (RooRealVar*)(_e_radLenPdf->getVariables()->find("e_radLen_tau"));
      tau->setVal   ( darray[0]               );
      tau->setRange ( darray[1], darray[2] );

      meanL  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Landau_mean"));
      meanL->setVal  ( darray[3] );
      sigmaL = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Landau_sigma"));
      sigmaL->setVal ( darray[4] );

      meanG  = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Gaus_mean"));
      meanG->setVal  ( darray[5] );
      sigmaG = (RooRealVar*)(_e_dEdxPdf->getVariables()->find("e_Gaus_sigma"));
      sigmaG->setVal ( darray[6] );

      frac = (RooRealVar*)(_g_dEdxPdf->getVariables()->find("g_fraction"));
      frac->setVal ( darray[7] );

      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Loaded electron parameters..." << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"Rad Length       : " << -1./tau->getVal() << " ["<< -1./tau->getMax() <<" => "<< -1./tau->getMin() <<"]" << std:: endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx Landau mean : " << meanL->getVal() << " sigma : " << sigmaL->getVal() << std::endl;
      std::cout<<"["<<__FUNCTION__<<"] "
	       <<"dEdx Gaus mean   : " << meanG->getVal() << " sigma : " << sigmaG->getVal() << std::endl;
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

  bool AlgoEMPart::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

    auto datacpy = data;

    if (_verbose){
      std::cout << "******************* BEGIN EMPArt Reconstruction  ******************" << std::endl;
      std::cout << "Number of Showers: " << datacpy.Shower().size() << std::endl;
      std::cout << "Number of Shower Particles: " << graph.GetParticleNodes(RecoType_t::kShower).size() << std::endl;
    }
    
    // Loop through showers
    for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower)){

      auto const& shr = datacpy.Shower(graph.GetParticle(s).RecoID());

      double dEdx;
      double dist;
      double e_like, g_like;

      if (!_training_mode){
	// for every shower find the object with the smallest IP
	double IPmin = 1036;
	::geoalgo::Point_t vtxMin(3);
	for (auto const& s2 : graph.GetParticleNodes(RecoType_t::kShower)){
	  // make sure we are not comparing with itself
	  if (s2 == s) continue;
	  ::geoalgo::Point_t vtx(3);
	  auto const& shr2 = datacpy.Shower(graph.GetParticle(s2).RecoID());
	  double IP = _findRel.FindClosestApproach(shr,shr2,vtx);
	  if (IP < IPmin){
	    IPmin = IP;
	    vtxMin = vtx;
	  }
	}// loop over other showers
	// loop over other tracks
	for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
	  // make sure we are not comparing with itself
	  ::geoalgo::Point_t vtx(3);
	  auto const& trk = datacpy.Track(graph.GetParticle(t).RecoID());
	  double IP = _findRel.FindClosestApproach(shr,trk,vtx);
	  if (IP < IPmin){
	    IPmin = IP;
	    vtxMin = vtx;
	  }
	}// loop over other tracks
	
	// if IP min is < 1 cm -> use radLen
	::geoalgo::Point_t shrvtx(3);
	dist = -1;
	dEdx = shr._dedx;
	if (IPmin < 1) {
	  dist = vtxMin.Dist(shr.Start());
	  e_like = LL(true,  dEdx, dist);
	  g_like = LL(false, dEdx, dist);
	  shrvtx = vtxMin;
	}
	else{
	  e_like = LL(true,  dEdx, dist);
	  g_like = LL(false, dEdx, dist);
	  shrvtx = shr.Start();
	}
	
	int pdg_code = ( g_like > e_like ? 22 : 11 );
	double mass  = ( pdg_code == 11 ? _e_mass : _g_mass );
	auto const& mom = shr.Dir() * (shr._energy - mass);
	graph.GetParticle(s).SetParticleInfo(pdg_code,mass,shrvtx,mom,0.);
      }// if not in training mode
      // if in training mode
      else{
	
	dist   = -1.;
	dEdx   = shr._dedx;

	// skip if dEdx out of bounds
	if ( !_dEdxVar->inRange( dEdx, 0 ) ) continue;
	
	// get vertex info if it exists from particle
	auto const& vtx = graph.GetParticle(s).Vertex();
	if (vtx != kINVALID_VERTEX)
	  dist = shr.Start().Dist(vtx);
	
	if (_verbose) { std::cout << "dEdx: " << dEdx << "\tdist: " << dist << std::endl; }
	
	e_like = LL(true,  dEdx, dist);
	g_like = LL(false, dEdx, dist);
	
	int pdg_code = ( g_like > e_like ? 22 : 11 );
	double mass  = ( pdg_code == 11 ? _e_mass : _g_mass );
	auto const& mom = shr.Dir() * (shr._energy - mass);
	graph.GetParticle(s).SetParticleInfo(pdg_code,mass,shr.Start(),mom,0.);
      }

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
	  _hradLen_g->Fill(dist);
	  _hdEdx_g->Fill(dEdx);
	}else {
	  if(!(dist<0)) { _e_radLenData->add(RooArgSet(*_radLenVar)); }
	  _e_dEdxData->add(RooArgSet(*_dEdxVar));
	  _hradLen_e->Fill(dist);
	  _hdEdx_e->Fill(dEdx);
	}
      }// if training mode

    }// for all showers
    
    return true;
  }

  void AlgoEMPart::ProcessEnd(TFile* fout)
  {

    _hradLen_e->Write();
    _hradLen_g->Write();

    _hdEdx_e->Write();
    _hdEdx_g->Write();
    
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
      std::vector<double> darray;
      auto& params = OutputPSet();
      if (!_mode){
	RooRealVar *tau, *meanL, *sigmaL, *meanG, *sigmaG, *frac;
	if (_verbose)
	  std::cout << "["<<__FUNCTION__<<"] " << Form("Extracted %s_params... ",part_letter.c_str()) << std::endl;
	// radLen
	tau = (RooRealVar*)(fit_res_radLen->floatParsFinal().find(Form("%s_radLen_tau",part_letter.c_str())));
	meanL  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Landau_mean",part_letter.c_str())));
	sigmaL = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Landau_sigma",part_letter.c_str())));
	meanG  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Gaus_mean",part_letter.c_str())));
	sigmaG = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Gaus_sigma",part_letter.c_str())));
	frac   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_fraction",part_letter.c_str())));

	darray.resize(8);
	darray[0] = tau->getVal();
	darray[1] = tau->getErrorLo();
	darray[2] = tau->getErrorHi();
	darray[3] = meanL->getVal();
	darray[4] = sigmaL->getVal();
	darray[5] = meanG->getVal();
	darray[6] = sigmaG->getVal();
	darray[7] = frac->getVal();
	
	std::cout << "["<<__FUNCTION__<<"] "
		  << "RadLen: "<< tau->getVal() << " [" << tau->getErrorLo() + tau->getVal()
		  << " => " << tau->getErrorHi() + tau->getVal() << "]" << std::endl;
	std::cout << "["<<__FUNCTION__<<"] "
		  << "dEdx: MeanL: " << meanL->getVal() << " SigmaL: " << sigmaL->getVal() << std::endl;
	std::cout << "["<<__FUNCTION__<<"] "
		  << "dEdx: MeanG: " << meanG->getVal() << " SigmaG: " << sigmaG->getVal() << std::endl;
	std::cout << "["<<__FUNCTION__<<"] "
		  << "fraction   : " << frac->getVal() << std::endl;
	params.add_value(Form("%s_params",part_letter.c_str()),::fcllite::VecToString<double>(darray));
      }
      if (_mode){
	RooRealVar *tau, *meanG, *sigmaG, *meanL, *sigmaL, *frac;
	// in case od double-gaussian
	tau = (RooRealVar*)(fit_res_radLen->floatParsFinal().find(Form("%s_radLen_tau",part_letter.c_str())));
	meanL  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Landau_mean",part_letter.c_str())));
	sigmaL = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Landau_sigma",part_letter.c_str())));
	meanG  = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Gaus_mean",part_letter.c_str())));
	sigmaG = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_Gaus_sigma",part_letter.c_str())));
	frac   = (RooRealVar*)(fit_res_dEdx->floatParsFinal().find(Form("%s_fraction",part_letter.c_str())));
	darray.resize(8);
	darray[0] = tau->getVal();
	darray[1] = tau->getVal()+tau->getErrorLo();
	darray[2] = tau->getVal()+tau->getErrorHi();
	darray[3] = meanL->getVal();
	darray[4] = sigmaL->getVal();
	darray[5] = meanG->getVal();
	darray[6] = sigmaG->getVal();
	darray[7] = frac->getVal();
	std::cout << "["<<__FUNCTION__<<"] "
		  << "RadLen           : "<< tau->getVal() << " [" << tau->getErrorLo() + tau->getVal()
		  << " => " << tau->getErrorHi() + tau->getVal() << "]" << std::endl
		  << "["<<__FUNCTION__<<"] "
		  << "dEdx: Landau Mean: " << meanL->getVal() << " Sigma: " << sigmaL->getVal() << std::endl 
		  << "["<<__FUNCTION__<<"] "
		  << "dEdx: Gauss  Mean: " << meanG->getVal() << " Sigma: " << sigmaG->getVal() << std::endl
		  << "["<<__FUNCTION__<<"] "
		  << "fraction         : " << frac->getVal() << std::endl;
	params.add_value(Form("%s_params",part_letter.c_str()),::fcllite::VecToString(darray));
      }
      
    }// if in traning mode

    // Plot the likelyhoods if in verbose mode
    if (_plot){

      TCanvas *c = new TCanvas("c","",1000,500);

      // Rad Length likelyhood
      TH1D *h11_radLen = new TH1D("h11_radLen","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",100,0,20);
      TH1D *h22_radLen = new TH1D("h22_radLen","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",100,0,20);
      
      for(size_t i=0; i<100; ++i) {
	_radLenVar->setVal(20*i/100.);
	
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
      if (fout){
	h11_radLen->Write();
	h22_radLen->Write();
      }

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
      if (fout){
	h11_dEdx->Write();
	h22_dEdx->Write();
      }
      
      delete h11_dEdx;
      delete h22_dEdx;

      // 2D ratio map
      TH2D *h_2DRatio = new TH2D("_h2DRatio","2D Likelyhood; dEdx [MeV/cm]; Rad Len [cm]",100,0,8,100,0,20);

      for (size_t dedx=0; dedx < 100; dedx++){
	for (size_t radlen=0; radlen < 100; radlen++){

	  _dEdxVar->setVal(8*dedx/100.);
	  _radLenVar->setVal(20*radlen/100.);
	  
	  //double e_likelyhood = LL(true,8*dedx/100.,50*radlen/100.)-LL(false,8*dedx/100.,50*radlen/100.);
	  double e_likelyhood = log((_e_dEdxPdf->getVal(*_dEdxVar) * _e_radLenPdf->getVal(*_radLenVar) ) /
				    (_g_dEdxPdf->getVal(*_dEdxVar) * _g_radLenPdf->getVal(*_radLenVar) ) );
	  if (e_likelyhood > 1)  { e_likelyhood =  1; }
	  if (e_likelyhood < -1) { e_likelyhood = -1; }
	  /*
	  double e_likelyhood = log((_e_dEdxPdf->getVal(*_dEdxVar) * _e_radLenPdf->getVal(*_radLenVar) ) / 
	    ( _g_dEdxPdf->getVal(*_dEdxVar) * _g_radLenPdf->getVal(*_radLenVar) + 
	      _e_dEdxPdf->getVal(*_dEdxVar) * _e_radLenPdf->getVal(*_radLenVar) ) );
	  */
	  h_2DRatio->SetBinContent(dedx+1,radlen+1,e_likelyhood);

	}
      }

      h_2DRatio->Draw("COLZ");
      c->SaveAs("2DRatio.png");
      if (fout) { h_2DRatio->Write(); }
	    

      // for fun make a ProdPdf to plot 2D Pdf surface
      RooProdPdf ePDF("ePDF","ePDF",RooArgSet(*_e_radLenPdf,*_e_dEdxPdf));
      RooProdPdf gPDF("gPDF","gPDF",RooArgSet(*_g_radLenPdf,*_g_dEdxPdf));
      TH1* h_2D = gPDF.createHistogram("gamma radLen vs. dEdx",*_radLenVar,RooFit::Binning(100,0,30),
				       RooFit::YVar(*_dEdxVar,RooFit::Binning(100,0,8)) ); 
      h_2D->SetNameTitle("gamma radLen vs. dEdx","gamma radLen vs. dEdx");
      h_2D->Draw("SURF3");
      c->SaveAs("radLen_vs_dEdx_2DPDF_gamma.png");
      if (fout) { h_2D->Write(); }
      delete h_2D;
      h_2D = ePDF.createHistogram("electron radLen vs. dEdx",*_radLenVar,RooFit::Binning(100,0,30),
				  RooFit::YVar(*_dEdxVar,RooFit::Binning(100,0,8)) ); 
      h_2D->SetNameTitle("electron radLen vs. dEdx","electron radLen vs. dEdx");
      h_2D->Draw("SURF3");
      c->SaveAs("radLen_vs_dEdx_2DPDF_electron.png");
      if (fout) { h_2D->Write(); }
      delete h_2D;
      
      
      // Plot a bunch of stuff!
      RooPlot* frame_radLen = nullptr;
      RooPlot* frame_dEdx   = nullptr;
      
      frame_radLen = _radLenVar->frame();
      frame_dEdx   = _dEdxVar->frame();
      
      if(_mode){
	_g_radLenData->plotOn(frame_radLen,RooFit::MarkerColor(kRed),RooFit::LineColor(kRed));
	_g_dEdxData->plotOn(frame_dEdx,RooFit::MarkerColor(kRed),RooFit::LineColor(kRed));
	_g_dEdxPdf->plotOn(frame_dEdx,RooFit::LineColor(kRed));
	_g_radLenPdf->plotOn(frame_radLen,RooFit::LineColor(kRed));
      }
      else{
	_e_radLenData->plotOn(frame_radLen,RooFit::MarkerColor(kBlue),RooFit::LineColor(kBlue));
	_e_dEdxData->plotOn(frame_dEdx,RooFit::MarkerColor(kBlue),RooFit::LineColor(kBlue));
	_e_dEdxPdf->plotOn(frame_dEdx,RooFit::LineColor(kBlue));
	_e_radLenPdf->plotOn(frame_radLen,RooFit::LineColor(kBlue));
      }

      c->SetTitle("Distance PDF");
      frame_radLen->Draw();
      if (_mode)
	frame_radLen->SetNameTitle("Radiation Length PDF for gammas","Radiation Length PDF for gammas");
      else
	frame_radLen->SetNameTitle("Radiation Length PDF for electrons","Radiation Length PDF for electrons");
      frame_radLen->SetXTitle("Distance from Shower Origin [cm]");
      frame_radLen->SetYTitle("Number of Showers");
      frame_radLen->SetTitleFont(50,"X");
      frame_radLen->SetTitleFont(50,"Y");
      c->SaveAs(Form("RadLength_Selected_%s.png", part_letter.c_str()));
      c->SetTitle("dEdx Selection");
      frame_dEdx->Draw();
      if (fout) { frame_dEdx->Write(); }
      if (_mode)
	frame_radLen->SetNameTitle("dE/dx PDF for gammas","dE/dx PDF for gammas");
      else
	frame_radLen->SetNameTitle("dE/dx PDF for electrons","dE/dx PDF for electrons");
      frame_radLen->SetXTitle("dE/dx [MeV/cm]");
      frame_radLen->SetYTitle("Number of Showers");
      frame_radLen->SetTitleFont(50,"X");
      frame_radLen->SetTitleFont(50,"Y");
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
