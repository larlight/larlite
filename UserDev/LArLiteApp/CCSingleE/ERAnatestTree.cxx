#ifndef ERTOOL_ERANATESTTREE_CXX
#define ERTOOL_ERANATESTTREE_CXX

#include "ERAnatestTree.h"

namespace ertool {

  ERAnatestTree::ERAnatestTree(const std::string& name) : AnaBase(name),
  _tree(nullptr)
  {
    if (_tree) { delete _tree; }
    _tree = new TTree("_tree","Result Tree");
    _tree->Branch("_xMC",&_xMC,"xMC/D");
    _tree->Branch("_yMC",&_yMC,"yMC/D");
    _tree->Branch("_zMC",&_zMC,"zMC/D");
    _tree->Branch("_eMC",&_eMC,"eMC/D");
    _tree->Branch("_keMC",&_keMC,"keMC/D");
    _tree->Branch("_mMC",&_mMC,"mMC/D");
    _tree->Branch("_xReco",&_xReco,"xReco/D");
    _tree->Branch("_yReco",&_yReco,"yReco/D");
    _tree->Branch("_zReco",&_zReco,"zReco/D");
    _tree->Branch("_eReco",&_eReco,"eReco/D");
    _tree->Branch("_keReco",&_keReco,"keReco/D");
    _tree->Branch("_mReco",&_mReco,"mReco/D");
    counter = 0;
    event_counter = 0;
    neutrino = 0;
  }

  void ERAnatestTree::Reset()
  {}

  void ERAnatestTree::PrepareTTree(){
    _xMC = -1000 ;
    _xReco = -1000 ;
    _yMC = -1000 ;
    _yReco = -1000 ;
    _zMC = -1000 ;
    _zReco = -1000 ;
    _mMC = -1000 ;
    _mReco = -1000 ;
    _eMC = -1000 ;
    _eReco = -1000 ;
    _keMC = -1000 ;
    _keReco = -1000 ;
    }

  void ERAnatestTree::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnatestTree::ProcessBegin()
  {}

  bool ERAnatestTree::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    
    event_counter++;
    PrepareTTree() ;
    auto const& mc_graph = MCParticleGraph();
    
     for (auto &p : mc_graph.GetParticleArray())
      {
        if (p.PdgCode() != 11) continue;
	else {
	  _xMC = p.Vertex()[0];
	  //std::cout << "\n" << "\n";
	  //std::cout << "*************************************" << "\n";
	  //std::cout << "************ NEW SHOWER *************" << "\n";
	  //std::cout << "*************************************" << "\n";
	  //std::cout << "\n\n" << "The MC x coordinate is " << _xMC << "\n";
	  _yMC = p.Vertex()[1];
	  //std::cout << "The MC y coordinate is " << _yMC << "\n";
	  _zMC = p.Vertex()[2];
	  //std::cout << "The MC z coordinate is " << _zMC << "\n";
	  _eMC = p.Energy();
	  //std::cout << "The MC PDG is " << p.PdgCode() << "\n";
	  //std::cout << "The MC energy is " << _eMC << " \n";
	  _keMC = p.KineticEnergy();
	  //std::cout << "The MC kinetic energy is " << _keMC << " \n";
	  _mMC = p.Mass();
	  //std::cout << "The MC mass is " << _mMC << " \n";
	  _tree->Fill();
	}
      }

     for (auto &p : ps.GetParticleArray())
       {
        if (p.PdgCode() != 11) continue;
	else {
	  _xReco = p.Vertex()[0];
	  //std::cout << "*************************************" << "\n";
	  //std::cout << "********* COMPARE WITH RECO *********" << "\n";
	  //std::cout << "*************************************" << "\n";
	  //std::cout << "The Reco x coordinate is " << _xReco << "\n";
	  _yReco = p.Vertex()[1];
	  //std::cout << "The Reco y coordinate is " << _yReco << "\n";
	  _zReco = p.Vertex()[2];
	  //std::cout << "The Reco z coordinate is " << _zReco << "\n";
	  _eReco = p.Energy();
	  //std::cout << "Reco PDG is " << p.PdgCode() << "\n";
	  //std::cout << "The Reco energy is " << _eReco << " \n";
	  _keReco = p.KineticEnergy();
	  //std::cout << "The Reco kinetic energy is " << _keReco << " \n";
	  _mReco = p.Mass();
	  //std::cout << "The Reco mass is " << _mReco << " \n";
	  _tree->Fill();
	  counter++;
	  //std::cout << "\n\n\n ";
	}

      }

     for (auto &p : ps.GetParticleArray()){

       if (p.PdgCode() == 12) {
	 neutrino++ ;
       }
     }



     if (neutrino != 1) {
       std::cout << " Missed this event!" << "\n" ;
       std::cout << "Missed event is: " << event_counter <<"\n";
       }


    return true; 
  }

  void ERAnatestTree::ProcessEnd(TFile* fout)
  {
    std::cout << "Events Selected: " << counter << "\n";
    std::cout << "Total Events: " << event_counter << "\n";
    std::cout << "Selection Efficiency: " << float (counter)/event_counter << "\n";
    if (fout){
      fout->cd();
      _tree->Write();
     }
  }

}

#endif
