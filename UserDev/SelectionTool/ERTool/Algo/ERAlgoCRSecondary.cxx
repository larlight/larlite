#ifndef ERTOOL_ERALGOCRSECONDARY_CXX
#define ERTOOL_ERALGOCRSECONDARY_CXX

#include "ERAlgoCRSecondary.h"
#include "GeoAlgo/GeoAlgo.h"
#include "AlgoFindRelationship.h"
namespace ertool {

  ERAlgoCRSecondary::ERAlgoCRSecondary(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoCRSecondary::Reset()
  {}

  void ERAlgoCRSecondary::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    auto p = cfg.get_pset(Name());
    _min_dist_trk_trk       = p.get<double>("min_dist_track_2_track");
    _min_dist_common_origin = p.get<double>("min_dist_common_origin");
    _min_dist_trk_def       = p.get<double>("min_dist_track_to_be"  );
    _use_perpdist_trk_trk   = p.get<bool>  ("use_perp_dist_track_2_track");
    _min_dist_trk_shr       = p.get<double>("min_dist_track_2_shower");
    _use_perpdist_trk_shr   = p.get<bool>  ("use_perp_dist_track_2_shower");
  }

  void ERAlgoCRSecondary::ProcessBegin()
  {}

  bool ERAlgoCRSecondary::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

    std::vector<std::vector<NodeID_t> > removed_id_v(1);

    for(auto const& id : graph.GetPrimaryNodes()) {

      auto const& part = graph.GetParticle(id);

      if(part.ProcessType() != kCosmic) continue;

      removed_id_v.back().push_back(id);
    }

    ::geoalgo::HalfLine_t short_trk;
    
    while(removed_id_v.back().size()) {

      removed_id_v.push_back(std::vector<NodeID_t>());
      auto const& seed_v = removed_id_v.at(removed_id_v.size()-2) ; //back();

      // Loop over seed (reference from which secondaries come from)
      for(auto const& seed_id : seed_v) {

	auto const& seed_part = graph.GetParticle(seed_id);

	if(seed_part.RecoType() != kTrack) continue;

	auto const& seed_trk = data.Track(seed_part.RecoID());

	// Loop over particles as candidate secondaries
	for(auto const& id : graph.GetParticleNodes()) {

	  auto& part = graph.GetParticle(id);
	  
	  // If primary or parent already exists, don't worry
	  if( part.Primary() ||
	      part.Descendant() ||
	      part.Parent() != id ||
	      part.ProcessType() == kCosmic ) continue;
	  
	  bool is_cosmic=false;
	  // Track
	  if(part.RecoType() == kTrack) {
	    auto const& cand_trk = data.Track(part.RecoID());
	    // Inspect sibling-ness via common origin
	    if(seed_trk[0].Dist(cand_trk[0]) < _min_dist_common_origin) {
	      is_cosmic = true;
	      if(seed_part.Primary()) graph.SetPrimary(id);
	      else graph.SetSiblings(seed_id,id);
	    }
	    // Else if track is a point, nothing we can do
	    else if(cand_trk.size()<2) {
	      is_cosmic = false;
	    }
	    // If track is long enough, treat as track
	    else if(cand_trk.IsLonger(_min_dist_trk_def)) {
	      is_cosmic = IsCRSecondary(seed_trk,cand_trk);
	      if(is_cosmic) graph.SetParentage(seed_id,id);
	    }
	    // Else treat as if a shower
	    else{
	      auto const& start = cand_trk.front();
	      auto const& dir   = cand_trk.Dir(0);
	      short_trk.Start(start[0], start[1], start[2]);
	      short_trk.Dir(dir[0],dir[1],dir[2]);
	      is_cosmic = IsCRSecondary(seed_trk,short_trk);
	      if(is_cosmic) graph.SetParentage(seed_id,id);
	    }
	  }
	  // Shower
	  else if(part.RecoType() == kShower) {
	    auto const& cand_shr = data.Shower(part.RecoID());
	    // Inspect sibling-ness via common origin
	    if(seed_trk[0].Dist(cand_shr.Start()) < _min_dist_common_origin) {
	      is_cosmic = true;
	      if(seed_part.Primary()) graph.SetPrimary(id);
	      else graph.SetSiblings(seed_id,id);
	    }
	    // Treat as a shower
	    else {
	      is_cosmic = IsCRSecondary(seed_trk,cand_shr);
	      if(is_cosmic) graph.SetParentage(seed_id,id);
	    }
	  }
	  if(is_cosmic) {
	    part.SetParticleInfo(part.PdgCode(),
				 part.Mass(),
				 part.Vertex(),
				 part.Momentum(),
				 part.RecoScore());
	    removed_id_v.back().push_back(id);
	  }
	} // loop over particles ends
      } // loop over seed ends
    } // iterative approach to find secondary ends
    return true;
  }

  bool ERAlgoCRSecondary::IsCRSecondary(const ::geoalgo::Trajectory_t seed,
					const ::geoalgo::Trajectory_t cand) const
  {
    ::geoalgo::GeoAlgo geo_alg;
    AlgoFindRelationship rel_alg;
    if(_use_perpdist_trk_trk) {
      // If cand.front() -> seed.front() > _min_dist_trk_trk
      // and if cand.length() > _min_dist_trk_trk
      // then no hope of the two being close enough -> return false
      auto d1 = cand.front().Dist(seed.front());
      if (d1 > _min_dist_trk_trk) return false;
      if ( cand.Length() > _min_dist_trk_trk ) return false;
      // If perpendicular distance of the "start" to "track" is bigger
      // than the cut value, not secondary
      auto dist = sqrt(geo_alg.SqDist(cand.front(),seed));

      if ( dist > _min_dist_trk_trk) return false;

    }else{
      ::geoalgo::Point_t pt;
      /*
	If the distance between the closest point of approach (CPA) and the subject trajectory is 
	larger than the cut value, ignore
      */
      auto dist = rel_alg.FindClosestApproach(cand,seed,pt);
      if(dist > _min_dist_trk_trk) return false;
      
      /*
	Check if the distance between the CPA and the subject track's start point. This has to be in
	the common origin radius
      */
      dist = cand[0].Dist(pt);
      if(dist > _min_dist_common_origin) return false;
    }
    return true;
  }

  bool ERAlgoCRSecondary::IsCRSecondary(const ::geoalgo::Trajectory_t seed,
					const ::geoalgo::HalfLine_t   cand) const
  {
    ::geoalgo::GeoAlgo geo_alg;
    AlgoFindRelationship rel_alg;
    if(_use_perpdist_trk_shr) {
      // If perpendicular distance of the "start" to "track" is bigger than the cut value, ignore this seed
      auto dist = sqrt(geo_alg.SqDist(cand.Start(),seed));

      if ( dist > _min_dist_trk_shr) return false;
      
    }else{
      /*
	If the distance between the closest approach mid-point and the subject trajectory is 
	larger than the cut value, ignore
      */
      ::geoalgo::Point_t pt;
      auto dist = rel_alg.FindClosestApproach(cand,seed,pt);

      if(dist > _min_dist_trk_shr) return false;
      
    }
    return true;
  }

  void ERAlgoCRSecondary::ProcessEnd(TFile* fout)
  {}

}

#endif
