#ifndef ERTOOL_ERALGOCCSINGLEMU_CXX
#define ERTOOL_ERALGOCCSINGLEMU_CXX

#include "ERAlgoCCSingleMu.h"
#include <set>

namespace ertool {

  ERAlgoCCSingleMu::ERAlgoCCSingleMu() : AlgoBase()
  {
    _name     = "ERAlgoCCSingleMu";
    _min_trk_length = 5;
    _min_dist_common_origin = 1;    
  }

  void ERAlgoCCSingleMu::Reset()
  {

  }

  ParticleSet ERAlgoCCSingleMu::Reconstruct(const EventData &data)
  { 

    std::set<size_t> non_primary_s;

    ::geoalgo::Point_t pt(3,0);

    //auto const& candidate_track_v = data.Track();

    // Search for secondary tracks to be rejected
    if(data.Track().size()>1) {
      for(auto const& pair : data.TrackCombination(2)) {
	
	auto const& trk_ptr_a = pair[0];
	auto const& trk_ptr_b = pair[1];
	
	auto const& start_a = trk_ptr_a->front();
	auto const& start_b = trk_ptr_b->front();
	
	// If 2 tracks come from the common origin, ignore this combination
	if(start_a.Dist(start_b) < _min_dist_common_origin)
	  continue;
	
	auto dist = _relalg.FindClosestApproach(*trk_ptr_a,*trk_ptr_b,pt);
	if(dist < 0 || dist > _min_dist_common_origin) continue;
	
	// Is A from B?
	if( trk_ptr_b->Length() > _min_trk_length &&
	    pt.Dist(start_a) < _min_dist_common_origin )
	  non_primary_s.insert(trk_ptr_a->ID());
	
	// Is B from A?
	if( trk_ptr_a->Length() > _min_trk_length &&
	    pt.Dist(start_b) < _min_dist_common_origin )
	  non_primary_s.insert(trk_ptr_b->ID());
      }
    }

    // Loop over primary tracks and inspect if it's a muon or not
    ParticleSet result;
    for(auto const& trk_ptr : data.Track()) {

      if(non_primary_s.find(trk_ptr->ID()) != non_primary_s.end())
	continue;

      Particle p(trk_ptr->_pid,0);
      p.Vertex(trk_ptr->front());
      p.RecoObjInfo(trk_ptr->ID(),Particle::RecoObjType_t::kTrack);
      result.push_back(p);
    }
    return result;
  }

}

#endif
