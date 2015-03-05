#ifndef ERTOOL_ERFILTERSECONDARIES_CXX
#define ERTOOL_ERFILTERSECONDARIES_CXX

#include "ERFilterSecondaries.h"

namespace ertool {

  ERFilterSecondaries::ERFilterSecondaries() : FilterBase()
  {
    _name = "ERFilterSecondaries";
    _min_dist_trk_trk = 1; 
    _use_perpdist_trk_trk = true;
    _min_dist_trk_shr = 1; 
    _use_perpdist_trk_shr = false;
    _seed_trk_s.clear();
    _removed_trk_v.clear();
    _remaining_trk_v.clear();
    _remaining_shr_v.clear();
  }

  void ERFilterSecondaries::RegisterSeed(const size_t trk_id)
  { _seed_trk_s.insert(trk_id); }

  void ERFilterSecondaries::Filter(EventData& data)
  { 
    // Make sure seed tracks exist
    for(auto const& trk_id : _seed_trk_s)
      if(trk_id >= data.AllTrack().size()) throw ERException("Invalid track id provided in the seed list!");

    _removed_trk_v.clear();
    _removed_trk_v.resize(1);
    for(auto const& seed : _seed_trk_s)
      _removed_trk_v.back().insert(seed);
    
    _remaining_trk_v.clear();
    for(auto const& trk : data.Track())
      _remaining_trk_v.push_back(trk->ID());

    _remaining_shr_v.clear();
    for(auto const& shr : data.Shower())
      _remaining_shr_v.push_back(shr->ID());

    ::geoalgo::Point_t pt(3,0);
    while(_removed_trk_v.back().size()) {

      auto const& seed_v = _removed_trk_v.back();
      std::set<size_t> remove_s;

      for(auto const& trk_id : _remaining_trk_v) {

	auto const& trk = data.Track(trk_id);
	
	for(auto const& seed_id : seed_v) {

	  auto const& seed = data.Track(seed_id);
	  
	  // If the seed and the subject track are close in start point, ignore this seed
	  auto dist = trk[0].Dist(seed[0]);
	  if(dist < _min_dist_trk_trk) continue;
	  
	  if(_use_perpdist_trk_trk) {
	    // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
	    auto dist = sqrt(_geoalg.SqDist(trk[0],seed));
	    if(dist > _min_dist_trk_trk) continue;

	  }else{
	    /*
	      If the distance between the closest approach mid-point and the subject trajectory is 
	      larger than the cut value, ignore
	    */
	    dist = _relalg.FindClosestApproach(trk,seed,pt);
	    if(dist > _min_dist_trk_trk) continue;

	    
	    dist = pt.Dist(seed[0]);
	    if(dist < _min_dist_trk_trk) continue;
	  }
	 
	  remove_s.insert(seed_id);
	  break;
	}
      }

      for(auto const& shr_id : _remaining_shr_v) {

	auto const& shr = data.Shower(shr_id);
	
	for(auto const& seed_id : seed_v) {

	  auto const& seed = data.Track(seed_id);

	  // If the seed and the subject track are close in start point, ignore this seed
	  auto dist = shr.Start().Dist(seed[0]);
	  if(dist < _min_dist_trk_shr) continue;

	  if(_use_perpdist_trk_shr) {
	    // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
	    auto dist = sqrt(_geoalg.SqDist(shr.Start(),seed));
	    if(dist > _min_dist_trk_shr) continue;

	  }else{
	    /*
	      If the distance between the closest approach mid-point and the subject trajectory is 
	      larger than the cut value, ignore
	    */
	    dist = _relalg.FindClosestApproach(shr,seed,pt);
	    if(dist > _min_dist_trk_shr) continue;

	    
	    dist = pt.Dist(seed[0]);
	    if(dist < _min_dist_trk_shr) continue;
	  }

	  data.FilterShower(shr.ID());
	  break;
	}
      }

      size_t index=0;
      while(index<_remaining_trk_v.size()) {
	auto trk_id = _remaining_trk_v.begin()+index;
	if(remove_s.find((*trk_id)) != remove_s.end())
	  _remaining_trk_v.erase(trk_id);
	else
	  ++index;
      }

      _removed_trk_v.emplace_back(remove_s);
    }

    for(size_t i=0; i<_removed_trk_v.size(); ++i) {

      if(!i) continue; // Skip seed tracks

      auto const& trk_id_s = _removed_trk_v[i];

      for(auto const& trk_id : trk_id_s)

	data.FilterTrack(trk_id);
    }

    _seed_trk_s.clear();
  }
}

#endif
