#ifndef ERTOOL_ERFILTERSECONDARIES_CXX
#define ERTOOL_ERFILTERSECONDARIES_CXX

#include "ERFilterSecondaries.h"

namespace ertool {

  ERFilterSecondaries::ERFilterSecondaries() : FilterBase()
  {
    _name = "ERFilterSecondaries";
    _min_dist_trk_trk = 10; 
    _min_dist_common_origin = 5;
    _min_dist_trk_def = 20;
    _use_perpdist_trk_trk = true;
    _min_dist_trk_shr = 50; 
    _use_perpdist_trk_shr = false;
    _seed_trk_s.clear();
    _removed_trk_v.clear();
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

    ::geoalgo::Point_t pt(3,0);
    while(_removed_trk_v.back().size()) {

      auto const& seed_v = _removed_trk_v.back();
      std::set<size_t> remove_trk_s;
      std::set<size_t> remove_shr_s;

      for(auto const& trk : data.Track()) {
	
	for(auto const& seed_id : seed_v) {

	  auto const& seed = data.Track(seed_id);

	  if(trk->Length()>_min_dist_trk_def) {
	    
	    // If the seed and the subject track are close in start point, ignore this seed
	    auto dist = (*trk)[0].Dist(seed[0]);
	    if(dist < _min_dist_common_origin) {
	      //std::cout<<trk->ID()<<" : "<<seed.ID()<<" Start too close: "<<dist<<std::endl;
	      continue;
	    }
	    
	    if(_use_perpdist_trk_trk) {
	      // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
	      auto dist = sqrt(_geoalg.SqDist((*trk)[0],seed));
	      if(dist > _min_dist_trk_trk)
		continue;
	      
	    }else{
	      
	      /*
		If the distance between the closest point of approach (CPA) and the subject trajectory is 
		larger than the cut value, ignore
	      */
	      dist = _relalg.FindClosestApproach(*trk,seed,pt);
	      if(dist > _min_dist_trk_trk)
		continue;
	      
	      /*
		Check if the distance between the CPA and the subject track's start point. This has to be in
		the common origin radius
	      */
	      dist = (*trk)[0].Dist(pt);
	      if(dist > _min_dist_common_origin)
		continue;
	    }

	  }else{

	    ::geoalgo::HalfLine hl((*trk)[0],(*trk)[1]-(*trk)[0]);

	    // If the seed and the subject track are close in start point, ignore this seed
	    auto dist = hl.Start().Dist(seed[0]);
	    if(dist < _min_dist_trk_shr) continue;
	    
	    if(_use_perpdist_trk_shr) {
	      // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
	      auto dist = sqrt(_geoalg.SqDist(hl.Start(),seed));
	      if(dist > _min_dist_trk_shr) continue;
	      
	    }else{
	      /*
		If the distance between the closest approach mid-point and the subject trajectory is 
		larger than the cut value, ignore
	      */
	      dist = _relalg.FindClosestApproach(hl,seed,pt);
	      if(dist > _min_dist_trk_shr) continue;
	      
	    }
	  }

	  remove_trk_s.insert(trk->ID());
	  data.FilterTrack(trk->ID());
	  break;
	}
      }

      for(auto const& shr : data.Shower()) {
	
	for(auto const& seed_id : seed_v) {

	  auto const& seed = data.Track(seed_id);

	  // If the seed and the subject track are close in start point, ignore this seed
	  auto dist = shr->Start().Dist(seed[0]);
	  if(dist < _min_dist_trk_shr) continue;

	  if(_use_perpdist_trk_shr) {
	    // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
	    auto dist = sqrt(_geoalg.SqDist(shr->Start(),seed));
	    if(dist > _min_dist_trk_shr) continue;

	  }else{
	    /*
	      If the distance between the closest approach mid-point and the subject trajectory is 
	      larger than the cut value, ignore
	    */
	    dist = _relalg.FindClosestApproach(*shr,seed,pt);
	    if(dist > _min_dist_trk_shr) continue;
	    
	  }

	  data.FilterShower(shr->ID());
	  break;
	}

	if(shr->IsFiltered()) continue;

	for(auto const& seed_id : remove_trk_s) {

	  auto const& seed = data.Track(seed_id);

	  // If the seed and the subject track are close in start point, ignore this seed
	  auto dist = shr->Start().Dist(seed[0]);
	  if(dist < _min_dist_common_origin) continue;

	  if(_use_perpdist_trk_shr) {
	    // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
	    auto dist = sqrt(_geoalg.SqDist(shr->Start(),seed));
	    if(dist > _min_dist_trk_shr) continue;

	  }else{
	    /*
	      If the distance between the closest approach mid-point and the subject trajectory is 
	      larger than the cut value, ignore
	    */
	    dist = _relalg.FindClosestApproach(*shr,seed,pt);
	    if(dist > _min_dist_trk_shr) continue;
	    
	  }

	  data.FilterShower(shr->ID());
	  break;
	}
      }

      _removed_trk_v.emplace_back(remove_trk_s);
    }

    _seed_trk_s.clear();
  }
}

#endif
