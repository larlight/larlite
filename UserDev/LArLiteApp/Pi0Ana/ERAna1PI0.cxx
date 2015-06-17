#ifndef ERTOOL_ERANA1PI0_CXX
#define ERTOOL_ERANA1PI0_CXX

#include "ERAna1PI0.h"

namespace ertool {

  ERAna1PI0::ERAna1PI0(const std::string& name) 
    : AnaBase(name)
    , _pi0_reco_tree(nullptr)
    , _1pi0_tree(nullptr)
  {
//====================
// Reco objects tree
//====================
  if (_pi0_reco_tree) { delete _pi0_reco_tree; }
    _pi0_reco_tree = new TTree("_pi0_reco_tree","Pi0_reco Tree");
// Reco about the pi0
    _pi0_reco_tree->Branch("_x_pi0_Reco",&_x_pi0_Reco,"x_pi0_Reco/D");
    _pi0_reco_tree->Branch("_y_pi0_Reco",&_y_pi0_Reco,"y_pi0_Reco/D");
    _pi0_reco_tree->Branch("_z_pi0_Reco",&_z_pi0_Reco,"z_pi0_Reco/D");
    _pi0_reco_tree->Branch("_px_pi0_Reco",&_px_pi0_Reco,"px_pi0_Reco/D");
    _pi0_reco_tree->Branch("_py_pi0_Reco",&_py_pi0_Reco,"py_pi0_Reco/D");
    _pi0_reco_tree->Branch("_pz_pi0_Reco",&_pz_pi0_Reco,"pz_pi0_Reco/D");
    _pi0_reco_tree->Branch("_p_pi0_Reco",&_p_pi0_Reco,"p_pi0_Reco/D");
    _pi0_reco_tree->Branch("_mass_pi0_Reco",&_mass_pi0_Reco,"mass_pi0_Reco/D");
//First shower
    _pi0_reco_tree->Branch("_x_shra_Reco",&_x_shra_Reco,"x_shra_Reco/D");
    _pi0_reco_tree->Branch("_y_shra_Reco",&_y_shra_Reco,"y_shra_Reco/D");
    _pi0_reco_tree->Branch("_z_shra_Reco",&_z_shra_Reco,"z_shra_Reco/D");
    _pi0_reco_tree->Branch("_px_shra_Reco",&_px_shra_Reco,"px_shra_Reco/D");
    _pi0_reco_tree->Branch("_py_shra_Reco",&_py_shra_Reco,"py_shra_Reco/D");
    _pi0_reco_tree->Branch("_pz_shra_Reco",&_pz_shra_Reco,"pz_shra_Reco/D");
    _pi0_reco_tree->Branch("_p_shra_Reco",&_p_shra_Reco,"p_shra_Reco/D");
    _pi0_reco_tree->Branch("_e_shra_Reco",&_e_shra_Reco,"e_shra_Reco/D");
//Second shower
    _pi0_reco_tree->Branch("_x_shrb_Reco",&_x_shrb_Reco,"x_shrb_Reco/D");
    _pi0_reco_tree->Branch("_y_shrb_Reco",&_y_shrb_Reco,"y_shrb_Reco/D");
    _pi0_reco_tree->Branch("_z_shrb_Reco",&_z_shrb_Reco,"z_shrb_Reco/D");
    _pi0_reco_tree->Branch("_px_shrb_Reco",&_px_shrb_Reco,"px_shrb_Reco/D");
    _pi0_reco_tree->Branch("_py_shrb_Reco",&_py_shrb_Reco,"py_shrb_Reco/D");
    _pi0_reco_tree->Branch("_pz_shrb_Reco",&_pz_shrb_Reco,"pz_shrb_Reco/D");
    _pi0_reco_tree->Branch("_p_shrb_Reco",&_p_shrb_Reco,"p_shrb_Reco/D");
    _pi0_reco_tree->Branch("_e_shrb_Reco",&_e_shrb_Reco,"e_shrb_Reco/D");
// both showers 
    _pi0_reco_tree->Branch("_shr_a_dot_b_Reco",&_shr_a_dot_b_Reco,"shr_a_dot_b_Reco/D");

///=========================================
// Reco & MC objects tree with 1pi0 events
//.... This is to try to compare
// reco object to mc object in cases of signal events
//=========================================
  if (_1pi0_tree){ delete _1pi0_tree; }
    _1pi0_tree = new TTree("_1pi0_tree","1Pi0 Tree");

		// RECO INFO
    _1pi0_tree->Branch("_x_pi0_Reco",&_x_pi0_Reco,"x_pi0_Reco/D");
    _1pi0_tree->Branch("_y_pi0_Reco",&_y_pi0_Reco,"y_pi0_Reco/D");
    _1pi0_tree->Branch("_z_pi0_Reco",&_z_pi0_Reco,"z_pi0_Reco/D");
    _1pi0_tree->Branch("_px_pi0_Reco",&_px_pi0_Reco,"px_pi0_Reco/D");
    _1pi0_tree->Branch("_py_pi0_Reco",&_py_pi0_Reco,"py_pi0_Reco/D");
    _1pi0_tree->Branch("_pz_pi0_Reco",&_pz_pi0_Reco,"pz_pi0_Reco/D");
    _1pi0_tree->Branch("_p_pi0_Reco",&_p_pi0_Reco,"p_pi0_Reco/D");
    _1pi0_tree->Branch("_mass_pi0_Reco",&_mass_pi0_Reco,"mass_pi0_Reco/D");
//First shower
    _1pi0_tree->Branch("_x_shra_Reco",&_x_shra_Reco,"x_shra_Reco/D");
    _1pi0_tree->Branch("_y_shra_Reco",&_y_shra_Reco,"y_shra_Reco/D");
    _1pi0_tree->Branch("_z_shra_Reco",&_z_shra_Reco,"z_shra_Reco/D");
    _1pi0_tree->Branch("_px_shra_Reco",&_px_shra_Reco,"px_shra_Reco/D");
    _1pi0_tree->Branch("_py_shra_Reco",&_py_shra_Reco,"py_shra_Reco/D");
    _1pi0_tree->Branch("_pz_shra_Reco",&_pz_shra_Reco,"pz_shra_Reco/D");
    _1pi0_tree->Branch("_p_shra_Reco",&_p_shra_Reco,"p_shra_Reco/D");
    _1pi0_tree->Branch("_e_shra_Reco",&_e_shra_Reco,"e_shra_Reco/D");
//Second shower
    _1pi0_tree->Branch("_x_shrb_Reco",&_x_shrb_Reco,"x_shrb_Reco/D");
    _1pi0_tree->Branch("_y_shrb_Reco",&_y_shrb_Reco,"y_shrb_Reco/D");
    _1pi0_tree->Branch("_z_shrb_Reco",&_z_shrb_Reco,"z_shrb_Reco/D");
    _1pi0_tree->Branch("_px_shrb_Reco",&_px_shrb_Reco,"px_shrb_Reco/D");
    _1pi0_tree->Branch("_py_shrb_Reco",&_py_shrb_Reco,"py_shrb_Reco/D");
    _1pi0_tree->Branch("_pz_shrb_Reco",&_pz_shrb_Reco,"pz_shrb_Reco/D");
    _1pi0_tree->Branch("_p_shrb_Reco",&_p_shrb_Reco,"p_shrb_Reco/D");
    _1pi0_tree->Branch("_e_shrb_Reco",&_e_shrb_Reco,"e_shrb_Reco/D");
// both showers 
    _1pi0_tree->Branch("_shr_a_dot_b_Reco",&_shr_a_dot_b_Reco,"shr_a_dot_b_Reco/D");
		// MC INFO
  }// 


  void ERAna1PI0::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAna1PI0::ProcessBegin()
  {
	std::cout<<" Look we are defining things "<<std::endl;
  }

  void ERAna1PI0::Reset()
  {}

  bool ERAna1PI0::Analyze(const EventData &data,  const ParticleGraph &graph)
  {
  if(_debug){
    std::cout << "Start of ANA : ParticleGraph Diagram: " << std::endl
              << graph.Diagram() << std::endl;
  }

   ResetTreeVariables();

	auto const& particles = graph.GetParticleArray();
	// First Find out how many pi0's we have 
	int pi0counter = 0;
	NodeID_t pi0node ;
	for(auto const &p: particles) if(p.PdgCode()==111){ pi0counter++; pi0node=p.ID();}

	// if single pi0 let's inspect it	
	if(pi0counter==1){ 
	// Fill out info for reconstructed pi0
	auto pi0particle_r = graph.GetParticle(pi0node);
	_x_pi0_Reco = pi0particle_r.Vertex()[0];
	_y_pi0_Reco = pi0particle_r.Vertex()[1];
	_z_pi0_Reco = pi0particle_r.Vertex()[2];
	_px_pi0_Reco = pi0particle_r.Momentum()[0];
	_py_pi0_Reco = pi0particle_r.Momentum()[1];
	_pz_pi0_Reco = pi0particle_r.Momentum()[2];
	//_p_pi0_Reco = pi0particle_r.Momentum()[3];// This returns a value that doesnt make sense. RG 
	_p_pi0_Reco = sqrt(pow(_px_pi0_Reco,2) + pow(_py_pi0_Reco,2) + pow(_pz_pi0_Reco,2));// not working. little hack
	_mass_pi0_Reco =pi0particle_r.Mass();
	
		// What are the descendents of this pi0 
		// We know that there are two photons in the reconstruction
		// We will define showerA and showerB
		auto photonid = graph.GetAllDescendantNodes(pi0node);
		auto shra_r =  graph.GetParticle(photonid[0]);
		auto shrb_r =  graph.GetParticle(photonid[1]);

		// Fill out the first shower 
		_x_shra_Reco = shra_r.Vertex()[0];
		_y_shra_Reco = shra_r.Vertex()[1];
		_z_shra_Reco = shra_r.Vertex()[2];
		_px_shra_Reco = shra_r.Momentum()[0];
		_py_shra_Reco = shra_r.Momentum()[1];
		_pz_shra_Reco = shra_r.Momentum()[2];
		_p_shra_Reco = shra_r.Momentum()[3];// not working.... but should be just energy for photons
		_e_shra_Reco = shra_r.Energy();
		
		//Fill out the second shower
		_x_shrb_Reco = shrb_r.Vertex()[0];
		_y_shrb_Reco = shrb_r.Vertex()[1];
		_z_shrb_Reco = shrb_r.Vertex()[2];
		_px_shrb_Reco = shrb_r.Momentum()[0];
		_py_shrb_Reco = shrb_r.Momentum()[1];
		_pz_shrb_Reco = shrb_r.Momentum()[2];
		_p_shrb_Reco = shrb_r.Momentum()[3];// not working.... but should be just energy for photons
		_e_shrb_Reco = shrb_r.Energy();

		// Fill out some comparison of the two photons. 
		// showerA dot showerB 
		_shr_a_dot_b_Reco = cos(shra_r.Momentum().Angle(shrb_r.Momentum()));// needs to be normalized
	_pi0_reco_tree->Fill();

	// We need to see how many pi0's are in the event from truth level 
	auto mcgraph = MCParticleGraph();	
	auto const& mcparticles = mcgraph.GetParticleArray();
	// First Find out how many pi0's we have 
	int mcpi0counter = 0;
	NodeID_t mcpi0node ;
	for(auto const &mcp: mcparticles) if(mcp.PdgCode()==111){ mcpi0counter++; mcpi0node=mcp.ID();}
	// Next find the matching shower. 
	auto mcgammanode = mcgraph.GetAllDescendantNodes(mcpi0node);
	//Dalitz Decay ?
	if(mcgammanode.size()!=2){std::cout<<"This is a dalitz decay "<<std::endl;}
	
	if(mcgammanode.size()==2 && mcpi0counter==1){
		auto photon1_mc =  mcgraph.GetParticle(mcgammanode[0]);
		auto photon2_mc =  mcgraph.GetParticle(mcgammanode[1]);
		auto p1_RID_MC= photon1_mc.RecoID();
		std::cout<<" THe reco ID "<<p1_RID_MC<<std::endl;
		//auto shr1_startpt_mc = mcgraph.GetParticle(shr1_nodes_mc[0]);
		
	/*	


		auto shr1_mc =  mcgraph.GetParticle(mcgammanode[0]);
		auto shr2_mc =  mcgraph.GetParticle(mcgammanode[1]);
		std::cout<<" MC NODE : "<< mcgammanode[0]<<"   ,     "<<mcgammanode[1]<<std::endl;
		std::cout<<" These are the two shower particle info : " 
			<<"\n\tShower1 : "<<shr1_mc.Energy()<<std::endl<<"\tPosition"<<shr1_mc.Vertex()<<std::endl
			<<"\n\n\tShower2 : "<<shr2_mc.Energy()<<std::endl<<"\tPosition"<<shr2_mc.Vertex()<<std::endl;
		// fill and mathch the photons
//================================
//================================
	// Match the showers
//================================
//================================
	int match_a;
	int match_b;
// Adding a shower match pair.
std::pair<int, int> shower_match_A;
std::pair<int, int> shower_match_B;
	
		// This can be a lot cleaner with a few loops but just temp for now 
		double vmatz= shra_r.Vertex().Dist(shr1_mc.Vertex());
		double vmatx= shra_r.Vertex().Dist(shr2_mc.Vertex());
	std::cout<<" vetx match dist "<< vmatz << " other vert match dist "<< vmatx<<std::endl;
*/
		}// if 2shower
//================================
//================================
//================================
//================================
/*
*/
	}// if pi0counter==1


	// First we need to match which shower belongs to which 
		
	//	    temp_dist = sqrt( pow(gammamc.Vertex()[0] - _x_gamma_Reco,2)
          //                                  + pow(gammamc.Vertex()[1] - _y_gamma_Reco,2)
            //                                + pow(gammamc.Vertex()[2] - _z_gamma_Reco,2) );
	//					if(_vtx_dist>temp_dist){ 
	//					_vtx_dist = temp_dist;

	// first check to see which showers match the photons from the pi0
	auto recoshowers = data.Shower();
	// here we can store the ID and then use to compare down the road. 
	
// Next Match some things.
   // If there is a pi0 match the truth info 
   // If pi0 goes to gamma/gamma match the gamma's
	// We still do not have a way to match the gamma directly 




/*
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
*/
 return true; }


  void ERAna1PI0::ProcessEnd(TFile* fout)
  {

    if (fout){
	std::cout<<" Are we finishing the fout part  HUHUHUHUHUHUH"<<std::endl;
      fout->cd();
      _pi0_reco_tree->Write();
      _1pi0_tree->Write();
    }
    
        double found = _pi0_reco_tree->GetEntries()/2 ;

	std::cout<<"From ER1pi0Ana  -->  Summary of Events \n" 	
	<< "\nTotal Reco 1pi0       "<<_pi0_reco_tree->GetEntries()/2
//	<< "\nTotal Reco Events that have 0pi0 "<<_pi0_tree->GetEntries()/2
	<< "\nTotal Reco Events that have 1pi0 "<<_1pi0_tree->GetEntries()/2
//	<< "\nTotal Reco Events that have >1pi0 "<<_mpi0_tree->GetEntries()/2
	

	<<std::endl;


    return;
  }

 void ERAna1PI0::ResetTreeVariables(){

    _x_pi0_Reco = -1000;
    _y_pi0_Reco = -1000;
    _z_pi0_Reco = -1000;
    _px_pi0_Reco = -1000;
    _py_pi0_Reco = -1000;
    _pz_pi0_Reco = -1000;
    _p_pi0_Reco = -1000;
    _mass_pi0_Reco = -1000;
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
