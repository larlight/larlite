#ifndef ERTOOL_ALGO_CLASS_NAME_CXX
#define ERTOOL_ALGO_CLASS_NAME_CXX

#include "Algo_Class_Name.h"

namespace ertool {

  Algo_Class_Name::Algo_Class_Name(const std::string& name) : AlgoBase(name)
  {}

  void Algo_Class_Name::Reset()
  {}

  void Algo_Class_Name::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void Algo_Class_Name::ProcessBegin()
  {}

  bool Algo_Class_Name::Reconstruct(const EventData &data, ParticleGraph& graph)
  {return true;}

  void Algo_Class_Name::ProcessEnd(TFile* fout)
  {}

}

#endif
