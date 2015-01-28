#ifndef MCBTALG_CXX
#define MCBTALG_CXX

#include "MCBTAlg.h"
#include <TStopwatch.h>
namespace btutil {

  MCBTAlg::MCBTAlg(const std::vector<unsigned int>& g4_trackid_v,
		   const std::vector<larlite::simch>& simch_v)
  {
    Reset(g4_trackid_v,simch_v);
  }

  void MCBTAlg::Reset(const std::vector<unsigned int>& g4_trackid_v,
		      const std::vector<larlite::simch>& simch_v)
  {
    _num_parts = 0;
    _sum_mcq.clear();
    _trkid_to_index.clear();
    _event_info.clear();
    // 
    for(auto const& id : g4_trackid_v)
      Register(id);
    _num_parts++;
    ProcessSimChannel(simch_v);

  }

  void MCBTAlg::Reset(const std::vector<std::vector<unsigned int> >& g4_trackid_v,
		      const std::vector<larlite::simch>& simch_v)
  {
    _num_parts = 0;
    _sum_mcq.clear();
    _trkid_to_index.clear();
    _event_info.clear();
    // 
    for(auto const& id : g4_trackid_v)
      Register(id);
    _num_parts++;
    ProcessSimChannel(simch_v);
  }

  void MCBTAlg::ProcessSimChannel(const std::vector<larlite::simch>& simch_v)
  {

    //art::ServiceHandle<geo::Geometry> geo;
    auto geo = ::larutil::Geometry::GetME();
    _sum_mcq.resize(geo->Nplanes(),std::vector<double>(_num_parts,0));
    
    for(auto const& sch : simch_v) {
      
      auto const ch = sch.Channel();
      if(_event_info.size() <= ch) _event_info.resize(ch+1);
      
      auto& ch_info = _event_info[ch];

      //size_t plane = geo->ChannelToWire(ch)[0].Plane;
      size_t plane = geo->ChannelToPlane(ch);

      for(auto const& time_ide : sch.TDCIDEMap()) {
	
	auto const& time  = time_ide.first;
	auto const& ide_v = time_ide.second;

	auto& edep_info = ch_info[time];

	if(!edep_info.size()) edep_info.resize(_num_parts,0);

	for(auto const& ide : ide_v) {
	  
	  size_t index = kINVALID_INDEX;
	  if(ide.trackID < (int)(_trkid_to_index.size())){
	    index = _trkid_to_index[ide.trackID];
	  }
	  if(_num_parts <= index) {
	    (*edep_info.rbegin()) += ide.numElectrons;
	    (*(_sum_mcq[plane]).rbegin()) += ide.numElectrons;
	  }
	  else {
	    edep_info[index] += ide.numElectrons;
	    _sum_mcq[plane][index] += ide.numElectrons;
	  }
	}
      }
    }
  }

  const std::vector<double>& MCBTAlg::MCQSum(const size_t plane_id) const
  {
    if(plane_id > _sum_mcq.size())
      throw MCBTException(Form("Invalid plane requested: %zu",plane_id));
    return _sum_mcq[plane_id];
  }

  std::vector<double> MCBTAlg::MCQ(const WireRange_t& hit) const
  {
    std::vector<double> res(_num_parts,0);
    
    if(_event_info.size() <= hit.ch) return res;
    
    auto const& ch_info = _event_info[hit.ch];

    //art::ServiceHandle<util::TimeService> ts;
    auto ts = ::larutil::TimeService::GetME();

    auto itlow = ch_info.lower_bound((unsigned int)(ts->TPCTick2TDC(hit.start)));
    auto itup  = ch_info.upper_bound((unsigned int)(ts->TPCTick2TDC(hit.end))+1);

    while(itlow != ch_info.end() && itlow != itup) {
      
      auto const& edep_info = (*itlow).second;
      
      for(size_t part_index = 0; part_index<_num_parts; ++part_index)
	
	res[part_index] += edep_info[part_index];
      
      ++itlow;
    }
    return res;
  }


  std::vector<double> MCBTAlg::MCQFrac(const WireRange_t& hit) const
  { 
    auto res = MCQ(hit);
    if(!res.size()) return res;
    
    double sum = 0;
    for(auto const& v : res) sum += v;
    for(size_t i=0; i<(res.size()-1); ++i)
      res[i] /= (sum - (*res.rbegin()));
    (*res.rbegin()) /= sum;
    return res;
  }

  std::vector<double> MCBTAlg::MCQ(const std::vector<WireRange_t>& hit_v) const
  {
    std::vector<double> res(_num_parts,0);
    for(auto const& h : hit_v) {
      auto tmp_res = MCQ(h);
      for(size_t i=0; i<res.size(); ++i) res[i] += tmp_res[i];
    }
    return res;
  }

  std::vector<double> MCBTAlg::MCQFrac(const std::vector<WireRange_t>& hit_v) const
  {
    auto res = MCQ(hit_v);
    if(!res.size()) return res;
    
    double sum = 0;
    for(auto const& v : res) sum += v;
    for(size_t i=0; i<(res.size()-1); ++i)
      res[i] /= (sum - (*res.rbegin()));
    (*res.rbegin()) /= sum;
    return res;
  }

  size_t MCBTAlg::Index(const unsigned int g4_track_id) const
  { 
    if(g4_track_id >= _trkid_to_index.size()) return kINVALID_INDEX;
    return _trkid_to_index[g4_track_id];
  }

  void MCBTAlg::Register(const unsigned int& g4_track_id)
  { 
    if(_trkid_to_index.size() <= g4_track_id)
      _trkid_to_index.resize(g4_track_id+1,kINVALID_INDEX);
    
    if(_trkid_to_index[g4_track_id] == kINVALID_INDEX){
      _trkid_to_index[g4_track_id] = _num_parts;
      ++_num_parts;
    }
  }

  void MCBTAlg::Register(const std::vector<unsigned int>& track_id_v)
  { 
    unsigned int max_id = 0;
    for(auto const& id : track_id_v) if(max_id < id) max_id = id;
    if(_trkid_to_index.size() <= max_id)
      _trkid_to_index.resize(max_id+1,kINVALID_INDEX);

    for(auto const& id : track_id_v) {

      if(_trkid_to_index[id] == kINVALID_INDEX)

	_trkid_to_index[id] = _num_parts;

      else

	throw MCBTException(Form("Doubly used TrackID: %d",id));

    }
    ++_num_parts;
  }

}

#endif  
