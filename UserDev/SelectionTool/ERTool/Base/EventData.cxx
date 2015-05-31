#ifndef ERTOOL_EVENTDATA_CXX
#define ERTOOL_EVENTDATA_CXX

#include "EventData.h"
#include "Particle.h"

namespace ertool {

  EventData::EventData()
  { Reset(); }

  void EventData::Reset()
  {
    _shower_v.clear();
    _track_v.clear();
  }

  const Shower& EventData::Shower (RecoID_t id)
  {
    if( id >= _shower_v.size() ) 
      throw ERException(Form("No shower corresponds to an id %zu",id));
    return _shower_v[id];
  }

  const Shower& EventData::Shower (const Particle& p)
  {
    if( p.RecoType() != kShower )
      throw ERException("Shower object queried for non-shower particle!");
    return this->Shower( p.RecoID() );
  }

  const Track& EventData::Track (RecoID_t id)
  {
    if( id >= _track_v.size() ) 
      throw ERException(Form("No track corresponds to an id %zu",id));
    return _track_v[id];
  }

  const Track& EventData::Track (const Particle& p)
  {
    if( p.RecoType() != kTrack )
      throw ERException("Track object queried for non-track particle!");
    return this->Track( p.RecoID() );
  }
  
  void EventData::Add(const ::ertool::Shower& obj) 
  { 
    _shower_v.push_back(obj); 
    _shower_v.back().SetRecoInfo(_shower_v.size()-1, kShower);
  }

  void EventData::Add(const ::ertool::Track&  obj) 
  { 
    _track_v.push_back(obj);  
    _track_v.back().SetRecoInfo(_track_v.size()-1, kTrack);
  }

  void EventData::Emplace(const ::ertool::Shower&& obj)
  { 
    _shower_v.emplace_back(obj); 
    _shower_v.back().SetRecoInfo(_shower_v.size()-1, kShower);
  }

  void EventData::Emplace(const ::ertool::Track&&  obj)
  { 
    _track_v.emplace_back(obj);  
    _track_v.back().SetRecoInfo(_track_v.size()-1, kTrack);
  }

}
#endif
