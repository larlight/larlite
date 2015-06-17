#ifndef ERTOOL_ERALGOTOY_CXX
#define ERTOOL_ERALGOTOY_CXX

#include "ERAlgoToy.h"

namespace ertool {

  ERAlgoToy::ERAlgoToy(const std::string& name) : AlgoBase(name)
  {
    auto& p = OutputPSet();
    p.add_value("aho","a");
  }

  void ERAlgoToy::Reset()
  {}

  void ERAlgoToy::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    std::cout << " called... " << std::endl;
    if(cfg.contains_pset(this->Name())) {

      std::cout << cfg.get_pset(this->Name()).dump() << std::endl;
      std::cout << cfg.get_pset(this->Name()).get<double>("boke")<<std::endl;
    }
  }

  void ERAlgoToy::ProcessBegin()
  {}

  bool ERAlgoToy::Reconstruct(const EventData &data, ParticleGraph& graph)
  {return true;}

  void ERAlgoToy::ProcessEnd(TFile* fout)
  {}

}

#endif
