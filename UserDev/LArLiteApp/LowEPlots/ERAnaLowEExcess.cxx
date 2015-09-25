#ifndef ERTOOL_ERANALOWEEXCESS_CXX
#define ERTOOL_ERANALOWEEXCESS_CXX

#include "ERAnaLowEExcess.h"

namespace ertool {

  ERAnaLowEExcess::ERAnaLowEExcess(const std::string& name) 
  : AnaBase(name)
  , _result_tree(nullptr)
  {
    
    PrepareTreeVariables();

    //    _numEvts = 0;

    // set default energy cut (for counting) to 0
    _eCut = 0;


  }


  bool ERAnaLowEExcess::Analyze(const EventData &data, const ParticleGraph &graph)
  { 
    _result_tree->SetName(Form("%s",_treename.c_str()));
    
    // Reset tree variables
    // Assume we will mis-ID
    ResetTreeVariables();
    
    _numEvts += 1;

    // size of ParticleSet should be the number of neutrinos found, each associated with a single electron
    
    auto const& particles = graph.GetParticleArray();
    
    for ( auto const & p : particles ){
      if ( p.PdgCode() == 12 ){
	
	_neutrinos  += 1;
	
	// get all descendants of the neutrino
	_e_dep = 0;
	auto const descendants = graph.GetAllDescendantNodes(p.ID());
	for ( auto const & desc : descendants){
	  auto const & part = graph.GetParticle(desc);
	  // does this particle have a reco ID?
	  if (part.HasRecoObject() == true){
	    // get the reco object's dep. energy
	    // if shower
	    if (part.RecoType() == kShower)
	      _e_dep += data.Shower(part.RecoID())._energy;
	    if (part.RecoType() == kTrack)
	      _e_dep += data.Track(part.RecoID())._energy;
	  }// if the particle has a reco object
	}// for all neutrino descendants
      }// if we found the neutrino
    }
    
    
    for( auto const & p : graph.GetParticleArray() ){
            
      if(abs(p.PdgCode()) == 12 ){
	
	auto neutrino = p ;
	
	_e_nuReco = 0;
	//find the neutrino daughter that is a lepton
	for (auto const& d : neutrino.Children()){
	  
	  auto daught = graph.GetParticle(d) ;
	  _e_nuReco += daught.KineticEnergy();
	  
	}
      }
    }

    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    for( auto const & mc : mc_graph.GetParticleArray() ){
      
      if(abs(mc.PdgCode()) == 12 ||
	 abs(mc.PdgCode()) == 14 ){
	
	int ntype = 0;
	int ptype = 0;
	double E = mc.Energy()/1e3;

	//	std::cout << E << std::endl;

	if(mc.PdgCode() == 12)       ntype = 1;
	else if(mc.PdgCode() == -12) ntype = 2;
	else if(mc.PdgCode() ==  14) ntype = 3;
	else if(mc.PdgCode() == -14) ntype = 4;
	
	
		
	if(mc.ProcessType() == ::ertool::kK0L) ptype = 3;
	else if(mc.ProcessType() == ::ertool::kKCharged) ptype = 4;
	else if(mc.ProcessType() == ::ertool::kMuDecay) ptype = 1;
	else if(mc.ProcessType() == ::ertool::kPionDecay) ptype = 2;
	
	if(mc.ProcessType() != ::ertool::kK0L && 
	   mc.ProcessType() != ::ertool::kKCharged &&
	   mc.ProcessType() != ::ertool::kMuDecay &&
	   mc.ProcessType() != ::ertool::kPionDecay){
	  
	  std::cout << " PDG : " << mc.PdgCode() << " Process Type : " << mc.ProcessType() << " from " << 
	    ::ertool::kK0L <<  " or " <<
	    ::ertool::kKCharged << " or " <<
	    ::ertool::kMuDecay << " or " << 
	    ::ertool::kPionDecay << std::endl;
	}

	_weight = _fluxRW.get_weight(E, ntype, ptype);
	  
	break;
	  
	  }     
    }

    _result_tree->Fill();
    
    return true; }

  void ERAnaLowEExcess::ProcessEnd(TFile* fout)
  {

    if (fout){
      fout->cd();
      _result_tree->Write();
    }
    
    return;

  }

  void ERAnaLowEExcess::PrepareTreeVariables(){

    if (_result_tree) { delete _result_tree; }
    
    _result_tree = new TTree(Form("%s",_treename.c_str()),"Result Tree");
    _result_tree->Branch("_numEvts",&_numEvts,"numEvts/I");
    _result_tree->Branch("_neutrinos",&_neutrinos,"neutrinos/I");
    _result_tree->Branch("_e_nuReco",&_e_nuReco,"e_nuReco/D");
    _result_tree->Branch("_e_dep",&_e_dep,"e_dep/D");
    _result_tree->Branch("_weight",&_weight,"weight/D");
    
    return;
  }

  void ERAnaLowEExcess::ResetTreeVariables(){
    
    _numEvts = 0;
    _neutrinos = 0;
    _e_nuReco = 0;
    _e_dep = 0;

    return;

  }


}

#endif
