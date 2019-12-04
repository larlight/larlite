#ifndef LIGHTPATH_CXX
#define LIGHTPATH_CXX

#include "LightPath.h"
#include "GeoAlgo/GeoTrajectory.h"
namespace flashana {

  static LightPathFactory __global_LightPathFactory__;

  LightPath::LightPath(const std::string name)
    : BaseAlgorithm(kCustomAlgo, name)
    , _gap         ( 0.5    )
    , _light_yield ( 40000. )
    , _dEdxMIP     ( 2.07    ) //1.42[Mev*cm^2*g]*1.4[g/cm^3]=2.004MeV/cm
  {}

  void LightPath::_Configure_(const Config_t &pset)
  {
    _gap          = pset.get< double > ( "SegmentSize" );
    _light_yield  = pset.get< double > ( "LightYield"  );
    _dEdxMIP      = pset.get< double > ( "MIPdEdx"     );
  }

  void LightPath::QCluster(const larlite::geoalgo::Vector& pt_1,
                           const larlite::geoalgo::Vector& pt_2,
                           QCluster_t& Q_cluster,
			   double dedx) const {

    if(dedx < 0) dedx = _dEdxMIP;
    
    double dist = pt_1.Dist(pt_2);
    QPoint_t q_pt;

    if (dist <= _gap) {
      larlite::geoalgo::Vector mid_pt((pt_1 + pt_2) / 2.);
      q_pt.x = mid_pt[0];
      q_pt.y = mid_pt[1];
      q_pt.z = mid_pt[2];
      q_pt.q = dedx * _light_yield * dist;
      Q_cluster.emplace_back(q_pt);

      return;
    }

    int num_div = int(dist / _gap);

    larlite::geoalgo::Vector direct = (pt_1 - pt_2).Dir();

    Q_cluster.reserve(Q_cluster.size() + num_div);

    for (int div_index = 0; div_index < num_div + 1; div_index++) {
      if (div_index < num_div) {
        auto const mid_pt = pt_2 + direct * (_gap * div_index + _gap / 2.);
        q_pt.x = mid_pt[0] ;
        q_pt.y = mid_pt[1];
        q_pt.z = mid_pt[2];
        q_pt.q = _gap * dedx * _light_yield;
        Q_cluster.emplace_back(q_pt);
      }
      else {
        double weight = (dist - int(dist / _gap) * _gap);
        auto const mid_pt = pt_2 + direct * (_gap * div_index + weight / 2.);
        q_pt.x = mid_pt[0] ;
        q_pt.y = mid_pt[1];
        q_pt.z = mid_pt[2];
        q_pt.q = weight * dedx * _light_yield;
        Q_cluster.emplace_back(q_pt);
      }//Last segment less than gap
    }
  }

  QCluster_t LightPath::FlashHypothesis(const larlite::geoalgo::Trajectory& trj) const {

    QCluster_t result;
    result.clear();

    for (size_t i = 0; i < trj.size() - 1; i++) {
      auto const& this_loc(trj[i]);
      auto const& last_loc(trj[i + 1]);

      LightPath::QCluster(this_loc, last_loc, result);
    }

    return result;
  }

}


#endif
