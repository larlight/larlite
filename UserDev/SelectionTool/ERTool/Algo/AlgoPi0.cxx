#ifndef ERTOOL_ALGOPI0_CXX
#define ERTOOL_ALGOPI0_CXX

#include "AlgoPi0.h"
#include "Base/BookKeeper.h"
#include "GeoAlgo/GeoAlgo.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace ertool {

  AlgoPi0::AlgoPi0() 
    : AlgoBase()
    , _pi0_pdf(nullptr)
    , _pi0_massVar(nullptr)
    , _pi0_massMean(nullptr)
    , _pi0_massSigma(nullptr)
    , _pi0_massData(nullptr)
    , _shrCorrelation_pdf(nullptr)
    , _radLenVar(nullptr)
    , _radLenVal(nullptr)
    , _radLenData(nullptr)
    , _vtxVar(nullptr)
    , _vtxErr(nullptr)
    , _vtxMu(nullptr)
    , _vtxErrSigl(nullptr)
    , _vtxErrBkgd(nullptr)
    , _hMass_vs_LL(nullptr)
    , _hMass(nullptr)
    , _hBestMass(nullptr)
    , _candidate_tree(nullptr)
    , _ll_tree(nullptr)
  {
    _name     = "AlgoPi0";
    _energy_min = 40.;
    _energy_max = 1200.;
    _fit_min    = 10;
    _fit_max    = 200;
    _angle_min  = 0.;
    _angle_max  = 3.141592653589793;
    _ip_max  = 300.;
    _pi0_mean      = 130.;
    _pi0_mean_min  = 100.;
    _pi0_mean_max  = 160.;
    _pi0_sigma     = 10.;
    _pi0_sigma_min = 5.;
    _pi0_sigma_max = 70.;
    _radLen        = 0.1; //[ CM-1 ]
    _radLen_min    = 1; //[ CM-1 ]
    _radLen_max    = 0.01; //[ CM-1 ]
    _vtx_err       = 2; //[ CM ]
  }

  void AlgoPi0::LoadParams(std::string fname, size_t version){

    // Load EMPart params
    _alg_emp.LoadParams(fname,version);

    //Load user_info
    Record::LoadParams(fname,version);

    // Extract if parameters found

    if(_params.exist_darray("energy_range")){
      auto darray = _params.get_darray("energy_range");
      _energy_min = (*darray)[0];
      _energy_max = (*darray)[1];
    }

    if(_params.exist_darray("angle_range")){
      auto darray = _params.get_darray("angle_range");
      _angle_min = (*darray)[0];
      _angle_max = (*darray)[1];
    }

    if(_params.exist_double("vtx_dist_max"))
      _ip_max = _params.get_double("ip_max");
      
    if(_params.exist_darray("pi0_mean")){
      auto darray = _params.get_darray("pi0_mean");
      _pi0_mean     = (*darray)[0];
      _pi0_mean_min = (*darray)[1];
      _pi0_mean_max = (*darray)[2];
    }

    if(_params.exist_darray("pi0_sigma")){
      auto darray = _params.get_darray("pi0_sigma");
      _pi0_sigma     = (*darray)[0];
      _pi0_sigma_min = (*darray)[1];
      _pi0_sigma_max = (*darray)[2];
    }

    return;
  }

  void AlgoPi0::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    delete _pi0_pdf;
    delete _pi0_massVar;
    delete _pi0_massMean;
    delete _pi0_massSigma;
    delete _pi0_massData;
    delete _shrCorrelation_pdf;
    delete _radLenVar;
    delete _radLenVal;
    delete _radLenData;

    double mass_min = _pi0_mean - _pi0_sigma_max * 5.;
    if(mass_min<50) mass_min = 50;
    double mass_max = _pi0_mean + _pi0_sigma_max * 5.;
    _pi0_massVar  = new RooRealVar("_pi0_massVar","Pi0 Mass Range [MeV]",
				   //				   mass_min, mass_max);
				   _energy_min, 400);

    _pi0_massMean = new RooRealVar("_pi0_massMean","Pi0 Mass Mean [MeV]",
				   _pi0_mean, _pi0_mean_min, _pi0_mean_max);

    _pi0_massSigma = new RooRealVar("_pi0_massSigma","Pi0 Mass Sigma [MeV]",
				    _pi0_sigma, _pi0_sigma_min, _pi0_sigma_max);

    _pi0_massData = new RooDataSet("_pi0_massData", "Pi0 Mass Fit Data", RooArgSet(*_pi0_massVar));

    PdfFactory factory;
    _pi0_pdf = factory.Pi0Mass(*_pi0_massVar, *_pi0_massMean, *_pi0_massSigma);


    _radLenVar = new RooRealVar("_radLenVar","Distance from Vertex [cm]",_x_min,_x_max);
    _radLenVal = new RooRealVar("_radLenVal","Radiation Length [cm-1]", _radLen, _radLen_min, _radLen_max);
    _radLenData = new RooDataSet("_radLenData","Distance from Vertex Data [cm]",RooArgSet(*_radLenVar));
    _shrCorrelation_pdf = factory.Pi0ShrCorrelation(*_radLenVar,*_radLenVal);

    /// Create PDF for vertex uncertainty (Impact Parameter of two shower directions)
    /// for signal and background
    delete _vtxVar;
    delete _vtxErr;
    delete _vtxMu;
    delete _vtxErrSigl;
    delete _vtxErrBkgd;

    _vtxVar = new RooRealVar("_vtxVar","Vtx Dist. [cm]",0,_ip_max);
    _vtxErr = new RooRealVar("_vtxErr", "Vtx. Err. [cm]",_vtx_err,0.,100.);
    _vtxMu  = new RooRealVar("_vtxMu", "Vtx. Mean Separation",0.,0.,10.);
    _vtxErrSigl = new RooGaussian("_vtxErrSigl","Vtx Err. Signal PDF", *_vtxVar, *_vtxMu, *_vtxErr);
    _vtxErrBkgd = factory.UniformDistrib(*_vtxVar);

    // Initialize Histograms
    delete _hMass_vs_LL;
    delete _hMass;
    delete _hBestMass;
    _hMass_vs_LL = new TH2D("hMass_vs_LL","Mass vs. Likelihood",100,0,200,100,-20,2);
    _hMass = new TH1D("hMass","Mass",100,0,300);
    _hBestMass = new TH1D("hBestMass","BestMass",100,0,300);

    // Initialize LL Tree
    delete _ll_tree;
    _ll_tree = new TTree("_ll_tree","LL Tree");
    _ll_tree->Branch("_ll_vtx",&_ll_vtx,"ll_vtx/D");
    _ll_tree->Branch("_vtx_IP",&_vtx_IP,"vtx_IP/D");
    _ll_tree->Branch("_ll_dedx_A",&_ll_dedx_A,"ll_dedx_A/D");
    _ll_tree->Branch("_dedx_A",&_dedx_A,"dedx_A/D");
    _ll_tree->Branch("_ll_dedx_B",&_ll_dedx_B,"ll_dedx_B/D");
    _ll_tree->Branch("_dedx_B",&_dedx_B,"dedx_B/D");
    _ll_tree->Branch("_m",&_m,"m/D");

    // Initialize Pi0 TTree
    if (_candidate_tree) { delete _candidate_tree; }
    _candidate_tree = new TTree("candidate_tree","Candidate Pi0 Tree");
    _candidate_tree->Branch("_shr1_E",&_shr1_E,"shr1_E/D");
    _candidate_tree->Branch("_shr2_E",&_shr2_E,"shr2_E/D");
    _candidate_tree->Branch("_shr1_dEdx",&_shr1_dEdx,"shr1_dEdx/D");
    _candidate_tree->Branch("_shr2_dEdx",&_shr2_dEdx,"shr2_dEdx/D");
    _candidate_tree->Branch("_mass",&_mass,"mass/D");
    _candidate_tree->Branch("_angle",&_angle,"angle/D");
    _candidate_tree->Branch("_dist_1",&_dist_1,"dist_1/D");
    _candidate_tree->Branch("_dist_2",&_dist_2,"dist_2/D");
    _candidate_tree->Branch("_vtx_dist",&_vtx_dist,"_vtxdist/D");
    _candidate_tree->Branch("_ll",&_ll,"_ll/D");
  }
  
  void AlgoPi0::LL(const Shower& shower_a,
		   const Shower& shower_b,
		   double& ll,
		   double& mass)
  {
    ll   = -1.e-10;
    mass = -1.;
    _dist_1 = -1;
    _dist_2 = -1;
    
    // Sanity check
    if(shower_a._energy<0 || shower_b._energy<0) return;



    // Pi0 Vtx point
    ::geoalgo::GeoAlgo geo_alg;
    ::geoalgo::Line_t   line_a( shower_a.Start(), shower_a.Start() + shower_a.Dir() );
    ::geoalgo::Line_t   line_b( shower_b.Start(), shower_b.Start() + shower_b.Dir() );
    // pt_a is the point on line_a from where the minimum IP is found. Analogous for pt_b
    ::geoalgo::Vector_t pt_a(3), pt_b(3);
    _vtx_dist = sqrt(geo_alg.SqDist(line_a, line_b, pt_a, pt_b));
    // let the candidate vertex be the midpoint between pt_a & pt_b
    auto vtx = pt_a + ((pt_b - pt_a) / 2.);
    _dist_1 = vtx.Dist(shower_a.Start());
    _dist_2 = vtx.Dist(shower_b.Start());
    if (_verbose) {
      std::cout << "Shr A: " << std::endl 
		<< "\tEnergy: " << shower_a._energy << std::endl
		<< "\tdEdx:   " << shower_a._dedx << std::endl
		<< "\tVtx D : " << _dist_1 << std::endl
		<< "Shr B: " << std::endl 
		<< "\tEnergy: " << shower_b._energy << std::endl
		<< "\tdEdx:   " << shower_b._dedx << std::endl 
		<< "\tVtx D : " << _dist_2 << std::endl << std::endl;
    }

    if(_vtx_dist > _ip_max) return;

    // Opening angle
    double angle = shower_a.Dir().Angle(shower_b.Dir());
    _angle = angle;
    if (_verbose) { std::cout << "Opening angle:" << angle << std::endl; }

    if(angle > _angle_max) return;

    // Corrected energies
    ::EMShowerProfile shower_prof;
    ::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);

    auto xs_a = geo_alg.Intersection(volume, ::geoalgo::HalfLine_t(shower_a.Start(),shower_a.Dir()));
    auto xs_b = geo_alg.Intersection(volume, ::geoalgo::HalfLine_t(shower_b.Start(),shower_b.Dir()));

    if(!xs_a.size() || !xs_b.size()) return;

    double energy_a = shower_a._energy / shower_prof.EnergyContainment( shower_a.Start().Dist(xs_a[0]) );
    double energy_b = shower_b._energy / shower_prof.EnergyContainment( shower_b.Start().Dist(xs_b[0]) );

    // Compute mass
    mass = sqrt(4 * energy_a * energy_b * pow(sin(angle/2.),2));

    if (_verbose) { std::cout << "reconstructed mass: " << mass << std::endl; }

    _pi0_massVar->setVal(mass);
    _m = mass;

    // likelihood of each shower of being more e-like or g-like:
    double e_like_a = _alg_emp.LL(true,shower_a._dedx,-1);//vtx.Dist(shower_a.Start()));
    _dedx_A = shower_a._dedx;
    _ll_dedx_A = _alg_emp.LL(false,shower_a._dedx,-1);//vtx.Dist(shower_a.Start()));
    double e_like_b = _alg_emp.LL(true,shower_b._dedx,-1);//vtx.Dist(shower_b.Start()));
    _dedx_B = shower_b._dedx;
    _ll_dedx_B = _alg_emp.LL(false,shower_b._dedx,-1);//vtx.Dist(shower_b.Start()));
    
    if (_verbose){
      std::cout << "likelihood shower A is e-like: " << e_like_a << "\tg-like: " << _ll_dedx_A << std::endl;
      std::cout << "likelihood shower B is e-like: " << e_like_b << "\tg-like: " << _ll_dedx_B << std::endl;
    }
     
    // get lilelyhood for vertex reconstruction:
    _vtxVar->setVal(_vtx_dist);
    _vtx_IP = _vtx_dist;
    _ll_vtx = log( _vtxErrSigl->getVal(*_vtxVar) / ( _vtxErrSigl->getVal(*_vtxVar) + _vtxErrBkgd->getVal(*_vtxVar) ) ); 
    if (_verbose) {
      std::cout << "IP for Showers: " << _vtx_dist 
		<< "\tLL: "<< _ll_vtx << std::endl;
    }
    ll += _ll_vtx;
    ll += _ll_dedx_A;
    ll += _ll_dedx_B;

    _ll_tree->Fill();

    return;
  }
  
  ParticleSet AlgoPi0::Reconstruct(const EventData &data)
  { 

    if (_verbose) {
      std::cout << "showers in event: " << data.Shower().size() << std::endl;
    }
    ParticleSet res;
    //    if(!data._vtxs.size() || data._showers.size()<2) return res;
    if(data.Shower().size()<2) return res;

    BookKeeper bk;
    Combination_t comb(2);

    double best_ll = -1.e10;
    double best_mass = 0.;

    for(auto const& shower_v : data.ShowerCombination(2)) {

      comb[0] = shower_v[0]->ID();
      comb[1] = shower_v[1]->ID();

      double likelihood = 0.;
      double mass       = -1.;
      
      _angle = 0;
      
      LL(*(shower_v[0]),
	 *(shower_v[1]),
	 likelihood,
	 mass);

      if (likelihood > best_ll) { best_ll = likelihood; best_mass = mass; }
      
      bk.book(likelihood,comb);

      if (likelihood != -1.e-10)
	_hMass_vs_LL->Fill(mass,likelihood);
      _hMass->Fill(mass);

      _ll = likelihood;
      _mass = mass;
      _shr1_E = shower_v[0]->_energy;
      _shr2_E = shower_v[1]->_energy;
      _shr1_dEdx = shower_v[0]->_dedx;
      _shr2_dEdx = shower_v[1]->_dedx;
      _candidate_tree->Fill();

      if( 0 < mass &&
	  0 < mass &&
	  (_pi0_mean - _pi0_sigma_max * 5.) < mass &&
	  mass < (_pi0_mean + _pi0_sigma_max * 5.) ) {
	_pi0_massVar->setVal(mass);
	_pi0_massData->add(RooArgSet(*_pi0_massVar));
	//std::cout<<_pi0_massData->sumEntries()<<" entries... last: "<<_pi0_massData->get(_pi0_massData->sumEntries()-1)->getRealValue("_pi0_massVar")<<std::endl;
      }
    } // for all shower combinations

    if (best_mass != 0) { _hBestMass->Fill(best_mass); }

    return res;
  }

  void AlgoPi0::ProcessEnd(TFile* fout)
  {

    if(_training_mode) {
      RooFitResult* fit_res = _pi0_pdf->fitTo(*_pi0_massData,RooFit::Save(),RooFit::Range(_fit_min,_fit_max));
      fit_res->Print();
      
      RooPlot* frame_mass = _pi0_massVar->frame();
      _pi0_massData->plotOn(frame_mass);
      _pi0_pdf->plotOn(frame_mass);
      
      TCanvas* c = new TCanvas("c","",600,500);
      frame_mass->Draw();
      c->SaveAs("pi0_mass.png");
      delete c;
    }

    if(fout) {
      fout->cd();
      _hMass_vs_LL->Write();
      _hMass->Write();
      _hBestMass->Write();
      _candidate_tree->Write();
      _ll_tree->Write();
    }else{
      delete _hMass_vs_LL;
      delete _hMass;
      delete _hBestMass;
    }

    return;
  }

}

#endif
