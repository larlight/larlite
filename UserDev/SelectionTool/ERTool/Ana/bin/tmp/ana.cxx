#ifndef ERTOOL_ANA_CLASS_NAME_CXX
#define ERTOOL_ANA_CLASS_NAME_CXX

#include "Ana_Class_Name.h"

namespace ertool {

  Ana_Class_Name::Ana_Class_Name() : AnaBase()
  {
    _name     = "Ana_Class_Name";
  }

  void Ana_Class_Name::Reset()
  {}

  bool Ana_Class_Name::Analyze(const EventData &data, const ParticleSet &ps)
  { return true; }

}

#endif
