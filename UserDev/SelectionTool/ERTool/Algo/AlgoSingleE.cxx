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
    _verbose = false;
    _useEMPart = false;

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
    // Particles created in EM selection (photon/gamma separation)
    ParticleSet emParticles = _alg_emp.Reconstruct(data);
    
    //res = emParticles;

    if (_verbose) { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
      std::cout << "Particles in event: " << emParticles.size() << std::endl;
      std::cout << "dEdx: [";
      for (auto &s : emParticles)
	std::cout << s.PdgCode() << ", ";
      std::cout << "]" << std::endl;
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
    // loop over emParticles and select only the PDG==11 ones
    for (auto &p : emParticles){
      if ( (_useEMPart && (p.PdgCode() == 11) ) || (!_useEMPart) ){
	ClearTree();
	_PDG = p.PdgCode();
	_E   = p.Energy();
	_IPtrkBody = 0;
	// Ok, we have an electron. Get the associated shower
	// and make sure it does not:
	// NOTE: IN 1) IMPLEMENT EXCLUSION ONLY OF e-like showers
	// 1) share a vertex with another e- shower (gamma-like one could be from Pi0: OK)
	// 2) come from another shower - NOT DONE AS OF YET
	// 3) come from a track
	// 2) & 3) because we are interested in showers from
	// the neutrino interaction
	Shower thisShower = data.Shower( p.RecoObjID() );
	if (_verbose) { std::cout << "This shower: (" << p.RecoObjID() << ")" << "\tE: " << thisShower._energy << std::endl; }
	bool single = true;
	// loop over other showers and check 1) and 2)
	for (size_t s=0; s < data.Shower().size(); s++){
	  Shower thatShower(data.Shower(s));
	  geoalgo::Point_t vtx(3);
	  // make sure we don't use thisShower in the loop
	  if (thatShower._energy != thisShower._energy){
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
	    Track thatTrack(data.Track(t));
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
		// if the track is secondary (we investigated this before)
		// then this shower should also be labeled as secondary -> not single
		auto it = std::find(secondaryTracks.begin(), secondaryTracks.end(), t );
		if ( it != secondaryTracks.end() ){
		  if (_verbose) { std::cout << "common origin w/ secondary track -> not Single" << std::endl; }
		  single = false;
		  break;
		}
	      }
	  }// for all tracks
	}// if single
	
	//If single still true -> we found it! Proceed!
	// the particle with all it's info was already
	// created, simply add it to the result vector
	if (single){
	  if (_verbose) { std::cout << "Shower is Single!" << std::endl; }
	  // Create an "unknown" particle that gave
	  // birth to this electron shower
	  Particle unknown(12,0);
	  unknown.Vertex(thisShower.Start());
	  unknown.Momentum(thisShower.Dir());
	  unknown.AddDaughter(p);
	  res.push_back(unknown);
	  //res.push_back(p);
	}
	else
	  if (_verbose) { std::cout << "Shower is not single." << std::endl; }
	

      }// if the PDG is 11
    }// for all particles from EMPart

    /*
      if (_verbose) { std::cout << "Showers in event: " << data.Shower().size() << std::endl; }
      
      /// Get a list of the event showers that are electron like
      auto e_showers = ElectronLikeShowers( data.Shower() );
      
      if (_verbose) { std::cout << "e-like showers  : " << e_showers.size() << std::endl; }
      
      /// Get a list of the electron showers that are start-point-isolated
      auto isolated_e_showers = IsolatedStartPtShowers( e_showers );
      
      if (_verbose) { std::cout << "isolated showers: " << isolated_e_showers.size() << std::endl; }
      
      if(isolated_e_showers.size() == 1) single_e_counter++;
      
      /// Make an electron Particle for each independent shower and add it to the set
      for(auto const& isol_shower : isolated_e_showers){
      Particle p_e(11,_e_mass);
      p_e.Vertex(isol_shower->Start());
      p_e.Momentum(isol_shower->Dir() * (isol_shower->_energy - p_e.Mass())); // for now fill with direction - unit vector
      p_e.RecoObjInfo(isol_shower->ID(), Particle::RecoObjType_t::kShower);
      res.push_back(p_e);
      }
    */
    return res;
  }
  

  bool AlgoSingleE::IsShowerElectron(const ertool::Shower* shower){

    //Make sure the shower is likely an electron
    //by using LL function from an AlgoEMPart instance
    //(LL uses only dEdX data if "dist" is plugged in negative)
    //If likelihood its gamma is more than electron, skip
    
    //if dedx value == 0, we don't know whether this is electron,
    //so let's assume it isn't.
    if(shower->_dedx < 0.02 || shower->_dedx > 10.) return false;
    _dedx_values->Fill(shower->_dedx);
    

    
    nonzero_dedx_counter++;
    
    _e_ll_values->Fill(
      _alg_emp.LL(true,shower->_dedx,-1.));

    
    bool e_like = (
      _alg_emp.LL(true, shower->_dedx, -1.) >
      _alg_emp.LL(false,shower->_dedx, -1.)) ?
      true : false;
    
    if(e_like) total_e_showers++;
    else total_g_showers++;

    return e_like;
    
  }

  bool AlgoSingleE::AreShowersStartPtCorrelated(const ertool::Shower* s1, const ertool::Shower* s2){

    //Is the start point of s1 close to start point of s2?
    double dist = s1->Start().Dist(s2->Start());
    //Hard cut on 1cm for now. This value chosen after viewing some std::couts.
    //Later: use a PDF to determine the likelihood "dist" is small enough
    return  (dist < 1.) ? true : false;

  }

  const std::vector<const ertool::Shower*> AlgoSingleE::IsolatedStartPtShowers(const std::vector< const ::ertool::Shower*>& showers){

    //Function purpose: loop over electron showers, make sure they aren't
    //correlated (start points are close) w/ other electron showers
    std::vector<const ::ertool::Shower*> isolated_showers;
    isolated_showers.clear();

    //Save some time if the list of showers is length 0
    //(return empty vector)
    if(showers.size() == 0) return isolated_showers;

    //Save some time if the list of showers is length 1
    //(return a vector with just the index "0" in it)
    if(showers.size() == 1){
      isolated_showers.push_back(showers.at(0));
      return isolated_showers;
    }

    //For more than 1 electron showers, need to do some combinatorics
    //Strategy: Make a list of the shower indices that are bad (E.G. are close to other showers)
    //Then use that list to return only the good showers.
    std::vector<size_t> bad_shower_indices;

    for(size_t i = 0; i < showers.size()-1; ++i){
      for(size_t j = i+1; j < showers.size(); ++j){
	
	if(AreShowersStartPtCorrelated(showers.at(i),showers.at(j))){
	  //if list of showers that aren't independent doesn't already contain "i"
	  //add "i" to the list of showers
	  if( !(std::find(bad_shower_indices.begin(),bad_shower_indices.end(),i) != bad_shower_indices.end()) )
	    bad_shower_indices.push_back(i);

	  //if list of showers that aren't independent doesn't already contain "j"
	  //add "j" to the list of showers
	  if( !(std::find(bad_shower_indices.begin(),bad_shower_indices.end(),j) != bad_shower_indices.end()) )
	    bad_shower_indices.push_back(j);
	}
      }
    }


    //Now that we have a list of *bad* shower indices
    //Return a vector of *good* showers
    for(size_t idx = 0; idx < showers.size(); idx++)
      //if shower index is not in "bad showers", it is a good shower
      if( !(std::find(bad_shower_indices.begin(),bad_shower_indices.end(),idx) != bad_shower_indices.end()) )
	isolated_showers.push_back( showers.at(idx) );
    
    return isolated_showers;

  }


  const std::vector<const ertool::Shower*> AlgoSingleE::ElectronLikeShowers(const std::vector< const ::ertool::Shower*>& showers){

    std::vector< const ::ertool::Shower*> e_showers;
    e_showers.clear();

    //Loop over all showers in the event, store ones that are electron-like
    for(auto const& shower : showers) 
      if(IsShowerElectron(shower)) e_showers.push_back(shower);

    return e_showers;
  }

  void AlgoSingleE::ProcessEnd(TFile* fout){
    
    _alg_emp.ProcessEnd(fout);
    
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
