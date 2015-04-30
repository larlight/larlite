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
    _verbose  = "false"; 
 
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

    
    if (_verbose) { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
    }

    // Reconstruct Track Hierarchy
    ParticleSet trackHierarchy;
    trackHierarchy = _findRel.FindTrackHierarchy(data.Track());

    // loop over showers
    for (auto const& thisShower : data.Shower()){
      
      // Ok, we have a shower.
      // make sure it does not:
      // 1) share a vertex with another e- shower (gamma-like one could be from Pi0: OK)
      // 2) come from another shower - NOT DONE AS OF YET
      // 3) come from a track
      // 2) & 3) because we are interested in showers from the neutrino interaction

      // Keep track of list of siblings found for the shower (will be used to reconstruct full neutrino interaction)
      std::vector<int> siblings;
     
      // if we find that this shower shares a vertex with a track -> change "_hassister" to true.
      _hassister = false;

      // Apply energy threshold (separate from the ERToolHelper threshold. Only used in this algorithm)
      if (thisShower->_energy < _Ethreshold ) continue;
      
      if (_verbose) { std::cout << "This shower: (" << thisShower->ID() << ")" << "\tE: " << thisShower->_energy << std::endl; }
      bool single = true;
      
      //loop over showers and check 1) and 2) 
      for(auto const& thatShower: data.Shower()){
	


      }


    }
    return ParticleSet(); }

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



}

#endif
