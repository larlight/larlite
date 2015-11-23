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

  _respect_trk_dir      = p.get<bool  >( "respect_track_dir"     );
  _min_dist_trk_def     = p.get<double>( "min_dist_track_to_be"  );
  _dist_back_to_ceiling = p.get<double>( "dist_back_to_ceiling"  );
}

void ERAlgoCROrphan::ProcessBegin()
{}

bool ERAlgoCROrphan::Reconstruct(const EventData &data, ParticleGraph& graph)
{

  ::geoalgo::HalfLine shower;

  for (auto const& id : graph.GetParticleNodes()) {

    auto& part = graph.GetParticle(id);

    if ( part.ProcessType() == kCosmic || part.Descendant() )
      continue;

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
                            kCosmicOrphan);
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

void ERAlgoCROrphan::ProcessEnd(TFile* fout)
{}

}

#endif
