#ifndef ERTOOL_ALGOSINGLEGAMMA_CXX
#define ERTOOL_ALGOSINGLEGAMMA_CXX

#include "AlgoSingleGamma.h"

namespace ertool {

  AlgoSingleGamma::AlgoSingleGamma() : AlgoBase()
  {
    _name     = "AlgoSingleGamma";
  }

  void AlgoSingleGamma::Reset()
  {}

  ParticleSet AlgoSingleGamma::Reconstruct(const EventData &data)
  { return ParticleSet(); }

}

#endif
