#ifndef DQDXMODULE_CXX
#define DQDXMODULE_CXX

#include "dQdxModule.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/LArProperties.h"
#include "LArUtil/DetectorProperties.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "math.h"
#include <algorithm>
#include <functional>

bool larger( std::pair<int, double> a, std::pair<int, double> b) {
  return a.second < b.second;
}

namespace showerreco {

dQdxModule::dQdxModule()
{
  _name = "dQdxModule";
}

void dQdxModule::initialize()
{
  // to go from mV to fC the ASIC gain and Shaping time
  // must be considered
  // fC -> mV *= ( shaping time * ASIC gain )
  _shp_time  = 2.;  // in usec
  _asic_gain = 7.8; // in mV/fC
  _fC_to_e = 6250.; // a fC in units of the electron charge
  _ADC_to_mV = 0.5; // ADC -> mV conversion from gain measurements
  _charge_conversion = _ADC_to_mV * _fC_to_e / ( _shp_time * _asic_gain ) ;
  // Correction factr
  _tau = larutil::LArProperties::GetME()->ElectronLifetime();             //electron lifetime in usec
  _timetick = larutil::DetectorProperties::GetME()->SamplingRate() * 1.e-3; //time sample in usec, 0.5us

  return;
}

void dQdxModule::do_reconstruction(const ProtoShower & proto_shower, Shower_t & resultShower) {

  auto & clusters = proto_shower.params();

  /***
  dQdx = hit charge / pitch .
  For details, see Doc-db 4909 .
  dQdx of a shower is the median of dQdxs of hits along trunk .
  Trunk connects cluster and shower start points .
  Calculation starts line 84 .
  dQdx is calculated for 3 planes. All variables are initialized
  correspondly as a 3-dimensional vector.
  Note within this Algorithm, the best plane is defined as the
  plane w/ longest trunk length. Therefore here best plane does
  NOT refer to best dQdx.
  User should define their own best plane OUTSIDE this algorithm.
  --------------------------------------------------------------
  In OCT 2015, develpers tried sigma or rms to better calculate
  dQdx but results showed no improvements. Now this part is removed.
  ***/

  auto geom = larutil::Geometry::GetME();
  auto geomHelper = larutil::GeometryHelper::GetME();
  const size_t nplanes = geom->Nplanes();
  //auto t2cm = geomHelper->TimeToCm();  //time to cm conversion for e lifetime correction
  auto const& dir3D = resultShower.fDCosStart;

  std::vector<double> trunk_length(nplanes, 0);
  std::vector<int>    n_hits(nplanes, 0);
  std::vector<std::vector<double> > dQdxs(nplanes, std::vector<double>());
  std::vector<double> median(nplanes, 0);

  //// loop over all input cluster -> calculate a dQdx per plane
  for (size_t n = 0; n < clusters.size(); n++) {

    // get the hits associated with this cluster
    auto const& hits = clusters.at(n).hit_vector;
    // get the plane associated with this cluster
    auto const& pl = clusters.at(n).plane_id.Plane;
    // get the start point for this cluster
    auto const& clu_start = clusters.at(n).start_point;
    // get the showering point for this cluster
    auto const& shr_start = clusters.at(n).showering_point;
    // cluster open angle
    auto const& cluster_open_angle = clusters.at(n).opening_angle;
    // start direction
    //auto const& start_dir = clusters.at(n).start_dir;

    if (pl >= nplanes) throw ShowerRecoException("Invalid plane found!");

    /***
    Find list of hits that are along the trunk of
    the shower. Grab all hits within 2.4cm w.r.t
    to cluster strat points and hits within trunk
    length compared to cluster start point.
    ---------------------------------------------
    2.4cm is just another magic number in uBooNE.
    If you found a better cut, should just modify it.
    ---------------------------------------------
    For all these hits, save dQdx into dQdx if tan
    value at cluster start point is no larger than
    tan value of shower open angle.
    ***/
    double pitch = geomHelper->GetPitch(dir3D, (int)pl);
    double dist_hit_clu;
    double dist_hit_shr;
    trunk_length[pl] = sqrt ( (shr_start.w - clu_start.w) * (shr_start.w - clu_start.w) +
                              (shr_start.t - clu_start.t) * (shr_start.t - clu_start.t) );
    n_hits[pl] = hits.size();

    for (int i = 0; i < n_hits[pl]; i++) {
      dist_hit_clu = sqrt((hits[i].w - clu_start.w) * (hits[i].w - clu_start.w) +
                          (hits[i].t - clu_start.t) * (hits[i].t - clu_start.t));
      dist_hit_shr = sqrt((hits[i].w - shr_start.w) * (hits[i].w - shr_start.w) +
                          (hits[i].t - shr_start.t) * (hits[i].t - shr_start.t));
      /***
          Correction for electron life time,
          Now removed since T0 is unknown.
          ---------------------------------
          double hit_tick =hits[i].t/t2cm;
          double lifetimeCorr = exp( hit_tick * _timetick / _tau );
          double Q = hits[i].charge * _charge_conversion*lifetimeCorr;
      ***/
      double Q = hits[i].charge * _charge_conversion;
      if (dist_hit_clu <= 2.4 || dist_hit_clu < trunk_length[pl]) {

        if (trunk_length[pl] == 0 || dist_hit_clu * dist_hit_shr == 0) {
          dQdxs[pl].push_back(Q / pitch);
        }

        else if (dist_hit_clu * dist_hit_shr > 0) {

          double cos_hit_angle = (dist_hit_clu * dist_hit_clu +
                                  trunk_length[pl] * trunk_length[pl] -
                                  dist_hit_shr * dist_hit_shr) / (2 * dist_hit_clu * trunk_length[pl]);

          if (tan(acos(cos_hit_angle)) <= tan(cluster_open_angle)) {
            dQdxs[pl].push_back(Q / pitch);
          }
        }
      }
    }
    // Get median of dQdx
    std::nth_element(dQdxs[pl].begin(), dQdxs[pl].begin() + dQdxs[pl].size() / 2, dQdxs[pl].end());
    if (dQdxs[pl].size() > 0)median[pl] = dQdxs[pl].at((dQdxs[pl].size() / 2));

    /***
    Best plane here refers to the plane w/ longest
    trunk length. Note that Some trunk length are
    larger than 10cm due to mis-assigned cluster
    or shower start point. Throw them away.
    --------------------------------------------------
    From previous study and for users' convenience,
    set Y plane the best plane.
    ***/
    /*if(pl==2){
    std::vector<std::pair<int,double>> pl_best;
    pl_best.resize(3);
    pl_best.at(0).first=0;
    pl_best.at(0).second=trunk_length[0];
    pl_best.at(1).first=1;
    pl_best.at(1).second=trunk_length[1];
    pl_best.at(2).first=2;
    pl_best.at(2).second=trunk_length[2];

    std::sort(pl_best.begin(),pl_best.end(),larger);

    if(pl_best.at(2).second<10)_pl_best=pl_best.at(2).first;
    else if(pl_best.at(1).second<10)_pl_best=pl_best.at(1).first;
    else if(pl_best.at(0).second<10)_pl_best=pl_best.at(0).first;
    else break;
    resultShower.fBestdQdxPlane = _pl_best;
    }*/
    _pl_best = nplanes - 1;
    resultShower.fBestdQdxPlane = _pl_best;
    resultShower.fdQdx_v[pl] = median[pl];

  }// for all clusters (planes)

  return;
}
} //showerreco

#endif
