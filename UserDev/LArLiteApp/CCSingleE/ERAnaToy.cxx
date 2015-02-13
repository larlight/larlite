#ifndef ERTOOL_ERANATOY_CXX
#define ERTOOL_ERANATOY_CXX

#include "ERAnaToy.h"

namespace ertool {

  ERAnaToy::ERAnaToy() : AnaBase()
		       , hShowerCount(nullptr)
		       , _target_particle(0,0)
  {
    _name     = "ERAnaToy";
    _verbose  = false;
    _target_particle.AddDaughter(Particle(11,0));
  }

  void ERAnaToy::Reset()
  {}

  void ERAnaToy::ProcessBegin()
  {
    hShowerCount = new TH1D("hShowerCount","",10,-0.5,9.5);
  }

  bool ERAnaToy::Analyze(const EventData &data, const ParticleSet &ps)
  { 
    // Print out reco's diagram
    for(auto const& p : ps)
      std::cout<<p.Diagram()<<std::endl;

    // Obtain MC ParticleSet. In particular make a copy (i.e. no reference)
    auto mc_ps = MCParticleSet();

    // "Collapse" => combine multiple particles in ParticleSet @ same vtx & time and set to unknown type
    mc_ps.Collapse();

    // Search for a target particle interaction type
    auto const& p_iter = mc_ps.Find(_target_particle);
    // If not found, return
    if(p_iter == mc_ps.end()) return false;
    // If found, print Diagram
    std::cout<<(*p_iter).Diagram()<<std::endl;
    // Fill Lepton count histogram
    double ctr=0;
    for(auto const& d : (*p_iter).Daughters())
      if(d.Type() == Particle::kShower) ctr++;
    if(ctr<1) std::cout<<"No shower?!"<<std::endl;
    hShowerCount->Fill(ctr);
    return true; 
  }

  void ERAnaToy::ProcessEnd(TFile* fout)
  {
    if(fout) {
      fout->cd();
      hShowerCount->Write();
    }
  }
}

#endif
