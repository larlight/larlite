#ifndef ERTOOL_ERANAEMPART_CXX
#define ERTOOL_ERANAEMPART_CXX

#include "ERAnaEMPart.h"

namespace ertool {

  ERAnaEMPart::ERAnaEMPart(const std::string& name)
    : AnaBase(name)
    , _result_tree(nullptr)
    , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)

  {

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");
    _result_tree->Branch("_dist_shrs",&_dist_shrs,"dist_shrs/D");
    _result_tree->Branch("_dist_part",&_dist_part,"dist_part/D");
    _result_tree->Branch("_dedx",&_dedx,"dedx/D");
    _result_tree->Branch("_pdg_reco",&_pdg_reco,"pdg_reco/I");
    _result_tree->Branch("_pdg_mc",&_pdg_mc,"pdg_mc/I");
    _result_tree->Branch("_dot",&_dot,"dot/D");
    _result_tree->Branch("_e_reco",&_e_reco,"e_reco/D");
    _result_tree->Branch("_e_mc",&_e_mc,"e_mc/D");
    // keep track of number of events gone by
    _numEvts = 0;

    // set verbosity
    _debug = false;

  }

  void ERAnaEMPart::Reset()
  {}

  bool ERAnaEMPart::Analyze(const EventData &data, const ParticleGraph &graph)
  {

    if (_debug)
      std::cout << "******  Begin ERAnaEMPart Analysis  ******" << std::endl;

    // Get MC particle set
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_evtdata = MCEventData();
    auto const& mcparticles = mc_graph.GetParticleArray();

    // define variables to use
    ::ertool::Particle recoPart;
    ::ertool::Particle mcPart;
    ::ertool::Particle closestPart;
    double minDist;
    
    // loop through particles reconstructed
    auto const& particles = graph.GetParticleArray();
    for (std::deque<::ertool::Particle>::const_iterator it = particles.begin(); it != particles.end(); it++){
      recoPart = *it;
      if ( (recoPart.PdgCode() == 11) or (recoPart.PdgCode() == 22) ){
	minDist = 1036;
	// find the shower in MCParticleGraph with the closest start/end point
	for (std::deque<::ertool::Particle>::const_iterator mcit = mcparticles.begin(); mcit != mcparticles.end(); mcit++){
	  mcPart = *mcit;
	  if ( (mcPart.PdgCode() == 11) or (mcPart.PdgCode() == 22) ){
	    // check distance between shower start points
	    if (recoPart.RecoID() < 1000 && mcPart.RecoID() < 1000){
	    auto const& recoShrStart = data.Shower(recoPart.RecoID()).Start();
	    auto const& mcShrStart   = mc_evtdata.Shower(mcPart.RecoID()).Start();
	    if ( recoShrStart.Dist(mcShrStart) < minDist ){
	      minDist = recoShrStart.Dist(mcShrStart);
	      closestPart = mcPart;
	    }
	    }
	  }// if a shower
	}// for all MCParticles
	if (minDist < 1036){
	  _dist_shrs = minDist;
	  _dist_part = recoPart.Vertex().Dist(closestPart.Vertex());
	  _dedx = data.Shower(recoPart.RecoID())._dedx;
	  _e_reco   = recoPart.Energy();
	  _e_mc     = closestPart.Energy();
	  _dot      = closestPart.Momentum().Dot(recoPart.Momentum())/(closestPart.Momentum().Length()*recoPart.Momentum().Length());
	  _pdg_reco = recoPart.PdgCode();
	  _pdg_mc   = closestPart.PdgCode();
	  _result_tree->Fill();
	  if (_debug){
	    std::cout << "Comparing RECO shower to MC w/ nearest start point:" << std::endl
		      << "Ereco: " << _e_reco << "\tE mc: " << _e_mc << std::endl
		      << "Shr Distance : " << _dist_shrs << std::endl
	      	      << "Part Distance: " << _dist_part << std::endl
		      << "Direction Dot: " << _dot << std::endl
		      << "PDG reco: " << _pdg_reco << "\tPDG mc: " << _pdg_mc << std::endl
		      << "dE/dx: " << _dedx << std::endl << std::endl;
	  }
	}
      }// if reco particle is shower
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
  
  void ERAnaEMPart::ProcessEnd(TFile* fout)
  {
    
    if (fout){
      fout->cd();
      _result_tree->Write();
    }

    return;
  }

  void ERAnaEMPart::ResetTreeVariables(){
    
    return;
  }

  }

#endif
