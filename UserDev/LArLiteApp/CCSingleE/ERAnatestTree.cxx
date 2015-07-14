#ifndef ERTOOL_ERANATESTTREE_CXX
#define ERTOOL_ERANATESTTREE_CXX

#include "ERAnatestTree.h"

namespace ertool {

  ERAnatestTree::ERAnatestTree(const std::string& name) : AnaBase(name),
  _tree(nullptr)
  {
    if (_tree) { delete _tree; }
    _tree = new TTree("_tree","Result Tree");
    _tree->Branch("_x",&_x,"x/D");
    _tree->Branch("_y",&_y,"y/D");
    _tree->Branch("_z",&_z,"z/D");
    _tree->Branch("_e",&_e,"e/D");
    _tree->Branch("_ke",&_ke,"ke/D");
    _tree->Branch("_m",&_m,"m/D");
  }

  void ERAnatestTree::Reset()
  {}

  void ERAnatestTree::PrepareTTree(){
    _x = -1000 ;
    }

  void ERAnatestTree::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnatestTree::ProcessBegin()
  {}

  bool ERAnatestTree::Analyze(const EventData &data, const ParticleGraph &ps)
  {

    PrepareTTree() ;
    //auto const& mc_graph = MCParticleGraph();
    
     for (auto &p : ps.GetParticleArray())
      {
        if (abs(p.PdgCode()) != 11 && abs(p.PdgCode()) != 22 ) continue;
	else {
	  _x = p.Vertex()[0];
	  _y = p.Vertex()[1];
	  _z = p.Vertex()[2];
	  _e = p.Energy();
	  std::cout << "\n\nPDG is " << p.PdgCode() << "\n";
	  std::cout << "The energy is " << _e << " \n";
	  _ke = p.KineticEnergy();
	  std::cout << "The kinetic energy is " << _ke << " \n";
	  _m = p.Mass();
	  std::cout << "The mass is " << _m << " \n";
	  _tree->Fill();
	  std::cout << "\n\n ";
	}
      }

    return true; 
  }

  void ERAnatestTree::ProcessEnd(TFile* fout)
  {
     if (fout){
      fout->cd();
      _tree->Write();
     }
  }

}

#endif
