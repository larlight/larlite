#ifndef ERTOOL_ERALGOCRPRIMARY_CXX
#define ERTOOL_ERALGOCRPRIMARY_CXX

#include "ERAlgoCRPrimary.h"

namespace ertool {

ERAlgoCRPrimary::ERAlgoCRPrimary(const std::string& name) : AlgoBase(name)
{}

void ERAlgoCRPrimary::Reset()
{}

void ERAlgoCRPrimary::AcceptPSet(const ::fcllite::PSet& cfg)
{
  auto p = cfg.get_pset(Name());

  /// Default values for TPC box are x: [0.3, 256], y: [-115,115], z: [0.5,1036] (roughly the size of the TPC)
  auto tpc_x = p.get<std::vector<double> >("tpc_aabox_xrange");
  auto tpc_y = p.get<std::vector<double> >("tpc_aabox_yrange");
  auto tpc_z = p.get<std::vector<double> >("tpc_aabox_zrange");
  _tpc_box.Min( tpc_x[0], tpc_y[0], tpc_z[0] );
  _tpc_box.Max( tpc_x[1], tpc_y[1], tpc_z[1] );

  /// Default values for the cosmic box are x: [-1.5,258], y: [-119,118], z: [-1.5,1038] (slightly larger than TPC)
  auto cr_x = p.get<std::vector<double> >("cosmic_aabox_xrange");
  auto cr_y = p.get<std::vector<double> >("cosmic_aabox_yrange");
  auto cr_z = p.get<std::vector<double> >("cosmic_aabox_zrange");
  _cosmic_box.Min( cr_x[0], cr_y[0], cr_z[0] );
  _cosmic_box.Max( cr_x[1], cr_y[1], cr_z[1] );

  _respect_track_dir = p.get<bool>("respect_track_dir");
  _tag_entering_or_exiting_as_cosmics = p.get<bool>("tag_entering_or_exiting_as_cosmics");
}

void ERAlgoCRPrimary::ProcessBegin()
{}

bool ERAlgoCRPrimary::Reconstruct(const EventData &data, ParticleGraph& graph)
{

  // Loop over all particles that come from tracks (only these can be tagged as primary cosmics)
  for (auto const& id : graph.GetParticleNodes(kTrack)) {

    // Grab the particle
    auto& part = graph.GetParticle(id); 

    // If somehow the particle has already been reconstructed to be the descendant of another particle,
    // it cannot be a primary.
    if (part.Descendant()) continue;

    // Grab the track that is associated with this particle
    auto const& trk  = data.Track(part.RecoID()); // Get track

    // If the track is less than 3mm long, it will not be tagged as a primary cosmic track.
    if (trk.Length() < 0.3) continue;

    auto const& start = trk.front(); // First step of the track (start point)
    auto const& end   = trk.back();  // Last step of the track (end point)

    // Start by assuming this particle is not a cosmic particle (innocent until proven guilty)
    bool is_cosmic = false;

    // Tag method 1: if either start/end points live outside CR box boundary, track track is a cosmic
    // Note CR box is slightly larger than TPC box
    if ( !_cosmic_box.Contain(start) || !_cosmic_box.Contain(end))
      is_cosmic = true;

    // If either start or end point is inside of the CR box boundary
    else {
      // See if the start and/or end points are contained within the TPC (slightly smaller than CR box)
      bool   start_contained = _tpc_box.Contain(start);
      bool   end_contained   = _tpc_box.Contain(end);

      // Tag method 1: if both ends are outside TPC (but, one of the two is inside the slightly larger CR box)
      // then this track is tagged as a primary cosmic
      if ( (!start_contained && !end_contained) ||  // Both ends are outside TPC, both are inside of CR box
           (_respect_track_dir && !start_contained) || // Check only start if told to respect dir
           (_tag_entering_or_exiting_as_cosmics && (!start_contained || !end_contained) ) // if we should remove anything that enters or exits though the TPC, check if either the start or end point pass a boundary
         )
        is_cosmic = true;
    }

    if (is_cosmic) {
      part.SetParticleInfo(part.PdgCode(),
                           part.Mass(),
                           part.Vertex(),
                           part.Momentum(),
                           part.RecoScore(),
                           kCosmic);
      graph.SetPrimary(id);
    }
  }
  return true;
}

void ERAlgoCRPrimary::ProcessEnd(TFile* fout)
{}

}

#endif
