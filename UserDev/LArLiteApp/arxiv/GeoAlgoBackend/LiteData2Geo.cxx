#ifndef LITEDATA2GEO_CXX
#define LITEDATA2GEO_CXX

#include "LiteData2Geo.h"

namespace larlite {

  ::geoalgo::Trajectory_t LiteData2Geo::GeoObj(const mctrack& trk) const
  {
    ::geoalgo::Trajectory_t res;
    for(auto const& pt : trk) {
      res += ::geoalgo::Point_t(pt.Position());
      std::cout<<pt.X()<<" : "<<pt.Y()<<" : "<<pt.Z()<<std::endl;
    }
    return res;
  }
  ::geoalgo::HalfLine_t LiteData2Geo::GeoObj(const mcshower& shw) const
  {
    ::geoalgo::HalfLine_t res(::geoalgo::Point_t(shw.DetProfile().Position()), 
			      ::geoalgo::Point_t(shw.DetProfile().Momentum()));
    return res;
  }
  
  std::vector<geoalgo::HalfLine_t> LiteData2Geo::GeoObj(const std::vector<larlite::mcshower>& shw_v) const
  {
    std::vector<geoalgo::HalfLine_t> res;
    res.reserve(shw_v.size());
    for(auto const& shw : shw_v) res.push_back(LiteData2Geo::GeoObj(shw));
    return res;
  }

  std::vector<geoalgo::Trajectory_t> LiteData2Geo::GeoObj(const std::vector<larlite::mctrack>& trk_v) const
  {
    std::vector<geoalgo::Trajectory_t> res;
    res.reserve(trk_v.size());
    for(auto const& trk : trk_v) {
      if(res.size()>=2)
	res.push_back(GeoObj(trk));
    }
    return res;
  }  

}

#endif
