#ifndef ERTOOL_ANABASE_CXX
#define ERTOOL_ANABASE_CXX

#include "AnaBase.h"
#include "ERException.h"

namespace ertool {

  AnaBase::AnaBase(const std::string& name) 
    : UnitBase(name)
    , _mc_data (nullptr)
    , _mc_graph   (nullptr)
  {}

  bool AnaBase::Analyze(const EventData& data,
			const ParticleGraph& graph)
  {
    return true;
  }

  void AnaBase::SetMCData(EventData& data, ParticleGraph& graph)
  {
    _mc_data = &data;
    _mc_graph   = &graph;
  }

  void AnaBase::UnsetMCData()
  {
    _mc_data = nullptr;
    _mc_graph = nullptr;
  }

  const EventData& AnaBase::MCEventData() const
  {
    if(!_mc_data) throw ERException("MC Data not available!");
    return *_mc_data;
  }
  
  const ParticleGraph& AnaBase::MCParticleGraph() const
  {
    if(!_mc_graph) throw ERException("MC Data not available!");
    return *_mc_graph;
  }
 
}
#endif
