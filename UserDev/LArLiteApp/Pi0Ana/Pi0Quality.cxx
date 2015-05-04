#ifndef PI0QUALITY_CXX
#define PI0QUALITY_CXX

#include "Pi0Quality.h"

namespace larlite {

  Pi0Quality::Pi0Quality()
    : ERToolAnaBase()
    , _pi0_tree(nullptr) 
  { 
    _name="Pi0Quality"; 

  }

  bool Pi0Quality::initialize() {

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
    _pi0_tree->Branch("_start_res",&_start_res,"start_res/D");

    return ERToolAnaBase::initialize();
  }
  
  bool Pi0Quality::analyze(storage_manager* storage) {

    // get MCTruth to find Pi0
    auto ev_mctruth  = storage->get_data<event_mctruth>("generator");
    if(!ev_mctruth) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
      return false;
    }
  
    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    if(!_mgr.Process()) return false;

    auto const& particles = _mgr.ParticleSet();

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

      if (part.PdgCode() == 111){ // pi0!
	
	_reco_E = part.Energy();
	_reco_x = part.Vertex()[0];
	_reco_y = part.Vertex()[1];
	_reco_z = part.Vertex()[2];

	_start_res = sqrt( (_reco_x-_mc_x)*(_reco_x-_mc_x) + 
			   (_reco_y-_mc_y)*(_reco_y-_mc_y) + 
			   (_reco_z-_mc_z)*(_reco_z-_mc_z) );
	
	_pi0_tree->Fill();

      }// if pi0

    }// for all returned particles
    
    return true;  
  }
    
  bool Pi0Quality::finalize() {

    _pi0_tree->Write();

    return ERToolAnaBase::finalize();
  }

}
#endif
