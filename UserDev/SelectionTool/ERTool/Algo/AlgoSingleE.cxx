#ifndef ERTOOL_ALGOSINGLEE_CXX
#define ERTOOL_ALGOSINGLEE_CXX

#include "AlgoSingleE.h"

namespace ertool {

  size_t single_e_counter = 0;
  size_t total_e_showers = 0;
  size_t total_g_showers = 0;
  size_t nonzero_dedx_counter = 0;
  
  AlgoSingleE::AlgoSingleE() : AlgoBase()
			     , _alg_tree(nullptr)
  {

    _name       = "AlgoSingleE";
    _e_mass     = TDatabasePDG().GetParticle(11)->Mass();
    _e_ll_values = 0;
    _dedx_values = 0;
    _Ethreshold = 0;
    _verbose = false;
    _useRadLength = false;
    _hassister = false;

  }

  void AlgoSingleE::Reset()
  {
    std::cout<<__FUNCTION__<<" found "<<single_e_counter<<" events with a single electron in it."<<std::endl;
    std::cout<<"and "<<nonzero_dedx_counter<<" nonzero dedx showers"<<std::endl;
    std::cout<<"Found "<<total_e_showers<<" total e showers"<<std::endl;
    std::cout<<"Found "<<total_g_showers<<" total g showers"<<std::endl;
  }
  
  void AlgoSingleE::ProcessBegin()
  {

    _alg_emp.LoadParams();
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);

    if(!_e_ll_values)
      _e_ll_values = new TH1F("e_ll_values","e_ll_values",1000,-1,0);

    if(!_dedx_values)
      _dedx_values = new TH1F("dedx_values","dedx_values",1000,0,8);
    
    if (_alg_tree) { delete _alg_tree; }
    _alg_tree = new TTree("_alg_tree","Algo SingleE Tree");
    _alg_tree->Branch("_E",&_E,"_E/D");
    _alg_tree->Branch("_PDG",&_PDG,"PDG/I");
    _alg_tree->Branch("_VsTrack",&_VsTrack,"_VsTrack/I");
    _alg_tree->Branch("_thatE",&_thatE,"thatE/D");
    _alg_tree->Branch("_dEdx",&_dEdx,"_dEdx/D");
    _alg_tree->Branch("_IP",&_IP,"_IP/D");
    _alg_tree->Branch("_IPthisStart",&_IPthisStart,"_IPthisStart/D");
    _alg_tree->Branch("_IPthatStart",&_IPthatStart,"_IPthatStart/D");
    _alg_tree->Branch("_IPtrkBody",&_IPtrkBody,"_IPtrkBody/D");
    
    return;
  }

  void AlgoSingleE::LoadParams(std::string fname, size_t version){
    
    // Load EMPart params
    _alg_emp.LoadParams(fname,version);
    
    return;
  }

  ParticleSet AlgoSingleE::Reconstruct(const EventData &data){
    
    // ParticleSet where to store single electrons
    ParticleSet res;
    
    //res = emParticles;

    if (_verbose) { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
    }
    
    // First, figure out if any tracks are coming from another track
    // if so, label them as secondary.
    // If a shower appears to have a common origin with a secondary
    // then it also will be secondary.
    // Vector where to keep track of track indices for secondarie
    std::vector<int> secondaryTracks;
    for (size_t u=0; u < data.Track().size(); u++){
      for (size_t v=0; v < data.Track().size(); v++){
	// check if u comes from v
	geoalgo::Point_t vtx(3);
	// make sure both tracks have more than 2 points
	if ( (data.Track(u).size() < 2) || (data.Track(v).size() < 2) ) continue;
	double IP = _findRel.FindClosestApproach(data.Track(u),data.Track(v),vtx);
	// if the vertex is close to u's start
	// and close to v's body (but not its start)
	// -> Then u is secondary to v
	if ( (vtx.Dist(data.Track(u).front()) < _vtxToTrkStartDist) && // vertex close to track U's start
	     (vtx.Dist(data.Track(v).front()) > _vtxToTrkStartDist) && // vertex far from track V's start
	     (sqrt(_geoAlgo.SqDist(vtx,data.Track(v))) < _vtxToTrkDist) )    // vertex close to track V's body
	  secondaryTracks.push_back(int(u));
      } // for V tracks
    } // for U tracks

    // We are only interested in electron-like showers
    // loop over showers
    for (size_t sh=0; sh < data.Shower().size(); sh++){
      
      // Ok, we have an electron. Get the associated shower
      // and make sure it does not:
      // NOTE: IN 1) IMPLEMENT EXCLUSION ONLY OF e-like showers
      // 1) share a vertex with another e- shower (gamma-like one could be from Pi0: OK)
      // 2) come from another shower - NOT DONE AS OF YET
      // 3) come from a track
      // 2) & 3) because we are interested in showers from
      // the neutrino interaction
      
      auto const& thisShower = data.Shower(sh);
      
      if (thisShower._energy < _Ethreshold ) continue;
      
      if (_verbose) { std::cout << "This shower: (" << sh << ")" << "\tE: " << thisShower._energy << std::endl; }
      bool single = true;
      // loop over other showers and check 1) and 2)
      for (size_t s=0; s < data.Shower().size(); s++){
	auto const& thatShower(data.Shower(s));
	geoalgo::Point_t vtx(3);
	// make sure we don't use thisShower in the loop
	if (thatShower.ID() != thisShower.ID()) {
	  // is this shower gamma or e-like?
	  // if gamma-like maybe a nearby pi0 -> ok if close
	  if ( _alg_emp.LL(true, thatShower._dedx, -1) < _alg_emp.LL(false, thatShower._dedx, -1) )
	    {
	      if (_verbose) {
		std::cout << "2nd shower has dEdx = " << thatShower._dedx
			  << "\tIgnore for comparison." << std::endl;
	      }
	      break;
	    }
	  if (_verbose) { std::cout << "Comparing with shower (" << s << ")" << std::endl; }
	  // compare the two showers
	  double IP = _findRel.FindClosestApproach(thisShower,thatShower,vtx);
	  _VsTrack = 0;
	  _thatE   = thatShower._energy;
	  _IP = IP;
	  _IPthisStart = vtx.Dist(thisShower.Start());
	  _IPthatStart = vtx.Dist(thatShower.Start());
	  _alg_tree->Fill();
	  if (_verbose)
	    std::cout << "\tImpact Parameter      : " << _IP << std::endl
		      << "\tIP to other Shr Start : " << _IPthatStart << std::endl
		      << "\tIP to this Shr Start  : " << _IPthisStart << std::endl;
	  if ( (IP < _maxIP) && ( vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) && ( vtx.Dist(thatShower.Start()) < _vtxToShrStartDist) ){
	    single = false;
	    if (_verbose) { std::cout << "NOT single" << std::endl; }
	    break;
	  }
	}// if not the same showers
      }//loop over showers
      // loop over tracks if still single
      if (single){
	for (size_t t=0; t < data.Track().size(); t++){
	  auto const& thatTrack(data.Track(t));
	  if (thatTrack.size() < 2)
	    continue;
	  if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }
	  geoalgo::Point_t vtx(3);
	  // compare the two tracks
	  double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx);
	  _VsTrack = 1;
	  _thatE   = thatTrack._energy;
	  _IP = IP;
	  _IPthisStart = vtx.Dist(thisShower.Start());
	  _IPthatStart = vtx.Dist(thatTrack.front());
	  _IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
	  _alg_tree->Fill();
	  if (_verbose)
	    std::cout << "\tImpact Parameter: " << _IP << std::endl
		      << "\tIP to Trk Start : " << _IPthatStart << std::endl
		      << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		      << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	  single = true;
	  if ( (IP < _maxIP)                                              // good correlation
	       && (vtx.Dist(thatTrack.front()) > _vtxToTrkStartDist)      // vertex far enough away from track start
	       && (sqrt(_geoAlgo.SqDist(vtx,thatTrack)) < _vtxToTrkDist)  // vertex close to track body
	       && (vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
	    {
	      // our shower comes from t -> not interested
	      single = false;
	      if (_verbose) { std::cout << "NOT single" << std::endl; }
	      break;
	    }
	  else if ( (IP < _maxIP)                                              // good correlation
		    && (vtx.Dist(thatTrack.front()) < _vtxToTrkStartDist)      // vertex close to track start
		    && (vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
	    {
	      // our shower has a common origin with this track
	      // save vertex position
	      _hassister = true;
	      // if the track is secondary (we investigated this before)
	      // then this shower should also be labeled as secondary -> not single
	      auto it = std::find(secondaryTracks.begin(), secondaryTracks.end(), t );
	      if ( it != secondaryTracks.end() ){
		if (_verbose) { std::cout << "common origin w/ secondary track -> not Single" << std::endl; }
		single = false;
		break;
	      } // if common with secondary
	      else{
		if (_useRadLength){
		  double vtxtostart = vtx.Dist(thisShower.Start());
		  if (_verbose) { std::cout << "common origin w/ primary! dEdx: " << thisShower._dedx
					    << "vtx-start: " << vtxtostart << std::endl; }
		  // Use Log-Likelihood for single-E w/ rad length info
		  if ( _alg_emp.LL(true, thisShower._dedx, vtxtostart) < _alg_emp.LL(false, thisShower._dedx, vtxtostart) ){
		    if (_verbose) { std::cout << "Shower is gamma-like. Ignore " << std::endl;
		      single = false;
		    }
		  }
		  else{
		    if (_verbose) { std::cout << "Shower is e-like. yay!" << std::endl; }
		    single = true;
		  }
		}// if use radLength
	      }// if common origin with primary!
	    }
	}// for all tracks
      }// if single
      
      // if still single (and no sister track) look at
      // dEdx to determine if e-like
      if (single && !_hassister){
	if ( ( _alg_emp.LL(true, thisShower._dedx, -1) < _alg_emp.LL(false, thisShower._dedx, -1) ) || 
	     (thisShower._dedx <= 0) || (thisShower._dedx > 10.) ){
	  if (_verbose) { std::cout << "Shower is single but gamma-like -> reject" << std::endl; }
	  single = false;
	}
      }
      //If single still true -> we found it! Proceed!
      // the particle with all it's info was already
      // created, simply add it to the result vector
      if (single){
	if (_verbose) { std::cout << "Shower is Single!" << std::endl; }
	// Create an "unknown" particle that gave
	// birth to this electron shower
	Particle electron(11,0.0005);
	electron.Vertex(thisShower.Start());
	electron.Momentum(thisShower.Dir());
	electron.RecoObjInfo(sh,Particle::RecoObjType_t::kShower);
	Particle unknown(12,0);
	unknown.Vertex(thisShower.Start());
	unknown.Momentum(thisShower.Dir());
	unknown.AddDaughter(electron);
	res.push_back(unknown);
	//res.push_back(p);
      }
      else
	if (_verbose) { std::cout << "Shower is not single." << std::endl; }
      
      
    }// for all showers

    return res;
  }
  
  void AlgoSingleE::ProcessEnd(TFile* fout){
    
    if(fout){
      fout->cd();
      
      if(_e_ll_values)
	_e_ll_values->Write();

      if(_dedx_values)
	_dedx_values->Write();

      if (_alg_tree)
	_alg_tree->Write();
    }

    return;
  }



  void AlgoSingleE::ClearTree(){

    _E = -1;
    _PDG = -1;
    _VsTrack = -1;
    _thatE = -1;
    _dEdx = -1;
    _IP = -1;
    _IPthisStart = -1;
    _IPthatStart = -1;
    _IPtrkBody = -1;

    return;
  }


}

#endif
