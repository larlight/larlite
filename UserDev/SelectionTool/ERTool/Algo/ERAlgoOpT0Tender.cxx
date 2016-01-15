#ifndef ERTOOL_ERALGOOPT0TENDER_CXX
#define ERTOOL_ERALGOOPT0TENDER_CXX

#include "ERAlgoOpT0Tender.h"

namespace ertool {

  ERAlgoOpT0Tender::ERAlgoOpT0Tender(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoOpT0Tender::Reset()
  {}

  void ERAlgoOpT0Tender::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoOpT0Tender::ProcessBegin()
  {}

  bool ERAlgoOpT0Tender::Reconstruct(const EventData &data, ParticleGraph& graph)
  {return true;}

  void ERAlgoOpT0Tender::ProcessEnd(TFile* fout)
  {}

  ::flashana::QCluster_t ERAlgoOpT0Tender::GetQCluster(const EventData& data,
						       const std::vector<RecoID_t>& shower_id_v,
						       const std::vector<RecoID_t>& track_id_v) const
  {
    ::flashana::QCluster_t res;
    ::flashana::QCluster_t res_tot;
    res_tot.clear();
    for(auto const& id : shower_id_v) {
      
      auto const& shower = data.Shower(id);

      if(shower.Length()<0.1) continue;

      AppendGeoSteps(res,shower.Start(),shower.Dir(),shower.Length());

    }

    for(auto const& id : track_id_v) {

      auto const& track = data.Track(id);

      if(track.size()<2) continue;

      if(!track.IsLonger(0.1)) continue;

      res.reserve(track.size());
      res = LP.FlashHypothesis(track);
      res_tot += res; 
    }

    return res_tot;
  }

  void ERAlgoOpT0Tender::AppendGeoSteps(::flashana::QCluster_t& cluster,
				  const ::geoalgo::Point_t& start,
				  const ::geoalgo::Vector_t& dir,
				  const double length) const
  {

    if(length < _step_size) {

      ::flashana::QPoint_t pt;
      pt.x = start[0] + dir[0] * length/2.;
      pt.y = start[1] + dir[1] * length/2.;
      pt.z = start[2] + dir[2] * length/2.;
      pt.q = length;
      cluster.emplace_back(pt);
      return;
    }

    size_t nsteps = size_t(length / _step_size);
    cluster.reserve(cluster.size() + nsteps+1);

    for(size_t i=0; i<nsteps; ++i) {
      ::flashana::QPoint_t pt;
      pt.x = start[0] + dir[0] * (_step_size * i + _step_size/2.);
      pt.y = start[1] + dir[1] * (_step_size * i + _step_size/2.);
      pt.z = start[2] + dir[2] * (_step_size * i + _step_size/2.);
      pt.q = _step_size;
      cluster.emplace_back(pt);
    }

    ::flashana::QPoint_t pt;
    double last_length = length - _step_size * nsteps;
    pt.x = start[0] + dir[0] * (length - last_length/2.);
    pt.y = start[1] + dir[1] * (length - last_length/2.);
    pt.z = start[2] + dir[2] * (length - last_length/2.);
    pt.q = last_length;
    cluster.emplace_back(pt);
  }
  
}

#endif
