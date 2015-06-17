#ifndef ERTOOL_ANABASE_CXX
#define ERTOOL_ANABASE_CXX

#include "AnaBase.h"

namespace ertool {

  AnaBase::AnaBase() 
    : UnitBase()
    , _mc_data (nullptr)
    , _mc_ps   (nullptr)
  { _name="AnaBase"; }

  bool AnaBase::Analyze(const EventData& data,
			const ParticleSet& ps)
  {
    return true;
  }

  void AnaBase::SetMCData(EventData& data, ParticleSet& ps)
  {
    _mc_data = &data;
    _mc_ps   = &ps;
  }

  void AnaBase::UnsetMCData()
  {
    _mc_data = nullptr;
    _mc_ps = nullptr;
  }

  const EventData& AnaBase::MCEventData() const
  {
    if(!_mc_data) throw ERException("MC Data not available!");
    return *_mc_data;
  }
  
  const ParticleSet& AnaBase::MCParticleSet() const
  {
    if(!_mc_ps) throw ERException("MC Data not available!");
    return *_mc_ps;
  }
 
}
#endif
