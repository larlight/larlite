#ifndef SPANAPI0QUALITY_CXX
#define SPANAPI0QUALITY_CXX

#include "SPAnaPi0Quality.h"

namespace larlite {


  SPAnaPi0Quality::SPAnaPi0Quality()
    : SPTAnaBase()
    , _pi0_tree(nullptr) 
  { 
    _name="SPAnaPi0Quality"; 

  }

  bool SPAnaPi0Quality::initialize() {

    // Setup Tree to hold Quality Check information
    if (_pi0_tree) { delete _pi0_tree; }
    _pi0_tree = new TTree("_pi0_tree","Pi0 Tree");
    _pi0_tree->Branch("_mc_E",&_mc_E,"mc_E/D");
    _pi0_tree->Branch("_reco_E",&_reco_E,"reco_E/D");
    _pi0_tree->Branch("_mc_x",&_mc_x,"mc_x/D");
    _pi0_tree->Branch("_reco_x",&_reco_x,"reco_x/D");
    _pi0_tree->Branch("_mc_y",&_mc_y,"mc_y/D");
    _pi0_tree->Branch("_reco_y",&_reco_y,"reco_y/D");
    _pi0_tree->Branch("_mc_z",&_mc_z,"mc_z/D");
    _pi0_tree->Branch("_reco_z",&_reco_z,"reco_z/D");

    _mgr.Initialize();
    return SPTAnaBase::initialize();
  }
  
  bool SPAnaPi0Quality::analyze(storage_manager* storage) {

    // get MCTruth to find Pi0
    auto ev_mctruth  = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }
  
    auto status = SPTAnaBase::analyze(storage);
    if(!status) return status;

    sptool::SPArticleSet particles = _mgr.Process(_data);

    int n_pi0 = 0;
    larlite::mcpart pion;
    for(auto const& particle : ev_mctruth->at(0).GetParticles()){

      double KE = particle.Trajectory().at(0).E()-particle.Mass();
      if(abs(particle.PdgCode()) == 111 && KE > 0.02){ //GeV
	pion = particle;
	n_pi0++;
      }

    }
    // Makse sure again there is only 1 Pi0
    if (n_pi0 != 1)
      return true;
    
    // if so, fill trees with MC & _data output
    mcstep start = pion.Trajectory().at(0);
    _mc_E = start.E()*1000.;
    _mc_x = start.X();
    _mc_y = start.Y();
    _mc_z = start.Z();
    
    // fill an entry per pi0 returned by _data
    for (auto const& part : particles){

      if (part.pdg_code() == 111){ // pi0!
	
	_reco_E = part.energy();
	_reco_x = part.pos()[0];
	_reco_y = part.pos()[1];
	_reco_z = part.pos()[2];
	
	_pi0_tree->Fill();

      }// if pi0

    }// for all returned particles


    
    return true;  
  }
    
  bool SPAnaPi0Quality::finalize() {

    _pi0_tree->Write();

    _mgr.Finalize(_fout);
    return SPTAnaBase::finalize();
  }

}
#endif
