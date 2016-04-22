#ifndef ERTOOL_ERALGOCRORPHAN_CXX
#define ERTOOL_ERALGOCRORPHAN_CXX

#include "ERAlgoCROrphan.h"

namespace ertool {

  ERAlgoCROrphan::ERAlgoCROrphan(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoCROrphan::Reset()
  {}

  void ERAlgoCROrphan::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    auto p = cfg.get_pset(Name());
    auto xrange_v = p.get<std::vector<double> >("tpc_aabox_xrange");
    auto yrange_v = p.get<std::vector<double> >("tpc_aabox_yrange");
    auto zrange_v = p.get<std::vector<double> >("tpc_aabox_zrange");
    _tpc_box.Min( xrange_v[0], yrange_v[0], zrange_v[0] );
    _tpc_box.Max( xrange_v[1], yrange_v[1], zrange_v[1] );

    _respect_trk_dir           = p.get<bool  >( "respect_track_dir"     );
    _min_dist_trk_def          = p.get<double>( "min_dist_track_to_be"  );
    _dist_back_to_ceiling      = p.get<double>( "dist_back_to_ceiling"  );
    _tag_protons_from_neutrons = p.get<bool  >( "tag_protons_from_neutrons" );
  }

  void ERAlgoCROrphan::ProcessBegin()
  {}

  bool ERAlgoCROrphan::Reconstruct(const EventData &data, ParticleGraph& graph)
  {


    ::geoalgo::HalfLine shower;

    /// Loop over all particles in the reco particle graph
    for (auto const& id : graph.GetParticleNodes()) {

      auto& part = graph.GetParticle(id);

      /// If the particle is already tagged as cosmic or as decendant of another, skip it.
      if ( part.ProcessType() == kCosmic || part.Descendant() )
        continue;

      /// First look for possibly-neutron-induced short proton tracks, if the cfg file says to
      if ( _tag_protons_from_neutrons ) {
        int possible_neutron_parent_nodeid = IsNeutronInduced( part, data, graph );
        // if nodeID -1 is returned, that means this particle wasn't neutron-induced
        if ( possible_neutron_parent_nodeid != -1 ) {
          ///tag the particle as orphan
          part.SetParticleInfo( part.PdgCode(),
                                part.Mass(),
                                part.Vertex(),
                                part.Momentum(),
                                part.RecoScore(),
                                kCosmic);
          /// (don't) set the particle as primary
          // graph.SetPrimary(id);
          /// (don't) set the parent to the nearest cosmic primary,
          // graph.SetParentage(possible_neutron_parent_nodeid, id);

          //and skip the next step (continue to the next particle in the reco particle graph)
          continue;
        }
      }

      /// Now look for orphans
      bool is_cosmic = false;
      if ( part.RecoType() == kTrack ) {

        auto const& trk = data.Track(part.RecoID());
        if (trk.size() < 2 || trk.IsLonger(_min_dist_trk_def)) continue;

        auto const& pt1 = trk[0];
        auto const& pt2 = trk[1];
        auto dir = pt2 - pt1;
        shower.Start(pt1[0], pt1[1], pt1[2]);
        shower.Dir(dir);

        is_cosmic = IsCROrphan(shower);

        if (!is_cosmic && !_respect_trk_dir) {

          auto const& rev_pt1 = trk[trk.size() - 1];
          auto const& rev_pt2 = trk[trk.size() - 2];
          auto rev_dir = rev_pt2 - rev_pt1;
          shower.Start(rev_pt1[0], rev_pt1[1], rev_pt1[2]);
          shower.Dir(rev_dir[0], rev_dir[1], rev_dir[2]);

          is_cosmic = IsCROrphan(shower);
        }

      } else if ( part.RecoType() == kShower)

        is_cosmic = IsCROrphan(data.Shower(part.RecoID()));

      if (is_cosmic) {

        graph.SetPrimary(id);

        part.SetParticleInfo( part.PdgCode(),
                              part.Mass(),
                              part.Vertex(),
                              part.Momentum(),
                              part.RecoScore(),
                              kCosmic);
      }
    }
    return true;
  }

  bool ERAlgoCROrphan::IsCROrphan(const ::geoalgo::HalfLine& shr) const
  {

    auto const& start = shr.Start();
    auto const& dir   = shr.Dir();

    if (dir[1] > 0) return false;

    if (!_tpc_box.Contain(start)) return true;

    double dist = _tpc_box.Max()[1] - start[1];

    dist = dist / dir[1] * -1.;

    //std::cout<<"Dist is  : "<<dist<<std::endl ;

    if (dist > _dist_back_to_ceiling) return false;

    return true;
  }

  int ERAlgoCROrphan::IsNeutronInduced(const ertool::Particle& p, const EventData &data, const ParticleGraph& ps) const
  {

    // For now only tracks are neutron-induced, in this function
    if ( p.RecoType() != RecoType_t::kTrack ) return -1;

    // If the track is too long, it's not neutron-induced (for now I'm looking for short proton tracks)
    if ( data.Track(p).Length() > _max_neutron_trklength ) return -1;

    // Compute the shortest distance between this track and all tagged primary cosmic tracks
    double _min_dist = 999999.;
    double _tmp_min_dist = 999999.;

    for ( auto const &pnodeid2 : ps.GetPrimaryNodes() ) {

      ertool::Particle p2 = ps.GetParticle(pnodeid2);

      if (p2.ProcessType() != ProcessType_t::kCosmic) continue;

      // Don't compare yourself to yourself
      if (p2.ID() == p.ID()) continue;

      if (p.RecoType() == RecoType_t::kTrack && p2.RecoType() == RecoType_t::kTrack)
        _tmp_min_dist =
          _findRel.FindClosestApproach(data.Track(p), data.Track(p2));
      else if (p.RecoType() == RecoType_t::kTrack && p2.RecoType() == RecoType_t::kShower)
        _tmp_min_dist =
          _findRel.FindClosestApproach(data.Track(p), data.Shower(p2));
      else std::cout << "wtf" << std::endl;

      if (_tmp_min_dist < _min_dist)
        _min_dist = _tmp_min_dist;

    }// End inner loop over particles to compare all permutations

    // If the track is too close to any other particles, it's not neutron-induced
    if ( _min_dist > _min_neutron_dist ) return -1;

    // If the track wasn't found to be non-neutron induced... then it is neutron-induced!
    return p.ID();
  }

  void ERAlgoCROrphan::ProcessEnd(TFile* fout)
  {}

}

#endif
