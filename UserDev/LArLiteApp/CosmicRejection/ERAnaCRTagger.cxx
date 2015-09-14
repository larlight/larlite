#ifndef ERTOOL_ERANACRTAGGER_CXX
#define ERTOOL_ERANACRTAGGER_CXX

#include "ERAnaCRTagger.h"

namespace ertool {

  ERAnaCRTagger::ERAnaCRTagger(const std::string& name) : AnaBase(name)
  {}

  void ERAnaCRTagger::Reset()
  {}

  void ERAnaCRTagger::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaCRTagger::ProcessBegin()
  {
    _event_tree = new TTree("event_tree","");
    _event_tree->Branch("run",&_run,"run/i");
    _event_tree->Branch("subrun",&_subrun,"subrun/i");
    _event_tree->Branch("event",&_event,"event/i");
    _event_tree->Branch("ctr_cosmic",&_ctr_cosmic,"ctr_cosmic/i");
    _event_tree->Branch("ctr_non_cosmic",&_ctr_non_cosmic,"ctr_non_cosmic/i");
    
    _part_tree = new TTree("part_tree","");
    _part_tree->Branch("run",&_run,"run/i");
    _part_tree->Branch("subrun",&_subrun,"subrun/i");
    _part_tree->Branch("event",&_event,"event/i");
    _part_tree->Branch("pdg",&_pdg,"pdg/I");
    _part_tree->Branch("primary",&_primary,"primary/i");
    _part_tree->Branch("x",&_x,"x/D");
    _part_tree->Branch("y",&_y,"y/D");
    _part_tree->Branch("z",&_z,"z/D");
    _part_tree->branch("ex",&_ex,"ex/D");
    _part_tree->branch("ey",&_ey,"ey/D");
    _part_tree->branch("ez",&_ez,"ez/D");
    _part_tree->Branch("px",&_px,"px/D");
    _part_tree->Branch("py",&_py,"py/D");
    _part_tree->Branch("pz",&_pz,"pz/D");

  }

  bool ERAnaCRTagger::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    
    return true;
  }

  void ERAnaCRTagger::ProcessEnd(TFile* fout)
  {}

}

#endif
