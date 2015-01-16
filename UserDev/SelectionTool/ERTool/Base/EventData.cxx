#ifndef ERTOOL_EVENTDATA_CXX
#define ERTOOL_EVENTDATA_CXX

#include "EventData.h"

namespace ertool {

  EventData::EventData()
  { Reset(); }

  void EventData::Reset()
  {
    _shower_v.clear();
    _track_v.clear();
    _vertex_v.clear();
  }

  const Shower& EventData::Shower (size_t id) const
  { 
    if(id >= _shower_v.size()) 
      throw ERException(Form("Invalid shower id (%zu: only %zu showers available)",
			     id, _shower_v.size()));
    return _shower_v[id];
  }

  const Track& EventData::Track (size_t id) const
  {
    if(id >= _track_v.size()) 
      throw ERException(Form("Invalid track id (%zu: only %zu tracks available)",
			     id, _track_v.size()));
    return _track_v[id];
  }

  const Vertex& EventData::Vertex (size_t id) const
  {
    if(id >= _vertex_v.size()) 
      throw ERException(Form("Invalid vertex id (%zu: only %zu vertexs available)",
			     id, _vertex_v.size()));
    return _vertex_v[id];
  }

  std::vector< const ::ertool::Shower* > EventData::Shower() const
  {
    std::vector< const ::ertool::Shower* > res;
    for(size_t i=0; i<_shower_v.size(); ++i)
      if(!(_shower_v[i].IsFiltered())) res.push_back( &(_shower_v[i]) );
    return res;
  }

  std::vector< const ::ertool::Track* > EventData::Track() const
  {
    std::vector< const ::ertool::Track* > res;
    for(size_t i=0; i<_track_v.size(); ++i)
      if(!(_track_v[i].IsFiltered())) res.push_back( &(_track_v[i]) );
    return res;
  }

  std::vector< const ::ertool::Vertex* > EventData::Vertex() const
  {
    std::vector< const ::ertool::Vertex* > res;
    for(size_t i=0; i<_vertex_v.size(); ++i)
      if(!(_vertex_v[i].IsFiltered())) res.push_back( &(_vertex_v[i]) );
    return res;
  }

  void EventData::FilterShower (size_t id, bool filter)
  {
    if(id >= _shower_v.size()) 
      throw ERException(Form("Invalid shower id (%zu: only %zu showers available)",
			     id, _vertex_v.size()));
    _shower_v[id].Filter(filter);
  }

  void EventData::FilterTrack (size_t id, bool filter)
  {
    if(id >= _track_v.size()) 
      throw ERException(Form("Invalid track id (%zu: only %zu tracks available)",
			     id, _vertex_v.size()));
    _track_v[id].Filter(filter);
  }

  void EventData::FilterVertex (size_t id, bool filter)
  {
    if(id >= _vertex_v.size()) 
      throw ERException(Form("Invalid vertex id (%zu: only %zu vertexs available)",
			     id, _vertex_v.size()));
    _vertex_v[id].Filter(filter);
  }



}
#endif
