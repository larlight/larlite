#ifndef ERTOOL_EVENTDATA_CXX
#define ERTOOL_EVENTDATA_CXX

#include "EventData.h"
#include "Particle.h"

namespace ertool {

  EventData::EventData()
  {
    Reset();
  }

  void EventData::Reset()
  {
    _shower_v.clear();
    _track_v.clear();
    _shower_id_v.clear();
    _track_id_v.clear();
    _event_id = _run = _subrun = kUINT_MAX;
  }
  
  void EventData::SetID(unsigned int evID,
			unsigned int runID,
			unsigned int subrunID)
  {
    _event_id = evID;
    _run      = runID;
    _subrun   = subrunID;
  }

  const Flash& EventData::Flash (const FlashID_t& id) const
  {
    if( id >= _flash_v.size() ) 
      throw ERException(Form("No flash corresponds to an id %zu",id));
    return _flash_v[id];
  }

  const Flash& EventData::Flash (const Particle& p) const
  {
    return this->Flash(p.FlashID());
  }

  const Shower& EventData::Shower (const RecoID_t& id) const
  {
    if( id >= _shower_v.size() ) 
      throw ERException(Form("No shower corresponds to an id %zu",id));
    return _shower_v[id];
  }

  const Shower& EventData::Shower (const Particle& p) const
  {
    if( p.RecoType() != kShower )
      throw ERException("Shower object queried for non-shower particle!");
    return this->Shower( p.RecoID() );
  }

  const Track& EventData::Track (const RecoID_t& id) const
  {
    if( id >= _track_v.size() ) 
      throw ERException(Form("No track corresponds to an id %zu",id));
    return _track_v[id];
  }

  const Track& EventData::Track (const Particle& p) const
  {
    if( p.RecoType() != kTrack )
      throw ERException("Track object queried for non-track particle!");
    return this->Track( p.RecoID() );
  }

  const RecoInputID_t& EventData::InputID(const RecoType_t& reco_type,
					  const RecoID_t&   reco_id) const
  {
    if(reco_type == kShower && _shower_id_v.size() < reco_id)
      return _shower_id_v[reco_id];
    if(reco_type == kTrack && _track_id_v.size() < reco_id)
      return _track_id_v[reco_id];
    return kINVALID_RECO_INPUT_ID;
  }
  
  const RecoInputID_t& EventData::InputID(const Particle& p) const
  {
    return InputID(p.RecoType(),p.RecoID());
  }

  void EventData::Add(const ertool::Flash& obj,
		      const ertool::RecoInputID_t& id)
  {
    _flash_id_v.push_back(id);
    _flash_v.push_back(obj); 
    _flash_v.back().SetFlashID(_flash_v.size()-1);
  }

  void EventData::Add(const ertool::Shower& obj,
		      const ertool::RecoInputID_t& id)
  {
    _shower_id_v.push_back(id);
    _shower_v.push_back(obj); 
    _shower_v.back().SetRecoInfo(_shower_v.size()-1, kShower);
  }

  void EventData::Add(const ertool::Track& obj,
		      const ertool::RecoInputID_t& id)
  {
    _track_id_v.push_back(id);
    _track_v.push_back(obj);  
    _track_v.back().SetRecoInfo(_track_v.size()-1, kTrack);
  }

  void EventData::Emplace(const ertool::Flash&& obj,
			  const ertool::RecoInputID_t&& id)
  {
    _flash_id_v.emplace_back(id);
    _flash_v.emplace_back(obj); 
    _flash_v.back().SetFlashID(_flash_v.size()-1);
  }

  void EventData::Emplace(const ertool::Shower&& obj,
			  const ertool::RecoInputID_t&& id)
  {
    _shower_id_v.emplace_back(id);
    _shower_v.emplace_back(obj); 
    _shower_v.back().SetRecoInfo(_shower_v.size()-1, kShower);
  }

  void EventData::Emplace(const ertool::Track&& obj,
			  const ertool::RecoInputID_t&& id)
  {
    _track_id_v.emplace_back(id);
    _track_v.emplace_back(obj);  
    _track_v.back().SetRecoInfo(_track_v.size()-1, kTrack);
  }

}
#endif
