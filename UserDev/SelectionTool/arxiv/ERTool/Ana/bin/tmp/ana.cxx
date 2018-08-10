#ifndef ERTOOL_ANA_CLASS_NAME_CXX
#define ERTOOL_ANA_CLASS_NAME_CXX

#include "Ana_Class_Name.h"

namespace ertool {

  Ana_Class_Name::Ana_Class_Name(const std::string& name) : AnaBase(name)
  {}

  void Ana_Class_Name::Reset()
  {}

  void Ana_Class_Name::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void Ana_Class_Name::ProcessBegin()
  {}

  bool Ana_Class_Name::Analyze(const EventData &data, const ParticleGraph &ps)
  { return true; }

  void Ana_Class_Name::ProcessEnd(TFile* fout)
  {}

}

#endif
