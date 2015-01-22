#ifndef ERTOOL_ALGOPI0_CXX
#define ERTOOL_ALGOPI0_CXX

#include "AlgoPi0.h"
#include "Base/BookKeeper.h"
#include "GeoAlgo/GeoAlgo.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace ertool {

  AlgoPi0::AlgoPi0() 
    : AlgoBase()
    , _radLenVar(nullptr)
    , _vtxVar(nullptr)
    , _vtxSigl(nullptr)
    , _vtxBkgd(nullptr)
    , _hMass_vs_LL(nullptr)
    , _hMass(nullptr)
    , _hBestMass(nullptr)
    , _ll_tree(nullptr)
  {
    _name     = "AlgoPi0";
    _energy_min = 40.;
    _energy_max = 1200.;
    _fit_min    = 10;
    _fit_max    = 200;
    _angle_min  = 0.;
    _angle_max  = 3.141592653589793;
    _IPMax  = 30000.;
    _vtxDistMax  = 100036.;
    _vtx_err       = 2; //[ CM ]

    // Get PDFs
    _radLenVar = new RooRealVar("_radLenVar","Distance from Vertex [cm]",0,_vtxDistMax);
    _radLenBkgd = _factory.UniformDistrib("pi0_radLen", *_radLenVar);
    _radLenSigl = _factory.RadiationLength("pi0", *_radLenVar);

    _vtxVar = new RooRealVar("_vtxVar","Impact Parameter [cm]",0,_IPMax);
    _vtxBkgd = _factory.UniformDistrib("pi0_vtx", *_vtxVar);
    _vtxSigl = _factory.Gaus("pi0", *_vtxVar);

    SetDefaultParams();
  }

  void AlgoPi0::SetDefaultParams()
  {

    RooRealVar *var;

    var = (RooRealVar*)(_radLenSigl->getVariables()->find("pi0_radLen_tau"));
    var->setVal    ( -0.033 );
    var->setRange  ( -0.1, -0.01);

    var = (RooRealVar*)(_vtxSigl->getVariables()->find("pi0_Gaus_mean"));
    var->setVal    ( 0.0 );
    var->setRange  ( 0.0, 10.0);
    
    var = (RooRealVar*)(_vtxSigl->getVariables()->find("pi0_Gaus_sigma"));
    var->setVal    ( 2.0 );
    var->setRange  ( 0.0, 10.0);

  }

  void AlgoPi0::LoadParams(std::string fname, size_t version){

    // Load EMPart params
    _alg_emp.LoadParams(fname,version);

    //Load user_info
    Record::LoadParams(fname,version);

    // Extract if parameters found
    std::cout << "["<<__FUNCTION__<<"] : Loading Pi0 Params." << std::endl;

    if(_params.exist_darray("energy_range")){
      auto darray = _params.get_darray("energy_range");
      _energy_min = (*darray)[0];
      _energy_max = (*darray)[1];
      std::cout<<"["<<__FUNCTION__<<"] "
	       << "Loaded energy range : "
	       << _energy_min << " => " << _energy_max
	       << std::endl;
    }

    if(_params.exist_darray("angle_range")){
      auto darray = _params.get_darray("angle_range");
      _angle_min = (*darray)[0];
      _angle_max = (*darray)[1];
      std::cout<<"["<<__FUNCTION__<<"] "
	       << "Loaded angle range : "
	       << _angle_min << " => " << _angle_max
	       << std::endl;
    }

    return;
  }

  void AlgoPi0::ProcessBegin()
  {
    _alg_emp.ProcessBegin();

    // Initialize Histograms
    delete _hMass_vs_LL;
    delete _hMass;
    delete _hBestMass;
    _hMass_vs_LL = new TH2D("hMass_vs_LL","Mass vs. Likelihood",100,0,200,100,-20,2);
    _hMass = new TH1D("hMass","Mass",100,0,500);
    _hBestMass = new TH1D("hBestMass","BestMass",100,0,500);

    // Initialize LL Tree
    delete _ll_tree;
    _ll_tree = new TTree("_ll_tree","LL Tree");
    _ll_tree->Branch("_ll_vtx",&_ll_vtx,"ll_vtx/D");
    _ll_tree->Branch("_vtx_IP",&_vtx_IP,"vtx_IP/D");
    _ll_tree->Branch("_ll_dedx_A",&_ll_dedx_A,"ll_dedx_A/D");
    _ll_tree->Branch("_dedx_A",&_dedx_A,"dedx_A/D");
    _ll_tree->Branch("_ll_dedx_B",&_ll_dedx_B,"ll_dedx_B/D");
    _ll_tree->Branch("_dedx_B",&_dedx_B,"dedx_B/D");
    _ll_tree->Branch("_ll_vtxDist_A",&_ll_vtxDist_A,"ll_vtxDist_A/D");
    _ll_tree->Branch("_vtxDist_A",&_vtxDist_A,"vtxDist_A/D");
    _ll_tree->Branch("_ll_vtxDist_B",&_ll_vtxDist_B,"ll_vtxDist_B/D");
    _ll_tree->Branch("_vtxDist_B",&_vtxDist_B,"vtxDist_B/D");
    _ll_tree->Branch("_ll",&_ll,"ll/D");
    _ll_tree->Branch("_mass",&_mass,"mass/D");
    _ll_tree->Branch("_angle",&_angle,"angle/D");
    _ll_tree->Branch("_dedx_A",&_dedx_A,"dedx_A/D");
    _ll_tree->Branch("_dedx_B",&_dedx_B,"E_B/D");
    _ll_tree->Branch("_E_A",&_E_A,"E_A/D");
    _ll_tree->Branch("_E_B",&_E_B,"E_B/D");
    _ll_tree->Branch("_dot",&_dot,"dot/D");

  }
  
  void AlgoPi0::LL(const Shower& shower_a,
		   const Shower& shower_b,
		   double& ll,
		   double& mass,
		   geoalgo::Point_t& vtx,
		   geoalgo::Vector_t& mom)
  {

    ll   = -1.e-15;
    _mass = -1.;
    _E_A = shower_a._energy;
    _E_B = shower_b._energy;
    _dedx_A = shower_a._dedx;
    _dedx_B = shower_b._dedx;
    
    // Sanity check
    if( (_E_A < 0) || (_E_B < 0) ) return;

    // dEdx should be > 1 MeV/cm (PDF for gamma is > PDF e- near 0! )
    //if( (_dedx_A < 0.5) || (_dedx_B < 0.5) ) return;

    // Pi0 Vtx point
    ::geoalgo::GeoAlgo geo_alg;
    ::geoalgo::HalfLine_t   line_a( shower_a.Start(), shower_a.Dir()*(-1) );
    ::geoalgo::HalfLine_t   line_b( shower_b.Start(), shower_b.Dir()*(-1) );
    // pt_a is the point on line_a from where the minimum IP is found. Analogous for pt_b
    ::geoalgo::Vector_t pt_a(3), pt_b(3);
    _vtx_IP = sqrt(geo_alg.SqDist(line_a, line_b, pt_a, pt_b));
    // let the candidate vertex be the midpoint between pt_a & pt_b
    vtx = pt_a + ((pt_b - pt_a) / 2.);
    // make sure vertex reconstructed backwards of 
    ::geoalgo::Vector_t vtx_A(shower_a.Start()-vtx);
    vtx_A.Normalize();
    double dotA = vtx_A.Dot(shower_a.Dir());
    ::geoalgo::Vector_t vtx_B(shower_b.Start()-vtx);
    vtx_B.Normalize();
    double dotB = vtx_B.Dot(shower_b.Dir());
    _dot = dotA*dotB;
    if (_verbose) {
      std::cout << "Vtx to Shr A and Shr A Direction dot product: " << dotA << std::endl;
      std::cout << "Vtx to Shr B and Shr B Direction dot product: " << dotB << std::endl;
    }
    _vtxDist_A = vtx.Dist(shower_a.Start());
    _vtxDist_B = vtx.Dist(shower_b.Start());
    // Make sure distance from vtx & IP not out of bounds
    //    if ( (_vtxDist_A > _vtxDistMax) or (_vtxDist_B > _vtxDistMax) or (_vtx_IP > _IPMax) ) return;

    _radLenVar->setVal(_vtxDist_A);
    _ll_vtxDist_A = log( _radLenSigl->getVal(*_radLenVar) / (_radLenSigl->getVal(*_radLenVar) + _radLenBkgd->getVal(*_radLenVar)) );
    _radLenVar->setVal(_vtxDist_B);
    _ll_vtxDist_B = log( _radLenSigl->getVal(*_radLenVar) / (_radLenSigl->getVal(*_radLenVar) + _radLenBkgd->getVal(*_radLenVar)) );

    if (_verbose) {
      std::cout << "Shr A: " << std::endl 
		<< "\tEnergy: " << shower_a._energy << std::endl
		<< "\tdEdx:   " << shower_a._dedx << std::endl
		<< "\tVtx D : " << _vtxDist_A << std::endl
		<< "Shr B: " << std::endl 
		<< "\tEnergy: " << shower_b._energy << std::endl
		<< "\tdEdx:   " << shower_b._dedx << std::endl 
		<< "\tVtx D : " << _vtxDist_B << std::endl 
		<< "\tImpact Parameter: " << _vtx_IP << std::endl << std::endl;
    }

    // Opening angle
    _angle = shower_a.Dir().Angle(shower_b.Dir());
    //if (_angle > _angle_max) return;
    if (_verbose) { std::cout << "Opening angle:" << _angle << std::endl; }

    // Corrected energies
    ::EMShowerProfile shower_prof;
    ::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);

    auto xs_a = geo_alg.Intersection(volume, ::geoalgo::HalfLine_t(shower_a.Start(),shower_a.Dir()));
    auto xs_b = geo_alg.Intersection(volume, ::geoalgo::HalfLine_t(shower_b.Start(),shower_b.Dir()));

    if(!xs_a.size() || !xs_b.size()) return;

    double energy_a = shower_a._energy / shower_prof.EnergyContainment( shower_a.Start().Dist(xs_a[0]) );
    double energy_b = shower_b._energy / shower_prof.EnergyContainment( shower_b.Start().Dist(xs_b[0]) );

    // Compute mass
    _mass = sqrt(4 * energy_a * energy_b * pow(sin(_angle/2.),2));
    mass = _mass;

    // calculate momentum:
    // add direction vectors scaled by shower energies and subtract mass
    geoalgo::Vector_t energy(shower_a.Dir() * energy_a + shower_b.Dir() * energy_b);
    energy *= (energy.Length() - _mass) / energy.Length();
    mom = energy;

    if (_verbose) { std::cout << "reconstructed mass: " << _mass << std::endl; }

    // likelihood of each shower of being more e-like or g-like:
    _dedx_A = shower_a._dedx;
    _ll_dedx_A = _alg_emp.LL(false,shower_a._dedx,-1);//vtx.Dist(shower_a.Start()));
    _dedx_B = shower_b._dedx;
    _ll_dedx_B = _alg_emp.LL(false,shower_b._dedx,-1);//vtx.Dist(shower_b.Start()));
    
    if (_verbose){
      std::cout << "dEdx Shower A: " << _dedx_A << "\tLL: " << _ll_dedx_A << std::endl;
      std::cout << "dEdx Shower B: " << _dedx_B << "\tLL: " << _ll_dedx_B << std::endl;
    }
     
    // get lilelyhood for vertex reconstruction:
    _vtxVar->setVal(_vtx_IP);
    _ll_vtx = log( _vtxSigl->getVal(*_vtxVar) / ( _vtxSigl->getVal(*_vtxVar) + _vtxBkgd->getVal(*_vtxVar) ) ); 
    
    if (_verbose) {
      std::cout << "IP for Showers: " << _vtx_IP << "\tLL: "<< _ll_vtx << std::endl;
    }

    ll += _ll_vtx;
    ll += _ll_dedx_A;
    ll += _ll_dedx_B;
    ll += _ll_vtxDist_A;
    ll += _ll_vtxDist_B;
    _ll = ll;

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
    geoalgo::Point_t vertex(3);
    geoalgo::Vector_t momentum(3);

    for(auto const& shower_v : data.ShowerCombination(2)) {

      comb[0] = shower_v[0]->ID();
      comb[1] = shower_v[1]->ID();

      double likelihood = 0.;
      double mass       = -1.;
      
      _angle = 0;
      
      LL(*(shower_v[0]),
	 *(shower_v[1]),
	 likelihood, mass, vertex, momentum);
      if (likelihood > best_ll) { best_ll = likelihood; best_mass = mass; }
      
      bk.book(likelihood,comb);

      if ( (likelihood != -1.e-10) && (mass > 0) ){
	_hMass_vs_LL->Fill(mass,likelihood);
	// create a Pi0 and two gamma showers!
	Particle gamma1(22,0.);
	gamma1.Vertex(shower_v[0]->Start());
	gamma1.Momentum(shower_v[0]->Dir()*shower_v[0]->_energy);
	gamma1.RecoObjInfo(comb[0],Particle::RecoObjType_t::kShower);
	res.push_back(gamma1);
	Particle gamma2(22,0.);
	gamma2.Vertex(shower_v[1]->Start());
	gamma2.Momentum(shower_v[1]->Dir()*shower_v[1]->_energy);
	gamma2.RecoObjInfo(comb[1],Particle::RecoObjType_t::kShower);
	res.push_back(gamma2);
	Particle pi0(111,mass);
	pi0.Vertex(vertex);
	pi0.Momentum(momentum);
	pi0.AddDaughter(gamma1);
	pi0.AddDaughter(gamma2);
	res.push_back(pi0);
      }
      _hMass->Fill(_mass);

    } // for all shower combinations

    if (best_mass != 0) { _hBestMass->Fill(best_mass); }

    return res;
  }

  void AlgoPi0::ProcessEnd(TFile* fout)
  {

    if(fout) {
      fout->cd();
      _hMass_vs_LL->Write();
      _hMass->Write();
      _hBestMass->Write();
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
