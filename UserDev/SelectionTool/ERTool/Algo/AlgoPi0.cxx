#ifndef ERTOOL_ALGOPI0_CXX
#define ERTOOL_ALGOPI0_CXX

#include "AlgoPi0.h"
#include "GeoAlgo/GeoAlgo.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace ertool {

  AlgoPi0::AlgoPi0(const std::string& name) 
    : AlgoBase(name)
    , _radLenVar(nullptr)
    , _vtxVar(nullptr)
    , _vtxSigl(nullptr)
    , _vtxBkgd(nullptr)
    , _hMass_vs_LL(nullptr)
    , _hMass(nullptr)
    , _hBestMass(nullptr)
    , _ll_tree(nullptr)
  {


    _energy_min = 40.;
    _energy_max = 1200.;
    _fit_min    = 10;
    _fit_max    = 200;
    _angle_min  = 0.;
    _angle_max  = 3.141592653589793;
    _IPMax  = 30000.;
    _vtxDistMax  = 100036.;
    _vtx_err       = 2; //[ CM ]

    _tau = 8.4e-17;

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

  void AlgoPi0::AcceptPSet(const ::fcllite::PSet& cfg) {

    // Load EMPart params
    _alg_emp.AcceptPSet(cfg);

    //Load user_info
    auto p = cfg.get_pset(Name());

    // Extract if parameters found
    std::cout << "["<<__FUNCTION__<<"] : Loading Pi0 Params." << std::endl;

    if(p.contains_value("energy_range")) {
      auto darray = p.get<std::vector<double> >("energy_range");
      _energy_min = darray[0];
      _energy_max = darray[1];
      std::cout<<"["<<__FUNCTION__<<"] "
	       << "Loaded energy range : "
	       << _energy_min << " => " << _energy_max
	       << std::endl;
    }

    if(p.contains_value("angle_range")){
      auto darray = p.get<std::vector<double> >("angle_range");
      _angle_min = darray[0];
      _angle_max = darray[1];
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
    _ll_tree->Branch("_dedx_B",&_dedx_B,"dedx_B/D");
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

    // Find the common origin, along with
//    if (  (shower_a.Dir().Length() != 1) || (shower_b.Dir().Length() != 1) )
//      return;
    _dot = _geoAlgo.commonOrigin(shower_a, shower_b, vtx, true);

    if (_verbose) {
      std::cout << "Sum of dot-products for direction-matching: " << _dot << std::endl;
    }
    _vtxDist_A = vtx.Dist(shower_a.Start());
    _vtxDist_B = vtx.Dist(shower_b.Start());

    _radLenVar->setVal(_vtxDist_A);
    _ll_vtxDist_A = log( _radLenSigl->getVal(*_radLenVar) / (_radLenSigl->getVal(*_radLenVar) + _radLenBkgd->getVal(*_radLenVar)) );
    _radLenVar->setVal(_vtxDist_B);
    _ll_vtxDist_B = log( _radLenSigl->getVal(*_radLenVar) / (_radLenSigl->getVal(*_radLenVar) + _radLenBkgd->getVal(*_radLenVar)) );

  std::cout << "Vertex of pi0 would be : "<< vtx[0]<<", "<<vtx[1]<<", "<<vtx[2]<<std::endl ; 

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

    auto xs_a = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shower_a.Start(),shower_a.Dir()));
    auto xs_b = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shower_b.Start(),shower_b.Dir()));

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
  
  bool AlgoPi0::Reconstruct(const EventData &data, ParticleGraph& graph)
  { 
	
//    if (_verbose) 
  //    std::cout << "showers in event: " << graph.GetParticleNodes(RecoType_t::kShower).size() << std::endl;
    

 //   std::cout<<"\n\n\nNew Event! "<<std::endl ;
    if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;



    Combination_t comb(2);

    double best_ll = -1.e10;
    double best_mass = 0.;
    geoalgo::Point_t vertex(3);
    geoalgo::Vector_t momentum(3);

    auto datacpy = data;

    // loop over showers and compare showers pair-by-pair
    auto const& shrIDs = graph.GetParticleNodes(RecoType_t::kShower);

    for (auto const& shrID1 : shrIDs){
      for (auto const& shrID2 : shrIDs){
	// skip if we have already checked this combination
	// or if we are comparing to self
	if (shrID2 <= shrID1) continue;

	// make sure the two gammas have not been "used" to form a relationship before 
	if ( graph.GetParticle(shrID1).RelationAssessed() or
	     graph.GetParticle(shrID2).RelationAssessed() ){
	  if (_verbose) { std::cout << "either shower already used to make pi0 or anything else...cannot proceed." << std::endl; }
	  continue;
	}


	auto const& shr1 = datacpy.Shower(graph.GetParticle(shrID1).RecoID());
	auto const& shr2 = datacpy.Shower(graph.GetParticle(shrID2).RecoID());

//	std::cout<< "SHOWER STARTS: "<<shr1.Start()[0]<<", "<<shr1.Start()[1]<<", "<<shr1.Start()[2]<<std::endl ;
//	std::cout<< "SHOWER STARTS: "<<shr2.Start()[0]<<", "<<shr2.Start()[1]<<", "<<shr2.Start()[2]<<std::endl ;

	double likelihood = 0.;
	double mass       = -1.;
      
	_angle = 0;
      
	LL(shr1, shr2, likelihood, mass, vertex, momentum);
	if (likelihood > best_ll) { best_ll = likelihood; best_mass = mass; }

//	std::cout<<"Shower dedx, likelihood: "<<shr1._dedx<<" , "<<shr2._dedx<<", "<<likelihood<<std::endl ;	
//	std::cout<<"Shower dedx, likelihood: "<<shr1._energy<<" , "<<shr2._energy<<std::endl ;
	
	if ( (likelihood != -1.e-10) && (likelihood > -2)&& (mass > 0)&& shr1._energy > 20 && shr2._energy > 20 ){

	  _hMass_vs_LL->Fill(mass,likelihood);
	  // edit particle info to reflect the fact 
	  // that we have 2 gammas coming from a pi0

	  // specify info for 1st gamma
	  auto const& gamma1vtx = shr1.Start();
	  auto const& gamma1mom = shr1.Dir()*shr1._energy;
	  graph.GetParticle(shrID1).SetParticleInfo(22,0,gamma1vtx,gamma1mom);
	  // specify info for 2nd gamma
	  auto const& gamma2vtx = shr2.Start();
	  auto const& gamma2mom = shr2.Dir()*shr2._energy;
	  graph.GetParticle(shrID2).SetParticleInfo(22,0,gamma2vtx,gamma2mom);
	  // create pi0
	  Particle& pi0 = graph.CreateParticle();


	  // Approximate vtx using lifetime
	  auto dir = momentum.Dir();
	  dir *= sqrt( 1 - pow( mass / (mass + momentum.Length()), 2)) * 2.998e10 * _tau;
	  vertex -= dir;
	  pi0.SetParticleInfo(111,mass,vertex,momentum);
	  // Add relationships
	  graph.SetParentage(pi0.ID(),shrID1);
	  graph.SetParentage(pi0.ID(),shrID2);
	  graph.SetSiblings(shrID1,shrID2);
    
	  //Let's try to make a shower object out of this pi0
	  double momDot = ( gamma1mom[0] * gamma2mom[0] + gamma1mom[1] * gamma2mom[1] + gamma1mom[2] * gamma2mom[2] );
	  double momLengthProd = ( gamma1mom.Length() * gamma2mom.Length() ) ;
	  double openAngle = acos ( momDot / momLengthProd ) ; 

	  auto radius = momentum.Length() * tan ( openAngle/2 ) ;

	  ::ertool::Shower s( vertex, momentum, momentum.Length(), radius);
//	  _mgr.Add (s, ::ertool::RecoInputType_t(0,data.Shower().name()), true i);

	}
	_hMass->Fill(_mass);

      } // for all showers
    } // for all showers

    if (best_mass != 0) { _hBestMass->Fill(best_mass); }

    return true;
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
