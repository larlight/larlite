#ifndef LARLITE_TRACKMOM_CXX
#define LARLITE_TRACKMOM_CXX

#include "trackmom.h"
namespace larlite {

  trackmom::trackmom() : data_base(data::kTrackMomentum)
  {
    clear_data();
  }

  trackmom::trackmom(const trackmom& original)
    : data_base(original)
    , _momentum(original._momentum)
  {
    if(_momentum.size() < data::kTrackMomentumTypeMax)
      _momentum.resize(data::kTrackMomentumTypeMax,-1);
  }
    
  void trackmom::clear_data()
  {
    _momentum.resize(data::kTrackMomentumTypeMax,-1);
    for(auto& v : _momentum) v = -1;
  }
  
  void trackmom::Momentum(const data::TrackMomentumType_t type,
			  const double mom)
  {
    if(_momentum.size() < data::kTrackMomentumTypeMax)
      _momentum.resize(data::kTrackMomentumTypeMax,-1);
    _momentum[type]=mom;
  }

  double trackmom::Momentum(const data::TrackMomentumType_t type)
  {
    if(_momentum.size() < data::kTrackMomentumTypeMax)
      _momentum.resize(data::kTrackMomentumTypeMax,-1);
    return _momentum[type];
  }
}

#endif
