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
    _pi0_reco_tree->Branch("_primary",&_primary,"primary/I");
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
  if(_verbose){
    std::cout << "Start of ANA : ParticleGraph Diagram: " << std::endl
              << graph.Diagram() << std::endl;
  }

   ResetTreeVariables();

	auto const& particles = graph.GetParticleArray();
	// First Find out how many pi0's we have 
	int pi0counter = 0;
	NodeID_t pi0node ;
//temp for primary
bool pi0primary = false;
	for(auto const &p: particles) if(p.PdgCode()==111){ pi0counter++; pi0node=p.ID(); pi0primary = p.Primary();}
      int pipr = pi0primary?0:1;
std::cout<<"pipr"<<pipr<<std::endl;// I have no idea why I need to cout this... otherwise I get an exception for invalid node

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
	_primary = pipr;
	
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

//=======================
//==Match things to MC===
	// Still need to be sorted out
//=======================
	// We need to see how many pi0's are in the event from truth level 
	auto mcgraph = MCParticleGraph();	
	auto mceventdata = MCEventData();	
	auto const& mcparticles = mcgraph.GetParticleArray();
	// First Find out how many pi0's we have 
	int mcpi0counter = 0;
	NodeID_t mcpi0node ;
	for(auto const &mcp: mcparticles) if(mcp.PdgCode()==111){ mcpi0counter++; mcpi0node=mcp.ID();}
	// Next find the matching shower. 
	auto mcgammanode = mcgraph.GetAllDescendantNodes(mcpi0node);
	//Dalitz Decay ?
	if(mcgammanode.size()!=2){
	if(_verbose) std::cout<<"This is a dalitz decay "<<std::endl;
		}
	if(mcgammanode.size()==2 && mcpi0counter==1){
		auto photon1_mc =  mcgraph.GetParticle(mcgammanode[0]);
		auto photon2_mc =  mcgraph.GetParticle(mcgammanode[1]);
		auto p1_RID_MC= photon1_mc.RecoID();
		auto p2_RID_MC= photon2_mc.RecoID();
		auto shr1_startpt_mc = mceventdata.Shower(p1_RID_MC);
		auto shr2_startpt_mc = mceventdata.Shower(p2_RID_MC);
			if(_verbose){
			std::cout<<"\n\tShower1 : "<<shr1_startpt_mc._energy<<std::endl;
			std::cout<<"\n\tShower1 : "<<shr1_startpt_mc.Start()<<std::endl;
			std::cout<<"\n\tShower2 : "<<shr2_startpt_mc._energy<<std::endl;
			std::cout<<"\n\tShower2 : "<<shr2_startpt_mc.Start()<<std::endl;
			}
		}// if 2shower
	}// if pi0counter==1
//================================
//================================
	

 return true; }

  void ERAna1PI0::ProcessEnd(TFile* fout)
  {
    if (fout){
      fout->cd();
      _pi0_reco_tree->Write();
      _1pi0_tree->Write();
	    }
	std::cout<<"From ER1pi0Ana  -->  Summary of Events \n" 	
	<< "\nTotal Reco 1pi0       "<<_pi0_reco_tree->GetEntries()/2
	<< "\nTotal Reco Events that have 1pi0 "<<_1pi0_tree->GetEntries()/2
	<<std::endl;
    return;
  }

 void ERAna1PI0::ResetTreeVariables(){

    _px_pi0_Reco = -1000;
    _py_pi0_Reco = -1000;
    _pz_pi0_Reco = -1000;
    _p_pi0_Reco = -1000;
    _mass_pi0_Reco = -1000;
    _primary = -1000 ;
    _x_gamma_MC = -1000;
    _y_gamma_MC = -1000;
    _z_gamma_MC = -1000;
    _px_gamma_MC = -1000;
    _py_gamma_MC = -1000;
    _pz_gamma_MC = -1000;
    _theta_gamma_MC = -1000;
    _phi_gamma_MC = -1000;

    _mass_pi0_Reco = -1000;


    _x_gamma_Reco = -1000;
    _y_gamma_Reco = -1000;
    _z_gamma_Reco = -1000;
    _px_gamma_Reco = -1000;
    _py_gamma_Reco = -1000;
    _pz_gamma_Reco = -1000;
    _theta_gamma_Reco = -1000;
    _phi_gamma_Reco = -1000;
    _x_pi0_Reco = -1000;
    _y_pi0_Reco = -1000;
    _z_pi0_Reco = -1000;
	
    _vtx_dist = 10000000;
	return;
}

}
#endif
