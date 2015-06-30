#ifndef ERTOOL_ERANAMICHELE_CXX
#define ERTOOL_ERANAMICHELE_CXX

#include "ERAnaMichelE.h"

namespace ertool {

  ERAnaMichelE::ERAnaMichelE(const std::string& name)
    : AnaBase(name)
    , _result_tree(nullptr)

  {

    // prepare a TTree with variables we want to save
    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");
    _result_tree->Branch("_dedx",&_dedx,"dedx/D");
    _result_tree->Branch("_pdg",&_pdg,"pdg/I");
    _result_tree->Branch("_energy",&_energy,"energy/D");
    // keep track of number of events gone by
    _numEvts = 0;

    // set verbosity
    _debug = false;

  }

  void ERAnaMichelE::Reset()
  {}

  bool ERAnaMichelE::Analyze(const EventData &data, const ParticleGraph &graph)
  {

    if (_debug)
      std::cout << "******  Begin ERAnaMichelE Analysis  ******" << std::endl;

    auto datacpy = data;

    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_evtdata = MCEventData();
    auto const& mcparticles = mc_graph.GetParticleArray();

    // instantiate objects to use
    ::ertool::Particle recoPart;
    
    // loop through particles reconstructed
    auto const& particles = graph.GetParticleArray();
    for (std::deque<::ertool::Particle>::const_iterator it = particles.begin(); it != particles.end(); it++){
      recoPart = *it;
      // looking for reconstructed michel electrons -> ask for PDG == 11
      if (recoPart.PdgCode() == 11){

	// if parent is not a muon continue
	if ( abs(graph.GetParticle(recoPart.Parent()).PdgCode()) != 13 )
	  continue;
	// get shower associated with the reconstructed michel particle
	auto const& shower = datacpy.Shower(recoPart.RecoID());
	
	_dedx   = shower._dedx;
	_pdg    = recoPart.PdgCode();
	_energy = recoPart.Energy();
	
	_result_tree->Fill();

      }// if reco particle is electron
    }// for all reco particles
    
    if (_debug){
      std::cout << "MC Particle Diagram: " << std::endl
		<< mc_graph.Diagram() << std::endl;
    }
    
    // Reset tree variables
    // Assume we will mis-ID
    ResetTreeVariables();

    _numEvts += 1;

    return true;
  }
  
  void ERAnaMichelE::ProcessEnd(TFile* fout)
  {
    
    if (fout){
      fout->cd();
      _result_tree->Write();
    }

    return;
  }

  void ERAnaMichelE::ResetTreeVariables(){
    
    return;
  }

  }

#endif
