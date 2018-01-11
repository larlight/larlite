#ifndef CLUSTER_CXX
#define CLUSTER_CXX

#include "Cluster.h"

#include <cmath>

namespace cluster {

  void Cluster::Clear() {

    _sum_charge = 0;
    _plane      = 4;

    _hits.clear();

  }

  int Cluster::SetHits(const std::vector<cluster::pt>& hits) {
    
    Clear();

    _hits = hits;

    _plane = hits.at(0)._pl;

    _angle = 0.;

    float angle_min =  360.;
    float angle_max = -360.;
    float angle_avg = 0.;
    float rad_min   = 1000.;
    float rad_max   = 0.;

    // prepare vectors for linearity calculation
    std::vector<double> hit_w_v;
    std::vector<double> hit_t_v;

    // if hits are present in 1st and 4th quadrants (left-hand side)
    // then span calculation will be incorrect.
    // correct for this.
    auto quadrantmixup = QuadMixup(_hits);

    for (auto const& hit: _hits) {

      auto angle = hit._a;

      if (quadrantmixup == true) {
	if (angle > 270.) angle -= 270;
	else angle += 90.;
      }
	
      _sum_charge += hit._q;
      _angle += angle * hit._q;
      angle_avg += angle;
      
      if (hit._r < rad_min) {
	rad_min = hit._r;
	_start_pt = hit;
      }

      if (hit._r > rad_max) {
	rad_max = hit._r;
	_end_pt = hit;
      }

      if (angle > angle_max) angle_max = angle;
      if (angle < angle_min) angle_min = angle;

      hit_w_v.push_back( hit._w );
      hit_t_v.push_back( hit._t );
      
    }// for all hits in cluster

    _lin = ::twodimtools::Linearity(hit_w_v,hit_t_v);
    
    _angle /= _sum_charge;

    _angle_span = ::cluster::anglespan(angle_min,angle_max);

    angle_avg /= _hits.size();
    _angle_rms = 0.;
    
    for (auto const& h : _hits)
      _angle_rms += (h._a - angle_avg) * (h._a - angle_avg);
    _angle_rms = sqrt( _angle_rms / (_hits.size() - 1) );
    
    
    if (quadrantmixup) {

      if (_angle > 90.) _angle -= 90.;
      else _angle += 270;

      if (_angle_span._amin > 90.) _angle_span._amin -= 90.;
      else _angle_span._amin += 270.;

      if (_angle_span._amax > 90.) _angle_span._amax -= 90.;
      else _angle_span._amax += 270.;

    }
    
    return _hits.size();
  }

  bool Cluster::QuadMixup(const std::vector<::cluster::pt>& pt_v) {
    
    bool quadrantmixup = false;
    bool quad1 = false;
    bool quad4 = false;
    for (auto const& hit: _hits) {
      
      if (hit._a > 270.) quad4 = true;
      if (hit._a < 90.)  quad1 = true;
    }
    
    if (quad1 && quad4) quadrantmixup = true;

    return quadrantmixup;
  }
  
}

#endif
