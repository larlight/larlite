/*

AlgoSingleGamma, April 2015

authors : Joseph Zennamo, jzennamo@uchicago.edu

(Adapted/Stolen from AlgoSingleE, David Kaleko and David Caratelli)

This code is meant to select event topologies associated with a single 
photon search (and related topologies). 

It will return an ERTools ParticleSet associated with that neutrino interaction 


To Do list: 

1) Add muon PID
2) Add second (and so on) generations to the particle set 
      >> Can do this easily with the FindTrackHierarchy
3) More clever vertex reconstruction
4) Remove hardcoded variables (detector size, etc.)
5) Need to make sure thisShower is not the daughter of thatShower
6) Make "no_track_mama" no longer be not a positive (make it not a double negative)
7) Build an interative event vertex with a χ^2 value so we can reject a vertex with a lower χ^2
8) Need to collect sets of photons and check they aren't from pions and the like


Step-by-step Guide to the code: 

::  1  :: Start with a shower 
::  2  :: Check it is above the energy threshold
::  3  :: Make sure it hasn’t been counted before
::  4  :: Check that it is a photon, if not label as electron and continue, maybe Compton
::  5  :: Look for other showers which it is correlated to
::  6  :: If it is matched to a vertex containing an electron break! elec+gamma is boring.
::  7  :: If it is matched to a vertex with a photon, attach that photon to the event
::  8  :: Iterate through all the tracks
::  9  :: If shower originates from a track break 
:: 10  :: If shower has tracks associated with the start of the shower, build a vertex and add those tracks to the event
:: 11  :: With this vertex check that the “test” shower is a photon, if not break.
:: 12  :: Now build an event by adding all the particles in and calculate the neutrino/event kinematics 

*/
#ifndef ERTOOL_ALGOSINGLEGAMMA_CXX
#define ERTOOL_ALGOSINGLEGAMMA_CXX

#include "AlgoSingleGamma.h"

namespace ertool {
  
  size_t single_gam_counter = 0;
  size_t num_e_showers = 0;
  size_t num_g_showers = 0;
  size_t dedx_counter = 0;


  AlgoSingleGamma::AlgoSingleGamma() : AlgoBase()
				     , _alg_tree(nullptr)
				     , fTPC(-10.,-126.,-10.,292.,136.,1150.)
  {
    _name     = "AlgoSingleGamma";
    _e_mass     = TDatabasePDG().GetParticle(11)->Mass();
    _e_ll_values = 0;
    _dedx_values = 0;
    _Ethreshold = 0;
    _verbose = false;
    _useRadLength = false;
    _hassister = false;
    _rejectLongTracks = true;
    _vtxProximityCut = 0;
    _BDtW = 0; 
    _BDtTW = 0;

 
  }

  void AlgoSingleGamma::Reset()
  {
    std::cout<<__FUNCTION__<<" found "<<single_gam_counter<<" events with a single photon in it."<<std::endl;
    std::cout<<"and "<<dedx_counter<<" nonzero dedx showers"<<std::endl;
    std::cout<<"Found "<<num_e_showers<<" num e showers"<<std::endl;
    std::cout<<"Found "<<num_g_showers<<" num g showers"<<std::endl;

  }

  void AlgoSingleGamma::ProcessBegin()
  {
    _alg_emp.LoadParams();
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);

    if(!_e_ll_values)
      _e_ll_values = new TH1F("e_ll_values","e_ll_values",1000,-1,0);
    
    if(!_dedx_values)
      _dedx_values = new TH1F("dedx_values","dedx_values",1000,0,8);
    
    if (_alg_tree) { delete _alg_tree; }
    _alg_tree = new TTree("_alg_tree","Algo SingleGamma Tree");
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
    
    return;

  }
  
  void AlgoSingleGamma::LoadParams(std::string fname, size_t version)
  {
    //Load EMPart params
    _alg_emp.LoadParams(fname,version);
    
    return;
  }


  ParticleSet AlgoSingleGamma::Reconstruct(const EventData &data)
  { 



    //Name a ParticleSet that will contain all the particles 
    //associated with our selected single photon event
    ParticleSet SingleGamEvent;

    if(data.Shower().size() == 0){ std::cout << "No Shower." << std::endl; return SingleGamEvent;}
      
    if (_verbose) { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      if(data.Shower().size() != 0 or data.Track().size() != 0){
	std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
	std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
      }
    }
  


    std::vector<int> showers_counted; 

    /*
      :::::::::
      ::  1  ::
      :::::::::
    */
    // loop over showers
    for (auto & thisShower : data.Shower()){
      
      // Keep track of list of kin found for the shower (will be used to reconstruct full neutrino interaction)
      std::vector<int> shower_kin;
      std::vector<int> track_kin;
      TVector3 evt_vtx;

      // if we find that this shower shares a vertex with a track -> change "_hassister" to true.
      _track_assister = false;
      _elec_assister = false;
      _muon_assister = false;
      _gam_assister = false;
      _vtx_assister = false;

    /*
      :::::::::
      ::  2  ::
      :::::::::
    */

      // Apply energy threshold (separate from the ERToolHelper threshold. Only used in this algorithm)
      if (thisShower->_energy < _Ethreshold ) continue;

    /*
      :::::::::
      ::  3  ::
      :::::::::
    */

      //Check if this shower has already been added to a particle tree
      if(std::find(std::begin(showers_counted),std::end(showers_counted),thisShower->ID()) != std::end(showers_counted)) continue; 
      
      if (_verbose) { std::cout << "This shower: (" << thisShower->ID() << ")" << "\tE: " << thisShower->_energy << std::endl; }

      bool no_elec = true;  
      bool no_track_mama = true;
      bool elec = false;


    /*
      :::::::::
      ::  4  ::
      :::::::::
    */
      if(isGammaLike(thisShower->_dedx,-1)) elec = false;
    

    /*
      :::::::::
      ::  5  ::
      :::::::::
    */

      //loop over showers and look for other showers coming from a vertex, 
      //making sure there are no electrons and adding all the photons 
      for(auto & thatShower: data.Shower()){
	
	//define the interaction vertex
	geoalgo::Point_t vtx(3);
      
	//don't check a shower against itself
	if(thatShower->ID() == thisShower->ID()) {continue;}
      
	//compare the shower with the electron shower
	double IP = _findRel.FindClosestApproach(*thisShower,*thatShower,vtx);
	_VsTrack = 0;
	_thatE = thatShower->_energy;
	_IP = IP;
	_IPthisStart = vtx.Dist(thisShower->Start());
	_alg_tree->Fill(); 

	if (_verbose)
	  std::cout << "\tImpact Parameter      : " << _IP << std::endl
		    << "\tIP to other Shr Start : " << _IPthatStart << std::endl
		    << "\tIP to this Shr Start  : " << _IPthisStart << std::endl;



	if( (IP < _maxIP) and 
	    ( vtx.Dist(thatShower->Start()) < _vtxToShrStartDist) and
	    ( vtx.Dist(thisShower->Start()) < _vtxToGammShrStartDist)){

    /*
      :::::::::
      ::  6  ::
      :::::::::
    */

	  if(isGammaLike((*thatShower)._dedx,vtx.Dist((*thatShower).Start()))) 
	    no_elec = true;
	  else
	    no_elec = false;	

    /*
      :::::::::
      ::  7  ::
      :::::::::
    */
	  
	  if(no_elec){
	    shower_kin.push_back((*thatShower).ID());
	    showers_counted.push_back((*thatShower).ID());
	  }
	  else{
	    if(_verbose) {std::cout << "Matched to an electron. \t\t\t ::NOT SIGNAL::" << std::endl;}
	    break;} 
	}
	//**** (1) is Satified
	//****     thisShower does not share a vertex with an electron 
	::geoalgo::HalfLine shr_vector(thisShower->Start(),thisShower->Dir());
	_distBackAlongTraj = sqrt(thisShower->Start().SqDist(_geoAlgo.Intersection(fTPC,shr_vector,true)[0]));
      	
	//Distance to Top Wall definition, page 19 of DocDB 3978-v2
	// DANGER DANGER DANGER Hard coded detector size
	double detHalfHeight = 116.5;
	_distToTopWall = ((*thisShower).Start()[1] - detHalfHeight)*((*thisShower).Dir().Length())/((*thisShower).Dir()[1]);
      }//Loop over all OTHER showers

    /*
      :::::::::
      ::  8  ::
      :::::::::
    */

      //Loop over tracks 
      // Check (2), if shower is daughter of a track	
      for(auto & thatTrack : data.Track()){
	if(thatTrack->size() < 2) continue; //Because short tracks that we cannot resolve don't matter.
	
	if(_verbose) {std::cout << "Comparing with track (" << thatTrack->ID() << ")" << std::endl;}
	geoalgo::Point_t vtx(3);
	
	// compare the track and the shower
	double IP = _findRel.FindClosestApproach(*thisShower,*thatTrack,vtx);
	_VsTrack = 1;
	_thatE = thatTrack->_energy;
	_IP = IP;
	_IPthisStart = vtx.Dist(thisShower->Start());
	_IPthatStart = vtx.Dist(thatTrack->front());
	_IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,*thatTrack));
	_alg_tree->Fill();
	
	if(_verbose)
	  std::cout << "\tImpact Parameter: " << _IP << std::endl
		    << "\tIP to Trk Start : " << _IPthatStart << std::endl
		    << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		    << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	
	no_track_mama = true;

    /*
      :::::::::
      ::  9  ::
      :::::::::
    */
	if( (IP < _maxIP) and
	    (vtx.Dist((*thatTrack).front()) > _vtxToTrkStartDist) and  // No chance that the track and the shower share that vertex!
	    (sqrt(_geoAlgo.SqDist(vtx,*thatTrack)) < _vtxToTrkDist) and
	    (vtx.Dist((*thisShower).Start()) < _vtxToShrStartDist) )
	  {
	    //This shower has 
	    no_track_mama = false;
	    if(_verbose) {std::cout << "This track has a mother which is a track! \t\t\t ::NOT SIGNAL::" << std::endl;}
	    break;
	  }
    /*
      :::::::::
      :: 10  ::
      :::::::::
    */
	else if( (IP < _maxIP) and
		 (vtx.Dist((*thatTrack).front()) < _vtxToTrkStartDist) and
		 (vtx.Dist((*thisShower).Start()) < _vtxToShrStartDist)
		 ){
	  
	  //this Shower has a track sibling 
	  
	  _vtx_assister = true;
	  evt_vtx.SetXYZ(vtx[0],vtx[1],vtx[2]);
	  track_kin.push_back((*thatTrack).ID());

    /*
      :::::::::
      :: 11  ::
      :::::::::
    */
	  
	  if(!isGammaLike((*thisShower)._dedx,vtx.Dist((*thisShower).Start()))) {
	    elec = true;
	    if(_verbose) {std::cout << "Matched to an electron. \t\t\t ::NOT SIGNAL::" << std::endl;}
	    break;
	  }
	  else
	    elec = false;

	  

	  /* isTrackMuon?... NEED to add this, muon = true; */	  
	}
	///NEED TO ADD A LOOP TO CHECK ALL SHOWER KIN AGAINST THAT VERTEX

      }//Loop over all tracks
    

    /*
      :::::::::
      :: 12  ::
      :::::::::
    */

      if(!elec and _vtx_assister){
	
	//Create a "neutrino" particle which anchors the event

	Particle neutrino(0,0);
	neutrino.Vertex(evt_vtx);

	//Define the neutrino energy and momentum
	geoalgo::Vector_t nuMom(0.,0.,0.);
	double nuKE = 0;

	//If there is a muon add set the neutrino flavor
	if(_muon_assister){
	  neutrino.SetPdgCode(14);
	}

	//if there is an electron, you did it wrong...
	if(_elec_assister){
	  std::cout << "Um, are you sure? Because I don't think there should be an electron. Maybe you should check your code and make sure you did everything right, because I am pretty sure you didn't but, you know...good luck with that!" << std::endl;
	}

	//Go through and add all the primary photons to the event
	for(auto s : shower_kin){	  
	  auto const& shower(data.Shower(s));

	  Particle photon(22,0);
	  photon.Vertex(shower.Start());
	  double mom = shower._energy;
	  if(mom < 0) mom = 0;
	  photon.Momentum(shower.Dir()*mom);
	  nuKE  += mom;
	  nuMom += photon.Momentum();
	  photon.RecoObjInfo(shower.ID(), Particle::RecoObjType_t::kShower);
	  neutrino.AddDaughter(photon);	  
	}

	for(auto t : track_kin){
	  auto const& track(data.Track(t));

	  //define your track as a particle 
	  Particle PiKProMu = _findRel.GetPDG(track);

	  //it starts where the track starts 
	  PiKProMu.Vertex(track[0]);

	  //define the particles kinimatics
	  double engy = track._energy;
	  if(engy < 0) engy = 0;
	  PiKProMu.Momentum(track.Dir()*(sqrt(engy+2*PiKProMu.Mass())));
	  nuKE += engy;
	  nuMom += PiKProMu.Momentum();
	  PiKProMu.RecoObjInfo(track.ID(), Particle::RecoObjType_t::kTrack);
	  neutrino.AddDaughter(PiKProMu);
	 
	}

	nuMom.Normalize();
	neutrino.Momentum(nuMom*nuKE);
	
	if(fTPC.Contain(neutrino.Vertex())){
	  SingleGamEvent.push_back(neutrino);
	}
      }
      else{
	if(_verbose){
	  if(elec){
	    std::cout << "Event has an electron! \t\t\t\t ::NOT SIGNAL::" << std::endl; }
	  else if(!_vtx_assister){
	    std::cout << "Event has no vertex activity! \t\t\t\t ::Possibly Signal...but not counted::" << std::endl; }
	}}
    }//Loop over all showers
      
    return SingleGamEvent; }

  void AlgoSingleGamma::ProcessEnd(TFile *fout){
    
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


  bool AlgoSingleGamma::isGammaLike(const double dedx, double radlen, bool forceRadLen)
  {
    if ( !_useRadLength and !forceRadLen )
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


  void AlgoSingleGamma::ClearTree(){
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
