#ifndef ERTOOL_ERANAPI0SELECTION_CXX
#define ERTOOL_ERANAPI0SELECTION_CXX

#include "ERAnaPi0Selection.h"

namespace ertool {

  ERAnaPi0Selection::ERAnaPi0Selection() : AnaBase()
			       , _pi0_tree(nullptr)

  {
    _name     = "ERAnaPi0Selection";

    if (_pi0_tree) { delete _pi0_tree; }
    _pi0_tree = new TTree("_pi0_tree","Pi0 Tree");

    _pi0_tree->Branch("_numEvts",&_numEvts,"numEvts/I");
    _pi0_tree->Branch("_n_showers",&_n_showers,"n_showers/I");
    _pi0_tree->Branch("_n_showersReco",&_n_showersReco,"n_showersReco/I");
    _pi0_tree->Branch("_n_gammas",&_n_gammas,"n_gammas/I");
    _pi0_tree->Branch("_n_gammasReco",&_n_gammasReco,"n_gammasReco/I");

    _pi0_tree->Branch("_e_nu_MC",&_e_nu_MC,"e_nu_MC/D");
    _pi0_tree->Branch("_x_nu_MC",&_x_nu_MC,"x_nu_MC/D");
    _pi0_tree->Branch("_y_nu_MC",&_y_nu_MC,"y_nu_MC/D");
    _pi0_tree->Branch("_z_nu_MC",&_z_nu_MC,"z_nu_MC/D");
    _pi0_tree->Branch("_px_nu_MC",&_px_nu_MC,"px_nu_MC/D");
    _pi0_tree->Branch("_py_nu_MC",&_py_nu_MC,"py_nu_MC/D");
    _pi0_tree->Branch("_pz_nu_MC",&_pz_nu_MC,"pz_nu_MC/D");
    _pi0_tree->Branch("_pdg_nu_MC",&_pdg_nu_MC,"pdg_nu_MC/I");
    _pi0_tree->Branch("_e_gamma_MC",&_e_gamma_MC,"e_gamma_MC/D");
    _pi0_tree->Branch("_pdg_gamma_MC",&_pdg_gamma_MC,"pdg_gamma_MC/I");
    _pi0_tree->Branch("_x_gamma_MC",&_x_gamma_MC,"x_gamma_MC/D");
    _pi0_tree->Branch("_y_gamma_MC",&_y_gamma_MC,"y_gamma_MC/D");
    _pi0_tree->Branch("_z_gamma_MC",&_z_gamma_MC,"z_gamma_MC/D");
    _pi0_tree->Branch("_px_gamma_MC",&_px_gamma_MC,"px_gamma_MC/D");
    _pi0_tree->Branch("_py_gamma_MC",&_py_gamma_MC,"py_gamma_MC/D");
    _pi0_tree->Branch("_pz_gamma_MC",&_pz_gamma_MC,"pz_gamma_MC/D");
    _pi0_tree->Branch("_theta_gamma_MC",&_theta_gamma_MC,"theta_gamma_MC/D");
    _pi0_tree->Branch("_phi_gamma_MC",&_phi_gamma_MC,"phi_gamma_MC/D");
    _pi0_tree->Branch("_gamma_score_MC",&_gamma_score_MC,"gamma_score_MC/D");

    _pi0_tree->Branch("_e_nu_Reco",&_e_nu_Reco,"e_nu_Reco/D");
    _pi0_tree->Branch("_x_nu_Reco",&_x_nu_Reco,"x_nu_Reco/D");
    _pi0_tree->Branch("_y_nu_Reco",&_y_nu_Reco,"y_nu_Reco/D");
    _pi0_tree->Branch("_z_nu_Reco",&_z_nu_Reco,"z_nu_Reco/D");
    _pi0_tree->Branch("_px_nu_Reco",&_px_nu_Reco,"px_nu_Reco/D");
    _pi0_tree->Branch("_py_nu_Reco",&_py_nu_Reco,"py_nu_Reco/D");
    _pi0_tree->Branch("_pz_nu_Reco",&_pz_nu_Reco,"pz_nu_Reco/D");
    _pi0_tree->Branch("_n_pi0",&_n_pi0,"n_pi0/I");
    _pi0_tree->Branch("_e_gamma_Reco",&_e_gamma_Reco,"e_gamma_Reco/D");
    _pi0_tree->Branch("_theta_gamma_Reco",&_theta_gamma_Reco,"theta_gamma_Reco/D");
    _pi0_tree->Branch("_phi_gamma_Reco",&_phi_gamma_Reco,"phi_gamma_Reco/D");
    _pi0_tree->Branch("_x_gamma_Reco",&_x_gamma_Reco,"x_gamma_Reco/D");
    _pi0_tree->Branch("_y_gamma_Reco",&_y_gamma_Reco,"y_gamma_Reco/D");
    _pi0_tree->Branch("_z_gamma_Reco",&_z_gamma_Reco,"z_gamma_Reco/D");
    _pi0_tree->Branch("_px_gamma_Reco",&_px_gamma_Reco,"px_gamma_Reco/D");
    _pi0_tree->Branch("_py_gamma_Reco",&_py_gamma_Reco,"py_gamma_Reco/D");
    _pi0_tree->Branch("_pz_gamma_Reco",&_pz_gamma_Reco,"pz_gamma_Reco/D");
    _pi0_tree->Branch("_gamma_score_Reco",&_gamma_score_Reco,"gamma_score_Reco/D");
    _pi0_tree->Branch("_mom_dot",&_mom_dot,"_mom_dot/D");
    _pi0_tree->Branch("_vtx_dist",&_vtx_dist,"_vtx_dist/D");

    _pi0_tree->Branch("_misID",&_misID,"misID/I");
    _pi0_tree->Branch("_n_psReco",&_n_psReco,"n_psReco/I");

    _singlePi0_ctr = 0;
    _debug = true;

    _eCut = 0;
    _eff = 0.;
    _vtx_dist = 10000000;

  }

  void ERAnaPi0Selection::Reset()
  {}

  bool ERAnaPi0Selection::Analyze(const EventData &data, const ParticleSet &ps)
{

    // Get MC particle set, EventData(showers/tracks...)
    auto mc_ps = MCParticleSet();
    auto mc_data = MCEventData();

    // Reset tree variables
    ResetTreeVariables();

    // count number of gammas and showers(MC and Reco) in event
    _n_gammas    = 0;
    _n_showers   = 0;
    _n_showersReco = 0;
    _n_gammasReco = 0;

    _numEvts += 1;

    // size of ParticleSet should be the number of neutrinos found 
    _n_psReco = ps.size();

    // Count number of showers with E > _eCut MeV
    for (auto &s : data.Shower())
	if (s->_energy > _eCut) { _n_showersReco += 1; }

    for (auto &s : data.AllShower())
        if (s._energy > _eCut) { _n_showers += 1; }
        

    // If debug -> print out MC particle set
    if (_debug){
      std::cout << "MC Particle Tree: " << std::endl;
      for (auto &p : mc_ps)
	std::cout << p.Diagram();

      std::cout << "Reco Particle Tree: " << std::endl;
      for (auto &p : ps)
	std::cout << p.Diagram();
	}

    // loop over all particles in MC particle set (made by Helper)
    for (auto &p : mc_ps){
      // Find the pi0 and store its energy
      if (abs(p.PdgCode()) == 12 || abs(p.PdgCode() == 14)){
	_e_nu_MC = p.Energy();
	_x_nu_MC = p.Vertex()[0];
	_y_nu_MC = p.Vertex()[1];
	_z_nu_MC = p.Vertex()[2];
	_px_nu_MC = p.Momentum()[0];
	_py_nu_MC = p.Momentum()[1];
	_pz_nu_MC = p.Momentum()[2];
	_pdg_nu_MC = p.PdgCode();

	// std::cout<<"\n\nneutrino: "<<p.PdgCode() <<" and size of daughers: "<<p.AllDaughters().size()<<std::endl ;
	for (auto &pi0 : p.AllDaughters()){
	  if (pi0->PdgCode() == 111){  //  || gamma->PdgCode() == 22 ) // 
	    _n_pi0 += 1;
	    for(int i = 0; i < 2 ; i++){
	    
	      _n_gammas += 1;
	      auto gamma = pi0->Daughters().at(i);

	      _e_gamma_MC = gamma.Energy();
	      _x_gamma_MC = gamma.Vertex()[0];
	      _y_gamma_MC = gamma.Vertex()[1];
	      _z_gamma_MC = gamma.Vertex()[2];
	      _px_gamma_MC = gamma.Momentum()[0];
	      _py_gamma_MC = gamma.Momentum()[1];
	      _pz_gamma_MC = gamma.Momentum()[2];
	      _theta_gamma_MC = (180./3.14) * acos( _pz_gamma_MC / sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC + _pz_gamma_MC*_pz_gamma_MC ) );
	      _phi_gamma_MC   = (180./3.14) * asin( _py_gamma_MC / sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC ) );
	      _pdg_gamma_MC = gamma.PdgCode();
	      _gamma_score_MC = gamma.Score();

	      double temp_dist = 0 ;

	      _e_nu_Reco = 0;
	      for(auto const & reco : ps){
		if (abs(reco.PdgCode()) == 12 || abs(reco.PdgCode() == 14)){
		
		    for (auto &recod : reco.AllDaughters()){
			_e_nu_Reco += recod->KineticEnergy();
			if (abs(recod->PdgCode()) == 111){
			    _misID = 0 ;
			    _singlePi0_ctr++;
			    _x_nu_Reco = pi0->Vertex()[0];
        		    _y_nu_Reco = pi0->Vertex()[1];
        		    _z_nu_Reco = pi0->Vertex()[2];
        		    _px_nu_Reco = pi0->Momentum()[0];
        		    _py_nu_Reco = pi0->Momentum()[1];
        		    _pz_nu_Reco = pi0->Momentum()[2];
			    //std::cout<<"vertex of pi0: "<<_x_nu_Reco<<", "<<_y_nu_Reco<<", "<<_z_nu_Reco<<std::endl;
			    for(int i = 0; i < 2 ; i++){
			      _n_gammasReco++;
	    
			      auto gamma_R = recod->Daughters().at(i);
			      temp_dist = sqrt( pow(gamma_R.Vertex()[0] - _x_gamma_MC,2) 
					    + pow(gamma_R.Vertex()[1] - _y_gamma_MC,2) 
				    	    + pow(gamma_R.Vertex()[2] - _z_gamma_MC,2) );
			    
				    if(_vtx_dist > temp_dist){
					_vtx_dist = temp_dist;
					_e_gamma_Reco = gamma_R.Energy(); 
					_x_gamma_Reco = gamma_R.Vertex()[0];
				        _y_gamma_Reco = gamma_R.Vertex()[1];
				        _z_gamma_Reco = gamma_R.Vertex()[2];
				        _px_gamma_Reco = gamma_R.Momentum()[0];
				        _py_gamma_Reco = gamma_R.Momentum()[1];
				        _pz_gamma_Reco = gamma_R.Momentum()[2];
				        _theta_gamma_Reco = (180./3.14) * acos( _pz_gamma_Reco / sqrt( _px_gamma_Reco*_px_gamma_Reco + _py_gamma_Reco*_py_gamma_Reco + _pz_gamma_Reco*_pz_gamma_Reco ) );
				        _phi_gamma_Reco   = (180./3.14) * asin( _py_gamma_Reco / sqrt( _px_gamma_Reco*_px_gamma_Reco + _py_gamma_Reco*_py_gamma_Reco ) );
					_gamma_score_MC = gamma_R.Score();

				        _mom_dot = ( ( _px_gamma_MC*_px_gamma_Reco + _py_gamma_MC*_py_gamma_Reco + _pz_gamma_MC*_pz_gamma_Reco ) / 
				      	       ( sqrt( _px_gamma_Reco*_px_gamma_Reco + _py_gamma_Reco*_py_gamma_Reco + _pz_gamma_Reco*_pz_gamma_Reco ) * 
				      		 sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC + _pz_gamma_MC*_pz_gamma_MC ) ) );
			  
			  
					}
				} //Loop over both reco gammas
			     _pi0_tree->Fill();
			    }//If daughter of reco particle is a pi0 
			}//Loop over all reco daughters 
		    }
		}//Loop over all reco neutrinos

	      }
	    }//If daughter of MC particle is a pi0
	  }
	}
    }//Loop over all MC particles
    
    
    if (_debug){
      std::cout << "Ana results:" << std::endl
		<< "Mis-ID                 : " << _misID << std::endl           
		<< "Pi0 E  [Mc,Reco]  : [" << _e_nu_MC << ", " << _e_nu_Reco << "]" <<  std::endl
		<< "Daughter E  [Mc,Reco]    : [" << _e_gamma_MC << ", " << _e_gamma_Reco << "]" <<  std::endl
		<< "Gamma Vtx dist Mc-Reco:  " << sqrt( (_x_gamma_MC-_x_gamma_Reco)*(_x_gamma_MC-_x_gamma_Reco) +
							 (_y_gamma_MC-_y_gamma_Reco)*(_y_gamma_MC-_y_gamma_Reco) +
							 (_z_gamma_MC-_z_gamma_Reco)*(_z_gamma_MC-_z_gamma_Reco) ) << std::endl
		<< "Daughter theta [Mc,Reco] : [" << _theta_gamma_MC << ", " << _theta_gamma_Reco << "]" <<  std::endl
		<< "Daughter phi [Mc,Reco]   : [" << _phi_gamma_MC << ", " << _phi_gamma_Reco << "]" <<  std::endl << std::endl;
	}
    
    return true;
}
  
  void ERAnaPi0Selection::ProcessEnd(TFile* fout)
  {
    
    _eff = 100*_singlePi0_ctr/float(_numEvts);
    
    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "Pi0 Events    : " << _singlePi0_ctr << std::endl
	      << "Eff           : " << _eff << " %" << std::endl;


	      std::cout<<"NUMBER pi0 ecents: "<<_n_pi0<<std::endl ;

    if (fout){
      fout->cd();
      _pi0_tree->Write();
    }

    return;
  }

  void ERAnaPi0Selection::ResetTreeVariables(){
    
    _misID = 1;

    _n_psReco  = -1;
    _n_gammas      = -1;
    _n_gammasReco  = -1;
    _n_showers     = -1;
    _n_showersReco = -1;
    _n_pi0         = -1;

    _e_nu_MC          = -1000;
    _x_nu_MC         = -1000;
    _y_nu_MC         = -1000;
    _z_nu_MC         = -1000;
    _px_nu_MC         = -1000;
    _py_nu_MC         = -1000;
    _pz_nu_MC         = -1000;
    _pdg_nu_MC        = -1;
    _e_gamma_MC         = -1000;
    _x_gamma_MC         = -1000;
    _y_gamma_MC         = -1000;
    _z_gamma_MC         = -1000;
    _pdg_gamma_MC       = -1;
    _theta_gamma_MC     = -360;
    _phi_gamma_MC       = -360;
    _gamma_score_MC	= -1000;

    _e_gamma_Reco     = -1000;
    _e_nu_Reco      = -1000;
    _x_nu_Reco     = -1000;
    _y_nu_Reco     = -1000;
    _z_nu_Reco     = -1000;
    _px_nu_Reco     = -1000;
    _py_nu_Reco     = -1000;
    _pz_nu_Reco     = -1000;
    _x_gamma_Reco     = -1000;
    _y_gamma_Reco     = -1000;
    _z_gamma_Reco     = -1000;
    _theta_gamma_Reco = -360;
    _phi_gamma_Reco   = -360;
    _gamma_score_Reco	= -1000;

    _mom_dot       = -2;
    _vtx_dist   = -1000;


    return;
  }

  }

#endif
