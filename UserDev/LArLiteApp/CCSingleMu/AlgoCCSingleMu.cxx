#ifndef ERTOOL_ALGOCCSINGLEMU_CXX
#define ERTOOL_ALGOCCSINGLEMU_CXX

#include "AlgoCCSingleMu.h"

namespace ertool {

  AlgoCCSingleMu::AlgoCCSingleMu() : AlgoBase()
  {
    _name     = "AlgoCCSingleMu";
  }

  void AlgoCCSingleMu::Reset()
  {}

  ParticleSet AlgoCCSingleMu::Reconstruct(const EventData &data)
  { return ParticleSet(); }

}

#endif
