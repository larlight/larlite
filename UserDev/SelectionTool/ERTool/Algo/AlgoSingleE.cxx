#ifndef ERTOOL_ALGOSINGLEE_CXX
#define ERTOOL_ALGOSINGLEE_CXX
#include "AlgoSingleE.h"

namespace ertool {

  size_t single_e_counter = 0;
  size_t total_e_showers = 0;
  size_t total_g_showers = 0;
  size_t nonzero_dedx_counter = 0;
  
  AlgoSingleE::AlgoSingleE(const std::string& name)
    : AlgoBase(name)
    , fTPC(-0.,-20.,0.,47.,20.,90.)
    , _empart_tree(nullptr)
    , _alg_tree(nullptr)
  {
    _e_mass     = ParticleMass(11);
    _Ethreshold = 0;
    _verbose = false;
    _useRadLength = false;
    _hassister = false;
    _rejectLongTracks = true;
    _vtxProximityCut = 0;
    _BDtW = 0; 
    _BDtTW = 0;
    _neutrinos = 0;

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

    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);

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
    _alg_tree->Branch("_distBackAlongTraj",&_distBackAlongTraj,"_distBackAlongTraj/D");
    _alg_tree->Branch("_distToTopWall",&_distToTopWall,"_distToTopWall/D");
    
    if (_empart_tree) { delete _empart_tree; }
    _empart_tree = new TTree("_empart_tree","EMPart gamma/electron Tagging Tree");
    _empart_tree->Branch("_dedx",&_dedx,"dedx/D");
    _empart_tree->Branch("_radlen",&_radlen,"radlen/D");
    _empart_tree->Branch("_pdg",&_pdg,"pdg/I");

    return;
  }

  void AlgoSingleE::AcceptPSet(const ::fcllite::PSet& cfg) {
    
    // Load EMPart params
    _alg_emp.AcceptPSet(cfg);
    
    return;
  }

  bool AlgoSingleE::Reconstruct(const EventData &data, ParticleGraph& graph){

    auto datacpy = data;

    if (_verbose) { 
      std::cout << "*********** BEGIN SingleE RECONSTRUCTION ************" << std::endl;
      std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
    }

    // We have a list of primaries.
    // Filter them to select single e- showers

    // loop through primary showers
    // Loop through showers
    for (auto const& p : graph.GetPrimaryNodes(RecoType_t::kShower)){
      
      auto const& thisShower = datacpy.Shower(graph.GetParticle(p).RecoID());
      // keep track of whether it is single
      bool single = true;
      // if we find that this shower shares a vertex with a track -> change "_hassister" to true.
      _hassister = false;
      // Keep track of list of siblings found for the shower (will be used to reconstruct full neutrino interaction)
      std::vector<int> siblings;

      if (_verbose) { std::cout << "This shower: (" << p << ")" << "\tE: " << thisShower._energy << std::endl; }

      // it is primary. Make sure it satisfies SingleE conditions also
      // 1) loop over all showers in event
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){
	
	auto const& thatShower = datacpy.Shower(graph.GetParticle(p2).RecoID());
	geoalgo::Point_t vtx(3);
	// make sure we don't use "thisShower" in the loop
	if (thatShower.RecoID() == thisShower.RecoID()) 
	  continue;
	if (_verbose) { std::cout << "Comparing with shower (" << p2 << ")" << std::endl; }
	// is "thatshower" gamma or e-like?
	// if gamma-like maybe a nearby pi0 -> ok if close
	if (isGammaLike(thatShower._dedx,-1))
	  continue;
	// "thatShower" is e-like. If the two are correlated and siblings
	// then we have two e-like showers at the vertex -> do not count
	// "thisShower" as a SingleE
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
      }// loop over all showers in event

      // if still single go over tracks. Find potential siblings
      if (!single)
	continue;

      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
	
	auto const& thatTrack = datacpy.Track(graph.GetParticle(t).RecoID());
	// make sure track has a length of at least 0.3 cm (wire spacing)
	// greater longer than 3 mm
	if (thatTrack.Length() < 0.3)
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
	if (_verbose) {
	  std::cout << "\tImpact Parameter: " << _IP << std::endl
		    << "\tIP to Trk Start : " << _IPthatStart << std::endl
		    << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		    << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	}
	single = true;
	if ( (IP < _maxIP)                                              // good correlation
	     && (vtx.Dist(thatTrack.front()) < _vtxToTrkStartDist)      // vertex close to track start
	     && (vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
	  {
	    // our shower has a common origin with this track
	    // they are siblings
	    if (_verbose) { std::cout << "common origin w/ track!" << std::endl; }
	    _hassister = true;
	    siblings.push_back(t);
	    
	    if (isGammaLike(thisShower._dedx,vtx.Dist(thisShower.Start()))){
	      _dedx   = thisShower._dedx;
	      _radlen = vtx.Dist(thisShower.Start());
	      _pdg    = 22;
	      if (_verbose) { std::cout << "Shower is gamma-like. Ignore " << std::endl; }
	      single = false;
	      break;
	    }
	    else{
	      _dedx   = thisShower._dedx;
	      _radlen = vtx.Dist(thisShower.Start());
	      _pdg    = 11;
	      single = true;
	    }
	    _empart_tree->Fill();
	    
	  }// if common origin with primary!
      }// for all tracks

      // If single and there are "track-candidate-vertices" formed by 2 or more tracks
      // compare shower start distance to these vertices.
      // if more than some threshold not single
      double distmin = 1036;
      // get list of potential vertices that come from 2 or more objects
      // sharing a start point
      auto const& candidateVertices = _findRel.GetVertices(graph,2);
      for (auto const& vertex : candidateVertices){
	double thisdist = thisShower.Start().Dist(vertex);
	if ( thisdist < distmin)
	  distmin = thisdist;
      }

      if ( single and !_hassister and (_vtxProximityCut != 0) ){
	if ( (distmin != 1036) and (distmin > _vtxProximityCut) ){
	  if(_verbose) { std::cout << "Trk-Vtx found @ distance of " << distmin << ". Shower not single!" << std::endl; }
	  single = false;
	}
      }


      // Try and remove any shower that is on top of a track
      // this could be due to a track mis-reconstructed as a 
      // shower.

      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
	
	auto const& track = datacpy.Track(graph.GetParticle(t).RecoID());
	
	// check if track and shower start points are close
	if (thisShower.Start().Dist(track[0]) > 1 )
	  continue;
	// if their start point is very close:
	// calculate a stupid track direction

	size_t nsteps = track.size();
	::geoalgo::Vector_t trackDir = track[int(nsteps/2.)]-track[0];
	trackDir.Normalize();
	// get dot product
	//double dot = thisShower.Dir()*trackDir;
	//if (dot > 0.9)
	//single = false;
      }

      // if still single (and no sister track) look at
      // dEdx to determine if e-like
      if (single && !_hassister){
	if ( isGammaLike(thisShower._dedx,-1) || (thisShower._dedx <= 0) || (thisShower._dedx > 10.) ){
	  if (_verbose) { std::cout << "Shower is single but gamma-like -> reject" << std::endl; }
	  single = false;
	}
      }

      //If single still true -> we found it! Proceed!
      // the particle with all it's info was already
      // created, simply add it to the result vector
      if (single){
	if (_verbose) { std::cout << "Shower is Single!" << std::endl; }

	// prepare holder for neutrino momentum
	//::geoalgo::Vector_t neutrinoMom(0,0,0);
	double neutrinoMom = 0;

	// fill in electron properties
	double mom = sqrt( (thisShower._energy)*(thisShower._energy) - (_e_mass*_e_mass) );
	if (mom < 0) { mom = 1; }
	if (_verbose) { std::cout << "Getting shower " << p << std::endl; }
	auto& electron = graph.GetParticle(p);
	if (_verbose) { std::cout << " and modifying properties..." << std::endl; }
	electron.SetParticleInfo(11,_e_mass,thisShower.Start(),thisShower.Dir()*mom);
	// create a new particle for the neutrino!
	if (_verbose) { std::cout << "number of partciles before: " << graph.GetNumParticles() << std::endl; }
	if (_verbose) { std::cout << "Making neutrino..." << std::endl; }
	Particle& neutrino = graph.CreateParticle();
	neutrinoMom += mom;//thisShower.Dir()*mom;
	neutrino.SetParticleInfo(12,0.,thisShower.Start(),thisShower.Dir()*mom);
	if (_verbose) { std::cout << "made neutrino with ID " << neutrino.ID() << " and PDG: " << neutrino.PdgCode() << std::endl; }
	if (_verbose) { std::cout << "number of partciles after: " << graph.GetNumParticles() << std::endl; }
	_neutrinos += 1;
	// set relationship
	graph.SetParentage(neutrino.ID(),p);

	// Now look for all potential siblins
	// using AlgoFindRelationship
	for (auto const& t : graph.GetPrimaryNodes(RecoType_t::kTrack)){
	  
	  auto const& track = datacpy.Track(graph.GetParticle(t).RecoID());
	  // make sure track has a length of at least 0.3 cm (wire spacing)
	  // greater longer than 3 mm
	  if (track.Length() < 0.3)
	    continue;

	  ::geoalgo::Point_t vtx(3);
	  double score = -1;
	  auto const& rel = _findRel.FindRelation(thisShower,track,vtx,score);
	  if (rel == kSibling)
	    { // add this track to PaticleTree
	      auto &trackParticle = graph.GetParticle(t);
	      // if not primary according to primary finder -> don't add
	      if (!trackParticle.Primary())
		continue;
	      // track deposited energy
	      double Edep = track._energy;
	      // track direction
	      geoalgo::Vector_t Dir = (track[1]-track[0]);
	      Dir.Normalize();
	      double mass = _findRel.GetMass(track);
	      geoalgo::Vector_t Mom = Dir * ( sqrt( Edep * (Edep+2*mass) ) );
	      trackParticle.SetParticleInfo(_findRel.GetPDG(track),mass,track[0],Mom);
	      neutrinoMom += sqrt( Edep * ( Edep + 2*mass ) );
	      graph.SetParentage(neutrino.ID(),t);
	    }
	}

	::geoalgo::Vector_t momdir(0,0,1);

	neutrino.SetParticleInfo(12,0.,thisShower.Start(),momdir*neutrinoMom);

	

      }// if single
      // if not single
      else
	if (_verbose) { std::cout << "Shower is not single." << std::endl; }


      
    }// for all primaries found / for all showers
    
    return true;
  }
  
  void AlgoSingleE::ProcessEnd(TFile* fout){

    std::cout << "Num. of neutrinos found: " << _neutrinos << std::endl;
    
    if(fout){
      fout->cd();
      
      if (_alg_tree)
	_alg_tree->Write();
      if (_empart_tree)
	_empart_tree->Write();
    }

    return;
  }

  
  bool AlgoSingleE::isGammaLike(const double dedx, double radlen, bool forceRadLen)
  {
    if ( !_useRadLength && !forceRadLen )
      radlen = -1;
    if ( _alg_emp.LL(true, dedx, radlen) < _alg_emp.LL(false, dedx, radlen) )
      {
	if (_verbose) {
	  std::cout << "Shower has dEdx = " << dedx
		    << "\tRadLen = " << radlen
		    << "\tIgnore for comparison." << std::endl;
	}
	return true;
      }
    
    return false;
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
    _distBackAlongTraj = -1;
    _distToTopWall  = -999999;

    return;
  }


}

#endif
