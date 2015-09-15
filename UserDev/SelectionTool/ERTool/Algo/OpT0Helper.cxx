#ifndef ERTOOL_OPT0HELPER_CXX
#define ERTOOL_OPT0HELPER_CXX

#include "OpT0Helper.h"

namespace ertool {

  OpT0Helper::OpT0Helper(const std::string name)
    : _name(name)
    , _use_energy(false)
    , _step_size(2)
  {}
  
  void OpT0Helper::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    auto p = cfg.get_pset(Name());
    _step_size  = p.get<double>("StepSize");
    _use_energy = p.get<bool>("UseEnergy");
  }
  
  ::flashana::QCluster_t OpT0Helper::GetQCluster(const EventData& data,
						 const std::vector<RecoID_t>& shower_id_v,
						 const std::vector<RecoID_t>& track_id_v) const
  {
    ::flashana::QCluster_t res;
    for(auto const& id : shower_id_v) {

      auto const& shower = data.Shower(id);

      AppendGeoSteps(res,shower.Start(),shower.Dir(),shower.Length());

    }

    for(auto const& id : track_id_v) {

      auto const& track = data.Track(id);

      if(track.size()<2) continue;

      res.reserve(track.size());
      for(size_t i=0; i<(track.size()-1); ++i) {

	auto const& pt1 = track[i];
	auto const& pt2 = track[i+1];
	auto dir = pt2 - pt1;
	double length = dir.Length();
	dir /= length;
	AppendGeoSteps(res,pt1,dir,length);
      }
      
    }

    return res;
  }

  void OpT0Helper::AppendGeoSteps(::flashana::QCluster_t& cluster,
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
