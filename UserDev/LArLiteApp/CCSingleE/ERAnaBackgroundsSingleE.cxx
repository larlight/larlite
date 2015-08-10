#ifndef ERTOOL_ERANABACKGROUNDSSINGLEE_CXX
#define ERTOOL_ERANABACKGROUNDSSINGLEE_CXX

#include "ERAnaBackgroundsSingleE.h"

namespace ertool {
  
  ERAnaBackgroundsSingleE::ERAnaBackgroundsSingleE(const std::string& name)
    : AnaBase(name)
    , _result_tree(nullptr)
    , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)
      
  {
    
    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");
    
    _result_tree->Branch("_numEvts",&_numEvts,"numEvts/I");
    _result_tree->Branch("_distToWall",&_distToWall,"distToWall/D");
    _result_tree->Branch("_e_nuReco",&_e_nuReco,"e_nuReco/D");
    _result_tree->Branch("_e_lepReco",&_e_lepReco,"e_lepReco/D");
    
    // keep track of number of events gone by
    _numEvts = 0;
    // keep track of singleEs found
    _singleE_ctr = 0;
    
    _debug = false;
    
    // set default energy cut (for counting) to 0
    _eCut = 0;
    
  }
  
  void ERAnaBackgroundsSingleE::Reset()
  {}
  
  bool ERAnaBackgroundsSingleE::Analyze(const EventData &data, const ParticleGraph &graph)
  {
    
    if (_debug)
      std::cout << "******  Begin ERAnaBackgroundsSingleE Analysis  ******" << std::endl;
    
    _numEvts += 1;
    
    if (_debug)
      std::cout << "ParticleGraph Diagram: " << std::endl
		<< graph.Diagram() << std::endl;
    
    // size of ParticleSet should be the number of neutrinos found, 
    //each associated with a single electron
    // If no single electrons reconstructed, don't care about anything
    if ( graph.GetNumPrimaries() == 0 )
      return false;
    
    // Reset tree variables
    ResetTreeVariables();
    
    /*
      auto const& particles = graph.GetParticleArray();
      int neutrinos = 0;
      for ( auto const & p : particles ){
      if ( p.PdgCode() == 12 )
      neutrinos += 1;
      }
    */
    
    
    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    
    // Get MC EventData (showers/tracks...)
    auto const& mc_data = MCEventData();
    
    // If debug -> print out MC particle set
    if (_debug){
      std::cout << "MC Particle Tree: " << std::endl;
      std::cout << mc_graph.Diagram();
    }
    
    /*
    // Loop through MC Particle Graph...
    // loop over all particles in MC particle set (made by Helper)
    for (auto &p : mc_graph.GetParticleArray()){
    
    // Find the neutrino store its energy
    if (abs(p.PdgCode()) == 12 || abs(p.PdgCode()) == 14){
    _e_nu = p.Energy();
    
    bool found_lepton_daughter = false;
    // Loop over the neutrino's children
    for (auto &nud : p.Children()){
    auto d = mc_graph.GetParticle(nud);
    // Look for lepton daughters of the neutrino
    if (abs(d.PdgCode()) == 11 || abs(d.PdgCode()) == 13){
    if(found_lepton_daughter) 
    std::cout<<"wtf already found daughter??"<<std::endl;
    found_lepton_daughter = true;
    _e_lep = d.Energy();
    }
    }
    } //end if neutrino was found in MC particle graph
    } // end loop over MC particle graph
    */
    
    //Loop over reconstructed particle graph
    for( auto const & p : graph.GetParticleArray() ){
      
      //Only care about reconstructed nues
      if(abs(p.PdgCode()) != 12) continue;
      
      auto neutrino = p;
      
      _e_nuReco = ComputeNuEnergy(neutrino,graph);
      
      //Distance between the neutrino vertex and the wall
      //(used for fiducial volume cuts)
      _distToWall = sqrt(_geoAlgo.SqDist(neutrino.Vertex(),fTPC));
      
      //Loop over neutrino daughters to grab the lepton energy
      for (auto const& d : neutrino.Children()){
	auto daught = graph.GetParticle(d);
	if(abs(daught.PdgCode()) == 11 ){
	  _e_lepReco = daught.Energy();
	}	
      }// for all children 
      
      //fill _result_tree once per reconstructed Nue
      _singleE_ctr++;
      _result_tree->Fill();
    }// loop over all CCSingleEs found in event
    
    
    if (_debug)
      std::cout << "Ana results:" << std::endl
		<< "Neutrino E  [Reco]  : [" << _e_nuReco << "]" <<  std::endl
		<< "Lepton E  [Reco]    : [" << _e_lepReco << "]" <<  std::endl;
	
    return true;
  }
  
  void ERAnaBackgroundsSingleE::ProcessEnd(TFile* fout)
  {
    
    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "SingleE Events: " << _singleE_ctr << std::endl;
    
    if (fout){
      fout->cd();
      _result_tree->Write();
    }
    
    return;
  }
  
  double ERAnaBackgroundsSingleE::ComputeNuEnergy(const Particle &neutrino, const ParticleGraph &graph){
   
    double energy = 0;
    
    for (auto const& d : neutrino.Children()){
      auto daught = graph.GetParticle(d);
      energy += daught.KineticEnergy();
    }
    
    return energy;
  }

  void ERAnaBackgroundsSingleE::ResetTreeVariables(){
    
    _e_nuReco      = -1000;
    _e_lepReco     = -1000;
    _distToWall    = -9999;
    
    return;
  }
  
} //end namespace ertool

#endif
