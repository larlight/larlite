#ifndef ERTOOL_ALGO_CLASS_NAME_CXX
#define ERTOOL_ALGO_CLASS_NAME_CXX

#include "Algo_Class_Name.h"

namespace ertool {

  Algo_Class_Name::Algo_Class_Name() : AlgoBase()
  {
    _name     = "Algo_Class_Name";
  }

  void Algo_Class_Name::Reset()
  {}

  ParticleSet Algo_Class_Name::Reconstruct(const EventData &data)
  { return ParticleSet(); }

}

#endif
