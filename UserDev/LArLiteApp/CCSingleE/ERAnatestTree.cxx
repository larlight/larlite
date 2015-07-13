#ifndef ERTOOL_ERANATESTTREE_CXX
#define ERTOOL_ERANATESTTREE_CXX

#include "ERAnatestTree.h"

namespace ertool {

  ERAnatestTree::ERAnatestTree(const std::string& name) : AnaBase(name)
  {
    if (_tree) { delete _tree; }
    _tree = new TTree("_tree","Result Tree");
    _tree->Branch("_x",&_x,"x/D");
  }

  void ERAnatestTree::Reset()
  {
    _x = -1000;
  }

  //void ERAnatestTree::AcceptPSet(const ::fcllite::PSet& cfg)
  //{}

  void ERAnatestTree::ProcessBegin()
  {}

  bool ERAnatestTree::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    auto const& mc_graph = MCParticleGraph();
    
      for (auto &p : mc_graph.GetParticleArray())
      {
        _x = p.Vertex()[0];
        _tree->Fill();
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
