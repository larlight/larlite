#ifndef ERTOOL_ERANA1PI0_CXX
#define ERTOOL_ERANA1PI0_CXX

#include "ERAna1PI0.h"

namespace ertool {

  ERAna1PI0::ERAna1PI0() : AnaBase()
			, _pi0_reco_tree(nullptr)
			, _pi0_tree(nullptr)
			, _1pi0_tree(nullptr)
			, _mpi0_tree(nullptr)
  {
    _name     = "ERAna1PI0";

  if (_pi0_reco_tree) { delete _pi0_reco_tree; }
    _pi0_reco_tree = new TTree("_pi0_reco_tree","Pi0_reco Tree");
    _pi0_reco_tree->Branch("_x_gamma_Reco",&_x_gamma_Reco,"x_gamma_Reco/D");
    _pi0_reco_tree->Branch("_y_gamma_Reco",&_y_gamma_Reco,"y_gamma_Reco/D");
    _pi0_reco_tree->Branch("_z_gamma_Reco",&_z_gamma_Reco,"z_gamma_Reco/D");
    _pi0_reco_tree->Branch("_px_gamma_Reco",&_px_gamma_Reco,"px_gamma_Reco/D");
    _pi0_reco_tree->Branch("_py_gamma_Reco",&_py_gamma_Reco,"py_gamma_Reco/D");
    _pi0_reco_tree->Branch("_pz_gamma_Reco",&_pz_gamma_Reco,"pz_gamma_Reco/D");
    _pi0_reco_tree->Branch("_theta_gamma_Reco",&_theta_gamma_Reco,"theta_gamma_Reco/D");
    _pi0_reco_tree->Branch("_phi_gamma_Reco",&_phi_gamma_Reco,"phi_gamma_Reco/D");


  if (_pi0_tree) { delete _pi0_tree; }
    _pi0_tree = new TTree("_pi0_tree","Pi0 Tree");
    _pi0_tree->Branch("_x_gamma_Reco",&_x_gamma_Reco,"x_gamma_Reco/D");
    _pi0_tree->Branch("_y_gamma_Reco",&_y_gamma_Reco,"y_gamma_Reco/D");
    _pi0_tree->Branch("_z_gamma_Reco",&_z_gamma_Reco,"z_gamma_Reco/D");
    _pi0_tree->Branch("_px_gamma_Reco",&_px_gamma_Reco,"px_gamma_Reco/D");
    _pi0_tree->Branch("_py_gamma_Reco",&_py_gamma_Reco,"py_gamma_Reco/D");
    _pi0_tree->Branch("_pz_gamma_Reco",&_pz_gamma_Reco,"pz_gamma_Reco/D");
    _pi0_tree->Branch("_theta_gamma_Reco",&_theta_gamma_Reco,"theta_gamma_Reco/D");
    _pi0_tree->Branch("_phi_gamma_Reco",&_phi_gamma_Reco,"phi_gamma_Reco/D");


  if (_1pi0_tree) { delete _1pi0_tree; }
    _1pi0_tree = new TTree("_1pi0_tree","1Pi0 Tree");
    _1pi0_tree->Branch("_x_gamma_MC",&_x_gamma_MC,"x_gamma_MC/D");
    _1pi0_tree->Branch("_y_gamma_MC",&_y_gamma_MC,"y_gamma_MC/D");
    _1pi0_tree->Branch("_z_gamma_MC",&_z_gamma_MC,"z_gamma_MC/D");
    _1pi0_tree->Branch("_x_pi0_MC",&_x_pi0_MC,"x_pi0_MC/D");
    _1pi0_tree->Branch("_y_pi0_MC",&_y_pi0_MC,"y_pi0_MC/D");
    _1pi0_tree->Branch("_z_pi0_MC",&_z_pi0_MC,"z_pi0_MC/D");
    _1pi0_tree->Branch("_e_pi0_MC",&_e_pi0_MC,"e_pi0_MC/D");
    _1pi0_tree->Branch("_e_gamma_MC",&_e_gamma_MC,"e_gamma_MC/D");
    _1pi0_tree->Branch("_theta_gamma_MC",&_theta_gamma_MC,"theta_gamma_MC/D");
    _1pi0_tree->Branch("_phi_gamma_MC",&_phi_gamma_MC,"phi_gamma_MC/D");
    _1pi0_tree->Branch("_px_gamma_MC",&_px_gamma_MC,"px_gamma_MC/D");
    _1pi0_tree->Branch("_py_gamma_MC",&_py_gamma_MC,"py_gamma_MC/D");
    _1pi0_tree->Branch("_pz_gamma_MC",&_pz_gamma_MC,"pz_gamma_MC/D");
    _1pi0_tree->Branch("_x_gamma_Reco",&_x_gamma_Reco,"x_gamma_Reco/D");
    _1pi0_tree->Branch("_y_gamma_Reco",&_y_gamma_Reco,"y_gamma_Reco/D");
    _1pi0_tree->Branch("_z_gamma_Reco",&_z_gamma_Reco,"z_gamma_Reco/D");
    _1pi0_tree->Branch("_e_gamma_Reco",&_e_gamma_Reco,"e_gamma_Reco/D");
    _1pi0_tree->Branch("_px_gamma_Reco",&_px_gamma_Reco,"px_gamma_Reco/D");
    _1pi0_tree->Branch("_py_gamma_Reco",&_py_gamma_Reco,"py_gamma_Reco/D");
    _1pi0_tree->Branch("_pz_gamma_Reco",&_pz_gamma_Reco,"pz_gamma_Reco/D");
    _1pi0_tree->Branch("_e_pi0_Reco",&_e_pi0_Reco,"e_pi0_Reco/D");
    _1pi0_tree->Branch("_x_pi0_Reco",&_x_pi0_Reco,"x_pi0_Reco/D");
    _1pi0_tree->Branch("_y_pi0_Reco",&_y_pi0_Reco,"y_pi0_Reco/D");
    _1pi0_tree->Branch("_z_pi0_Reco",&_z_pi0_Reco,"z_pi0_Reco/D");
    _1pi0_tree->Branch("_theta_gamma_Reco",&_theta_gamma_Reco,"theta_gamma_Reco/D");
    _1pi0_tree->Branch("_phi_gamma_Reco",&_phi_gamma_Reco,"phi_gamma_Reco/D");

  if (_mpi0_tree) { delete _mpi0_tree; }
    _mpi0_tree = new TTree("_mpi0_tree","mPi0 Tree");
    _mpi0_tree->Branch("_x_gamma_MC",&_x_gamma_MC,"x_gamma_MC/D");
    _mpi0_tree->Branch("_y_gamma_MC",&_y_gamma_MC,"y_gamma_MC/D");
    _mpi0_tree->Branch("_z_gamma_MC",&_z_gamma_MC,"z_gamma_MC/D");
    _mpi0_tree->Branch("_px_gamma_MC",&_px_gamma_MC,"px_gamma_MC/D");
    _mpi0_tree->Branch("_py_gamma_MC",&_py_gamma_MC,"py_gamma_MC/D");
    _mpi0_tree->Branch("_pz_gamma_MC",&_pz_gamma_MC,"pz_gamma_MC/D");
    _mpi0_tree->Branch("_theta_gamma_MC",&_theta_gamma_MC,"theta_gamma_MC/D");
    _mpi0_tree->Branch("_phi_gamma_MC",&_phi_gamma_MC,"phi_gamma_MC/D");
    _mpi0_tree->Branch("_x_gamma_Reco",&_x_gamma_Reco,"x_gamma_Reco/D");
    _mpi0_tree->Branch("_y_gamma_Reco",&_y_gamma_Reco,"y_gamma_Reco/D");
    _mpi0_tree->Branch("_z_gamma_Reco",&_z_gamma_Reco,"z_gamma_Reco/D");
    _mpi0_tree->Branch("_px_gamma_Reco",&_px_gamma_Reco,"px_gamma_Reco/D");
    _mpi0_tree->Branch("_py_gamma_Reco",&_py_gamma_Reco,"py_gamma_Reco/D");
    _mpi0_tree->Branch("_pz_gamma_Reco",&_pz_gamma_Reco,"pz_gamma_Reco/D");
    _mpi0_tree->Branch("_theta_gamma_Reco",&_theta_gamma_Reco,"theta_gamma_Reco/D");
    _mpi0_tree->Branch("_phi_gamma_Reco",&_phi_gamma_Reco,"phi_gamma_Reco/D");

    ct = 0 ;
  }

  void ERAna1PI0::Reset()
  {}

  bool ERAna1PI0::Analyze(const EventData &data, const ParticleSet &ps)
  {
    // Get MC particle set, EventData(showers/tracks...)
    auto mc_ps = MCParticleSet();
    auto mc_data = MCEventData();
    /// Function to re-set TTree variables
   ResetTreeVariables();

	// In almost all of my code I will always be looking for pi0-->gamma,gamma
	bool DalitzDecay = false;
	// Look to see if there was a recod pi0 
	int _n_pi0reco = 0; 
        for (auto &preco : ps){ if(preco.PdgCode()==111) _n_pi0reco++;}
	
	if(_n_pi0reco==1){
	ct++;
	    for (auto &preco : ps){
		if(preco.PdgCode()==111){ 
		//fill out the info about that was reconstructed 
// still needs to be done 
		// Find the reco daughters and then look for the gammas
		// at the moment I am assuming that a reco pi0 is only filling a gamma/gamma 
		_x_pi0_Reco = preco.Vertex()[0];
		_y_pi0_Reco = preco.Vertex()[1];
		_z_pi0_Reco = preco.Vertex()[2];
		_e_pi0_Reco = preco.Energy();

		for(auto &gammareco: preco.Daughters()){
		/// fill out the info about the reco gamma  particle 
	        _x_gamma_Reco = gammareco.Vertex()[0];
		_y_gamma_Reco = gammareco.Vertex()[1];
		_z_gamma_Reco = gammareco.Vertex()[2]; 
	        _px_gamma_Reco = gammareco.Momentum()[0];
		_py_gamma_Reco = gammareco.Momentum()[1];
		_pz_gamma_Reco = gammareco.Momentum()[2]; 
		_e_gamma_Reco = gammareco.Energy(); 
		_theta_gamma_Reco = (180./3.14) * acos( _pz_gamma_Reco / sqrt( _px_gamma_Reco*_px_gamma_Reco + _py_gamma_Reco*_py_gamma_Reco + _pz_gamma_Reco*_pz_gamma_Reco ) );
		_phi_gamma_Reco   = (180./3.14) * asin( _py_gamma_Reco / sqrt( _px_gamma_Reco*_px_gamma_Reco + _py_gamma_Reco*_py_gamma_Reco ) );

		// FIll the reco tree 
		_pi0_reco_tree->Fill();
			// look to find the matching gamma in truth
			double temp_dist = 0;
			// loop over mcparticle set
		        for(auto &p : mc_ps){
			// see if there is one in the MC_truth
		        if (abs(p.PdgCode()) != 12 && abs(p.PdgCode()) != 14){
			// first see how many single pi0 
				// a. 0 pi0's 
				// b. 1 pi0's 
				// c. >1 pi0's 
				int singlepi0mc = 0;
				for(auto &pdaughter: p.Daughters()){if(pdaughter.PdgCode() == 111) singlepi0mc++;}

				//=================================
				//=======Case a. 0 pi0  ===========
				//=================================
				if(singlepi0mc==0){
				// Nothing to compare to. 
				// at the moment
				_pi0_tree->Fill();
				}
				//====================================
				//=======ENDCase a. 0 pi0  ===========
				//====================================


				//=================================
				//=======Case b. 1 pi0  ===========
				//=================================
				if(singlepi0mc==1){
					for(auto &pdaughter: p.Daughters()){
					 if(pdaughter.PdgCode() == 111 ){
						// was it a Dalitz Decay? 
						if(pdaughter.Daughters().size()!=2){DalitzDecay = true; }
						
						_x_pi0_MC = pdaughter.Vertex()[0];
						_y_pi0_MC = pdaughter.Vertex()[1];
						_z_pi0_MC = pdaughter.Vertex()[2];
						_e_pi0_MC = pdaughter.Energy();

					// 
					for(auto&gammamc : pdaughter.Daughters()){ 
					// now match the reco gamma to mc gamma
					    temp_dist = sqrt( pow(gammamc.Vertex()[0] - _x_gamma_Reco,2)
                                            + pow(gammamc.Vertex()[1] - _y_gamma_Reco,2)
                                            + pow(gammamc.Vertex()[2] - _z_gamma_Reco,2) );

						if(_vtx_dist>temp_dist){ 
						_vtx_dist = temp_dist;
						_x_gamma_MC = gammamc.Vertex()[0];
						_y_gamma_MC = gammamc.Vertex()[1];
						_z_gamma_MC = gammamc.Vertex()[2];
						_px_gamma_MC = gammamc.Momentum()[0];
						_py_gamma_MC = gammamc.Momentum()[1];
						_pz_gamma_MC = gammamc.Momentum()[2]; 
						_e_gamma_MC = gammamc.Energy(); 
						_theta_gamma_MC = (180./3.14) * acos( _pz_gamma_MC / sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC + _pz_gamma_MC*_pz_gamma_MC ) );
						_phi_gamma_MC   = (180./3.14) * asin( _py_gamma_MC / sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC ) );
						_mass_pi0_MC = pow(2*(e_g ));
							}//(_vtx_dist>temp_dist)
						}// loop over mc gamma
					    }// if the particle is a pi0
					}// for loop over daughters
				   if(!DalitzDecay)_1pi0_tree->Fill();
				}// if(singlepi0mc==1)
				//======================================
				//======= END Case b. 1 pi0  ===========
				//======================================

				//=================================
				//=======Case c. 1 pi0  ===========
				//=================================
				if(singlepi0mc>=2){
					for(auto &pdaughter: p.Daughters()){
						 if(pdaughter.PdgCode() == 111){
						// was it a Dalitz Decay? 
						// will need to figure out how to compare things for multi-pi events 
						// will deal with this later
					for(auto&gammamc : pdaughter.Daughters()){ 
					// now match the reco gamma to mc gamma
					    temp_dist = sqrt( pow(gammamc.Vertex()[0] - _x_gamma_Reco,2)
                                            + pow(gammamc.Vertex()[1] - _y_gamma_Reco,2)
                                            + pow(gammamc.Vertex()[2] - _z_gamma_Reco,2) );

						if(_vtx_dist>temp_dist){ 
						_vtx_dist = temp_dist;
						_x_gamma_MC = gammamc.Vertex()[0];
						_y_gamma_MC = gammamc.Vertex()[1];
						_z_gamma_MC = gammamc.Vertex()[2];
						_px_gamma_MC = gammamc.Momentum()[0];
						_py_gamma_MC = gammamc.Momentum()[1];
						_pz_gamma_MC = gammamc.Momentum()[2]; 
						_theta_gamma_MC = (180./3.14) * acos( _pz_gamma_MC / sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC + _pz_gamma_MC*_pz_gamma_MC ) );
						_phi_gamma_MC   = (180./3.14) * asin( _py_gamma_MC / sqrt( _px_gamma_MC*_px_gamma_MC + _py_gamma_MC*_py_gamma_MC ) );
							}//(_vtx_dist>temp_dist)
						}// loop over mc gamma
					    }// if the particle is a pi0
					}// for loop over daughters
				_mpi0_tree->Fill();
				}// if(singlepi0mc==1)
				//=====================================
				//=======END Case c. 1 pi0  ===========
				//=====================================

			   }// if(abs(p.PdgCode()) != 12 && abs(p.PdgCode() != 14)){
			 }// (auto &p : mc_ps)
			}//for(auto &pi0dreco: preco.Daughters())
	       	       }// for (auto &preco : ps)	
		}// if(preco.PdgCode()==111)
	}//if(_n_pi0reco==1) Single pi0

 return true; }


  void ERAna1PI0::ProcessEnd(TFile* fout)
  {
    if (fout){
      fout->cd();
      _pi0_reco_tree->Write();
     // _pi0_tree->Write();
      _1pi0_tree->Write();
     // _mpi0_tree->Write();
    }
    
        double found = _pi0_reco_tree->GetEntries()/2 ;

	std::cout<<"From ER1pi0Ana  -->  Summary of Events \n" 	
	<< "\nTotal Reco 1pi0       "<<_pi0_reco_tree->GetEntries()/2
//	<< "\nTotal MC Events that have 0pi0 "<<_pi0_tree->GetEntries()/2
	<< "\nTotal MC Events that have 1pi0 "<<_1pi0_tree->GetEntries()/2
//	<< "\nTotal MC Events that have >1pi0 "<<_mpi0_tree->GetEntries()/2
	<< "\nTotal MC Events with !=1 pi0: "<<ct
	<< "\nEfficiency is: "<< found / ct 
	

	<<std::endl;


    return;
  }

 void ERAna1PI0::ResetTreeVariables(){

    _x_gamma_MC = -1000;
    _y_gamma_MC = -1000;
    _z_gamma_MC = -1000;
    _e_gamma_MC = -1000;
    _px_gamma_MC = -1000;
    _py_gamma_MC = -1000;
    _pz_gamma_MC = -1000;
    _theta_gamma_MC = -1000;
    _phi_gamma_MC = -1000;
    _x_pi0_MC = -1000;
    _y_pi0_MC = -1000;
    _z_pi0_MC = -1000;
    _e_pi0_MC = -1000;

    _mass_pi0_MC = -1000;
    _mass_pi0_Reco = -1000;


    _x_gamma_Reco = -1000;
    _y_gamma_Reco = -1000;
    _z_gamma_Reco = -1000;
    _px_gamma_Reco = -1000;
    _py_gamma_Reco = -1000;
    _pz_gamma_Reco = -1000;
    _e_gamma_Reco = -1000;
    _theta_gamma_Reco = -1000;
    _phi_gamma_Reco = -1000;
    _x_pi0_Reco = -1000;
    _y_pi0_Reco = -1000;
    _z_pi0_Reco = -1000;
    _e_pi0_Reco = -1000;
	
    _vtx_dist = 10000000;
	return;
}

}
#endif
