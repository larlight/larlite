#ifndef ERTOOL_ALGOSINGLEE_CXX
#define ERTOOL_ALGOSINGLEE_CXX

#include "AlgoSingleE.h"

namespace ertool {

  size_t single_e_counter = 0;
  size_t total_e_showers = 0;
  size_t total_g_showers = 0;
  size_t nonzero_dedx_counter = 0;
  
  AlgoSingleE::AlgoSingleE() : AlgoBase()
  {

    _name       = "AlgoSingleE";
    _e_mass     = TDatabasePDG().GetParticle(11)->Mass();
    _e_ll_values = 0;
    _dedx_values = 0;
    _verbose = false;

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

    //kaleko
    _alg_emp.SetMode(true);

    if(!_e_ll_values)
      _e_ll_values = new TH1F("e_ll_values","e_ll_values",1000,-1,0);

    if(!_dedx_values)
      _dedx_values = new TH1F("dedx_values","dedx_values",1000,0,8);

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

    res = emParticles;

    if (_verbose) { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      std::cout << "Showers in event: " << data.Shower().size() << std::endl;
    }

    // We are only interested in electron-like showers
    // loop over emParticles and select only the PDG==11 ones
    for (auto &p : emParticles){
      if (p.PdgCode() == 11){
	// Ok, we have an electron. Get the associated shower
	// and make sure it does not:
	// NOTE: IN 1) IMPLEMENT EXCLUSION ONLY OF e-like showers
	// 1) share a vertex with another e- shower (gamma-like one could be from Pi0: OK)
	// 2) come from another shower
	// 3) come from a track
	// 2) & 3) because we are interested in showers from
	// the neutrino interaction
	std::cout << "This shower: (" << p.RecoObjID() << ")" << std::endl;
	Shower thisShower = data.Shower( p.RecoObjID() );
	bool single = true;
	// loop over other showers and check 1) and 2)
	for (size_t s=0; s < data.Shower().size(); s++){
	  Shower thatShower(data.Shower(s));
	  geoalgo::Point_t vtx(3);
	  // make sure we don't use thisShower in the loop
	  if (thatShower._energy != thisShower._energy){
	    std::cout << "Comparing with shower (" << s << ")" << std::endl;
	    // check 1
	    if ( _findRel.haveCommonOrigin(thisShower,thatShower,vtx) ){
	      // showers have common origin -> not interested
	      single = false; 
	      break;
	    }
	    if ( _findRel.isShowerFromShower(thisShower,thatShower,vtx) ){
	      // our shower comes from s -> not interested
	      single = false;
	      break;
	    }
	  }
	}// for all showers
	// loop over tracks if still single
	if (single){
	  for (size_t t=0; t < data.Track().size(); t++){
	    Track thatTrack(data.Track(t));
	    std::cout << "Comparing with track (" << t << ")" << std::endl;
	    geoalgo::Point_t vtx(3);
	    if ( _findRel.isShowerFromTrack(thisShower,thatTrack,vtx) ){
	      // our shower comes from t -> not interested
	      single = false;
	      break;
	    }
	  }// for all tracks
	}// if single
	
	//If single still true -> we found it! Proceed!
	// the particle with all it's info was already
	// created, simply add it to the result vector
	if (single){
	  std::cout << "Shower is Single!" << std::endl;
	  // Create an "unknown" particle that gave
	  // birth to this electron shower
	  Particle unknown;
	  unknown.AddDaughter(p);
	  res.push_back(unknown);
	}
	else
	  std::cout << "Shower is not single." << std::endl;
	
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
    std::cout << std::endl;
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
    }

  }
}

#endif
