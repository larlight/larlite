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

  // Loop over particles already tagged as Primary by upstream algorithms
  for (auto const& id : graph.GetPrimaryNodes()) {

    auto const& part = graph.GetParticle(id);

    if (part.ProcessType() != kCosmic) continue;
    // If the previously tagged Primary particle also has process "kCosmic",
    // Keep track of this primary particle's node ID in the "removed_id_v" vector of vector
    removed_id_v.back().push_back(id);
  }

  ::geoalgo::HalfLine_t short_trk;

  // While last vector stored in removed_id_v vector of vector has nonzero size,
  while (removed_id_v.back().size()) {
    // Add an empty vector of NodeIDs to the back of the removed_id_v vector
    removed_id_v.push_back(std::vector<NodeID_t>());

    // Grab the vector of NodeID's that is second-from-last in the removed_id_v vector,
    // and this vector of nodeIDs is called the "seed_v" vector
    auto const& seed_v = removed_id_v.at(removed_id_v.size() - 2) ; //back();

    // Loop over seed_v vector of NodeID's
    for (auto const& seed_id : seed_v) {

      // Grab the particle from NodeID in seed_v, and use it as a seed
      auto const& seed_part = graph.GetParticle(seed_id);

      // Only Tracks can be used as seeds
      if (seed_part.RecoType() != kTrack) continue;

      // Grab the track associated with this seed particle
      auto const& seed_trk = data.Track(seed_part.RecoID());

      // Loop over all of the particles in the particle graph (candidate secondaries)
      for (auto const& id : graph.GetParticleNodes()) {

        auto& part = graph.GetParticle(id);

        // If this candidate secondary is primary, descendant, or kCosmic, skip
        // because if it's primary, it definitely isn't a secondary, and if it
        // has already been reconstructed to be a descendant then we already know it's a secondary
        // and if it has already been reconstructed as a kCosmic, I don't care if it's primary or
        // secondary, I know it's a cosmic and that's enough.
        if ( part.Primary() ||
             part.Descendant() ||
             part.Parent() != id ||
             part.ProcessType() == kCosmic ) continue;

        // Start by assuming the candidate secondary is NOT a cosmic (innocent until proven guilty)
        bool is_cosmic = false;

        // If the candidate secondary is a Track
        if (part.RecoType() == kTrack) {
          // Grab the actual track associated with this particle
          auto const& cand_trk = data.Track(part.RecoID());

          // Inspect sibling-ness via common origin
          // (If distance between candidate secondary track start point and seed track
          // start point is less than some value, this candidate secondary is indeed a secondary)
          if (seed_trk[0].Dist(cand_trk[0]) < _min_dist_common_origin) {
            is_cosmic = true;
            // If the start points of candidate secondary and seed track are very close,
            // we don't need to use the IsCRSecondary function. Just set the candidate secondary
            // as the child of the seed.
            // If this were a neutrino interaction or something, they may both be primary and they
            // may be siblings, but upstream of here we know the seed track is tagged as kCosmic
            // so anything associated with this seed track is tagged as a child of the track.
            graph.SetParentage(seed_id, id);
          }
          // Case if the candidate secondary track only has one point, it's useless...
          // there isn't enough information to identify it as a cosmic, or a secondary,
          // so we continue assuming it's not a cosmic.
          else if (cand_trk.size() < 2) {
            is_cosmic = false;
          }
          // Case if candidate secondary track is longer than some minimum length
          else if (cand_trk.IsLonger(_min_dist_trk_def)) {
            // Use the IsCRSecondary function to determine if this candidate secondary track is indeed a secondary.
            is_cosmic = IsCRSecondary(seed_trk, cand_trk);
            // If the candidate secondary track was found to be a secondary, set it as the child of the seed track.
            if (is_cosmic) graph.SetParentage(seed_id, id);
          }
          // If the candidate secondary track doesn't have start point close to the seed track,
          // and it has more than one point in its trajectory
          // and it isn't long enough to be considered a legitimate track for the IsCRSecondary fucntion to analyze,
          // treat the track as if it is a shower!
          else {
            // Build an artificial ERTool::Shower (well, a GeoHalfLine to be precise) from this candidate secondary track
            auto const& start = cand_trk.front();
            auto const& dir   = cand_trk.Dir(0);
            short_trk.Start(start[0], start[1], start[2]);
            short_trk.Dir(dir[0], dir[1], dir[2]);
            // Use the IsCRSecondary function that uses a seed track and a GeoHalfLine (shower-like) to determine
            // if this candidate secondary is indeed a secondary.
            is_cosmic = IsCRSecondary(seed_trk, short_trk);
            // If this candidate secondary was found to be a secondary, set it as a child of the seed track.
            if (is_cosmic) graph.SetParentage(seed_id, id);
          }
        } // end if the candidate secondary is a Track

        // If the candidate secondary is a Shower
        else if (part.RecoType() == kShower) {
          // Grab the actual shower associated with this particle
          auto const& cand_shr = data.Shower(part.RecoID());

          // If the distance between the seed track start point and the candidate secondary (shower) start
          // point is less than some value, the candidate secondary is a cosmic.
          if (seed_trk[0].Dist(cand_shr.Start()) < _min_dist_common_origin) {
            is_cosmic = true;
            // If the start points of candidate secondary and seed track are very close,
            // we don't need to use the IsCRSecondary function. Just set the candidate secondary
            // as the child of the seed.
            // If this were a neutrino interaction or something, they may both be primary and they
            // may be siblings, but upstream of here we know the seed track is tagged as kCosmic
            // so anything associated with this seed track is tagged as a child of the track.
            graph.SetParentage(seed_id, id);
          }
          // If the distance between the seed track start point and the candidate secondary (shower) start
          // point is greater than some value, we use the IsCRSecondary function to determine if the candidate
          // secondary (shower) is indeed a secondary. If so, set it as the child of the seed track.
          else {
            is_cosmic = IsCRSecondary(seed_trk, cand_shr);
            if (is_cosmic) graph.SetParentage(seed_id, id);
          }
        } // end if the candidate secondary is a Shower

        if (is_cosmic) {
          // If we've decided this candidate secondary is a cosmic,
          // Set its particle info (is this doing anything?)
          part.SetParticleInfo(part.PdgCode(),
                               part.Mass(),
                               part.Vertex(),
                               part.Momentum(),
                               part.RecoScore());
          // push_back this particle's node ID to the last vector of removed_id_v vector.
          removed_id_v.back().push_back(id);
        }
      } // end loop over all particles in the graph (candidate secondaries)
    } // end loop over seed_v vector of NodeID's

    // If you've looped over all particles and checked them against all seeds and found no more cosmics,
    // the while loop will break now.
    // Otherwise, it will continue interating until you have identified no new cosmic particles.

  } // end while last vector stored in removed_id_v vector of vector has nonzero size
  return true;
}

/// Function to take in a seed track and a candidate secondary track and determine if the
/// candidate secondary is indeed a secondary, using AlgoFindRelationship.
bool ERAlgoCRSecondary::IsCRSecondary(const ::geoalgo::Trajectory_t seed,
                                      const ::geoalgo::Trajectory_t cand) const
{

  ::geoalgo::GeoAlgo geo_alg;
  AlgoFindRelationship rel_alg;

  // If user wants to use perpendicular track-to-track comparison
  if (_use_perpdist_trk_trk) {

    // If the distance between the candidate secondary track's start point and the seed track's
    // start point is larger than some cutoff value, the candidate secondary is NOT a secondary.
    if (cand.front().Dist(seed.front()) > _min_dist_trk_trk) return false;

    // If the length of the candidate secondary's track is greater than the minimum allowed
    // distance between two tracks, the candidate secondary is NOT a secondary.
    // (the candidate secondary isn't close enough to the seed track to be assumed associated with it)
    if ( cand.Length() > _min_dist_trk_trk ) return false;

    // If perpendicular distance between the start point of the candidate secondary track
    // and the entire seed track is bigger than the cut value,
    // the candidate secondary is NOT a secondary.
    if ( sqrt(geo_alg.SqDist(cand.front(), seed)) > _min_dist_trk_trk) return false;

  }

  // If user doesn't want to use perpendicular track-to-track comparison
  else {
    // Container to hold point of closest approach (CPA) between the candidate secondary track and the seed track
    ::geoalgo::Point_t pt;

    // Compute the CPA between the candidate secondary track and the seed track (fill the pt container)
    // and compute the distance between the CPA and the seed trajectory (dist) [using AlgoFindRelationship]
    auto dist = rel_alg.FindClosestApproach(cand, seed, pt);

    // If dist is larger than the cut value, this candidate secondary track is NOT a secondary.
    if (dist > _min_dist_trk_trk) return false;

    // Compute the distance between the CPA and the candidate secondary track's start point.
    // (overwrite the "dist" variable)
    dist = cand[0].Dist(pt);

    // If the dist between CPA and candidate secondary track's start point is greater than some cutoff,
    // the candidate secondary track is NOT a secondary
    if (dist > _min_dist_common_origin) return false;

  }

  // If the candidate secondary hasn't been proved NON secondary (IE we haven't returned false yet), it is a secondary.
  return true;
}

bool ERAlgoCRSecondary::IsCRSecondary(const ::geoalgo::Trajectory_t seed,
                                      const ::geoalgo::HalfLine_t   cand) const
{

  ::geoalgo::GeoAlgo geo_alg;
  AlgoFindRelationship rel_alg;

  // If user wants to use perpendicular track-to-shower comparison
  if (_use_perpdist_trk_shr) {

    // If perpendicular distance between the start point of the candidate secondary shower
    // and the entire seed track is bigger than the cut value,
    // the candidate secondary is NOT a secondary.
    if ( sqrt(geo_alg.SqDist(cand.Start(), seed)) > _min_dist_trk_shr) return false;

  }
  // If user doesn't want to use perpendicular track-to-shower comparison
  else {

    // If the distance between the closest approach mid-point and the subject trajectory is
    // larger than the cut value, ignore

    ::geoalgo::Point_t pt;
    auto dist = rel_alg.FindClosestApproach(cand, seed, pt);

    if (dist > _min_dist_trk_shr) return false;

  }
  return true;
}

void ERAlgoCRSecondary::ProcessEnd(TFile* fout)
{}

}

#endif
