#ifndef SHOWERRECOUTILS_CXX
#define SHOWERRECOUTILS_CXX

#include "ShowerRecoUtils.h"

namespace showerreco {

  ShowerCluster_t cpan_2_scluster(const ::cluster::ClusterParamsAlg& cpan)
  {
    ShowerCluster_t res;
    
    res.start_point    = cpan.GetParams().start_point;
    res.end_point      = cpan.GetParams().end_point;
    res.angle_2d       = cpan.GetParams().angle_2d;
    res.opening_angle  = cpan.GetParams().opening_angle;

    ::larlite::geo::PlaneID id;
    id.Plane = cpan.Plane();
    res.plane_id       = id;
    res.hit_vector     = cpan.GetHitVector();

    return res;
  }
  
  ShowerClusterSet_t cpan_2_scluster(const std::vector< ::cluster::ClusterParamsAlg>& cpan_v)
  {
    ShowerClusterSet_t res;
    for(auto const& cpan : cpan_v)

      res.emplace_back(cpan_2_scluster(cpan));

    return res;
  }

}
#endif
