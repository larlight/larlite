#ifndef FILLGEOMPARAMS_CXX
#define FILLGEOMPARAMS_CXX

#include "FillGeomParams.h"
#include "LArUtil/GeometryHelper.h"
// #include "ClusterRecoUtil/Base/CRUException.h"
#include <map>

namespace cluster {

FillGeomParams::FillGeomParams() {
  _percentage = 0.9;
  _Nbins      = 200;
  // _Qmin       = 25;
  _name       = "FillGeomParams";
}

void FillGeomParams::do_params_fill(cluster_params & cluster) {
  // Geometry Utilities
  auto geomHelper = ::larutil::GeometryHelper::GetME();

  larutil::PxPoint this_startPoint, this_endPoint;

  this_startPoint = cluster.start_point;
  this_endPoint   = cluster.end_point;

  if (_verbose) {
    std::cout << "Angle: Start point: (" << this_startPoint.w
              << ", " << this_startPoint.t << ")\n";
    std::cout << "Angle: End point  : (" << this_endPoint.w
              << ", " << this_endPoint.t << ")\n";
  }

  double endStartDiff_x = (this_endPoint.w - this_startPoint.w);
  double endStartDiff_y = (this_endPoint.t - this_startPoint.t);
  double rms_forward   = 0;
  double rms_backward  = 0;
  double mean_forward  = 0;
  double mean_backward = 0;
  //double weight_total  = 0;
  double hit_counter_forward  = 0;
  double hit_counter_backward = 0;
  /// Max( (distance between hit and shower axis)^2 ) over all hits
  double max_orthog_dist2 = -1.;

  if (_verbose && endStartDiff_y == 0 && endStartDiff_x == 0) {
    std::cerr << "Error:  end point and start point are the same!\n";

    return;
  }

  double percentage_HC = _percentage * cluster.N_Hits_HC / cluster.N_Hits;
  const double wgt = 1.0 / cluster.N_Hits;

  // Containers for the angle histograms
  std::vector<float> opening_angle_bin(_Nbins, 0.0 ) ;
  std::vector<float> closing_angle_bin(_Nbins, 0.0 ) ;
  std::vector<float> opening_angle_highcharge_bin(_Nbins, 0.0 ) ;
  std::vector<float> closing_angle_highcharge_bin(_Nbins, 0.0 ) ;
  std::vector<float> opening_angle_chargeWgt_bin(_Nbins, 0.0 ) ;
  std::vector<float> closing_angle_chargeWgt_bin(_Nbins, 0.0 ) ;

  int index = -1;

  // Calculate a Rough Charge Profile
  // This is basically a set of two numbers, containing info
  // on how much charge is in each half of the shower
  // Each hit's charget is added to either bin depending on whether
  // the perpendicular projection of the hit onto the 2D slope
  // of the cluster falls in the 1st or 2nd half.
  // the charge of the cluster is weighted by the perpendicular
  // distance to the 2Dslope line.
  float Q_begin = 0; ///< Charge (weighted) in the 1st half of the cluster
  float Q_end   = 0; ///< Charge (weighted) in the 2nd half of the cluster

  // calculate a cluster length as distance from start and end point
  cluster.length = geomHelper->Get2DDistance(cluster.start_point, cluster.end_point);


  for (auto& hit : cluster.hit_vector) {
    index ++;

    // keep information on a hit's projected point on the cluster's slope
    larutil::Point2D hitProjectedOnLine;
    geomHelper->GetPointOnLine(cluster.slope_2d, cluster.start_point, hit, hitProjectedOnLine);

    /// Distance between this hit and its projection onto the cluster axis
    /// This is computed to estimate the cluster "width"
    /// (note: using dist squared here because it is faster... will square-root at the end)
    double orthog_dist2 = geomHelper->Get2DDistanceSqrd(hit, hitProjectedOnLine);
    if (orthog_dist2 > max_orthog_dist2) max_orthog_dist2 = orthog_dist2;

    // Compute forward mean
    double hitStartDiff_x = (hit.w - this_startPoint.w) ;
    double hitStartDiff_y = (hit.t - this_startPoint.t) ;

    if (hitStartDiff_x == 0 && hitStartDiff_y == 0) continue;

    double cosangle_start = (endStartDiff_x * hitStartDiff_x +
                             endStartDiff_y * hitStartDiff_y);
    cosangle_start /= ( pow(pow(endStartDiff_x, 2) + pow(endStartDiff_y, 2), 0.5)
                        * pow(pow(hitStartDiff_x, 2) + pow(hitStartDiff_y, 2), 0.5));

    if (cosangle_start > 0) {
      // Only take into account for hits that are in "front"
      //no weighted average, works better as flat average w/ min charge cut
      mean_forward += cosangle_start;
      rms_forward  += pow(cosangle_start, 2);
      hit_counter_forward++;
    }

    // Compute backward mean
    double hitEndDiff_x = (hit.w - this_endPoint.w);
    double hitEndDiff_y = (hit.t - this_endPoint.t);
    if (hitEndDiff_x == 0 && hitEndDiff_y == 0) continue;

    double cosangle_end  = (endStartDiff_x * hitEndDiff_x +
                            endStartDiff_y * hitEndDiff_y) * (-1.);
    cosangle_end /= ( pow(pow(endStartDiff_x, 2) + pow(endStartDiff_y, 2), 0.5)
                      * pow(pow(hitEndDiff_x, 2) + pow(hitEndDiff_y, 2), 0.5));

    if (cosangle_end > 0) {
      //no weighted average, works better as flat average w/ min charge cut
      mean_backward += cosangle_end;
      rms_backward  += pow(cosangle_end, 2);
      hit_counter_backward++;
    }

    int N_bins_OPEN = (_Nbins - 1) * acos(cosangle_start) / M_PI;
    int N_bins_CLOSE = (_Nbins - 1) * acos(cosangle_end) / M_PI;
    if (N_bins_OPEN  < 0) N_bins_OPEN  = 0;
    if (N_bins_CLOSE < 0) N_bins_CLOSE = 0;

    if (_verbose) {
      std::cout << "endStartDiff_x :" << endStartDiff_x
                << "endStartDiff_y :" << endStartDiff_y
                << "cosangle_start :" << cosangle_start
                << "cosangle_end   :" << cosangle_end
                << "N_bins_OPEN    :" << N_bins_OPEN
                << "N_bins_CLOSE   :" << N_bins_CLOSE
                << std::endl;
    }

    opening_angle_chargeWgt_bin[N_bins_OPEN ]
    += hit.charge / cluster.sum_charge;
    closing_angle_chargeWgt_bin[N_bins_CLOSE]
    += hit.charge / cluster.sum_charge;
    opening_angle_bin[N_bins_OPEN] += wgt;
    closing_angle_bin[N_bins_CLOSE] += wgt;

    // //Also fill bins for high charge hits
    // if (hit.charge > cluster.mean_charge) {
    //   opening_angle_highcharge_bin[N_bins_OPEN] += wgt;
    //   closing_angle_highcharge_bin[N_bins_CLOSE] += wgt;
    // }

  } // end loop over hits in the cluster

  //Square-root the maximum orthogonal hit distance and call it (half of the) the cluster width
  cluster.width = std::sqrt(max_orthog_dist2);

  //initialize iterators for the angles
  size_t iBin(0), jBin(0), kBin(0), lBin(0), mBin(0), nBin(0);
  double percentage_OPEN(0.0),
         percentage_CLOSE(0.0),
         // percentage_OPEN_HC(0.0),
         // percentage_CLOSE_HC(0.0),  //The last 2 are for High Charge (HC)
         percentage_OPEN_CHARGEWGT(0.0),
         percentage_CLOSE_CHARGEWGT(0.0);

  for (iBin = 0; percentage_OPEN <= _percentage && iBin < _Nbins; iBin++)
    percentage_OPEN += opening_angle_bin[iBin];


  for (jBin = 0; percentage_CLOSE <= _percentage && jBin < _Nbins; jBin++)
    percentage_CLOSE += closing_angle_bin[jBin];

  // for (kBin = 0; percentage_OPEN_HC <= percentage_HC && kBin < _Nbins; kBin++)
  //   percentage_OPEN_HC += opening_angle_highcharge_bin[kBin];

  // for (lBin = 0; percentage_CLOSE_HC <= percentage_HC && lBin < _Nbins; lBin++)
  //   percentage_CLOSE_HC += closing_angle_highcharge_bin[lBin];

  for (mBin = 0; percentage_OPEN_CHARGEWGT <= _percentage && mBin < _Nbins; mBin++)
    percentage_OPEN_CHARGEWGT += opening_angle_chargeWgt_bin[mBin];

  for (nBin = 0; percentage_CLOSE_CHARGEWGT <= _percentage && nBin < _Nbins; nBin++)
    percentage_CLOSE_CHARGEWGT += closing_angle_chargeWgt_bin[nBin];

  double opening_angle = iBin * M_PI / _Nbins ;
  double closing_angle = jBin * M_PI / _Nbins ;
  double opening_angle_highcharge = kBin * M_PI / _Nbins ;
  double closing_angle_highcharge = lBin * M_PI / _Nbins ;
  double opening_angle_charge_wgt = mBin * M_PI / _Nbins ;
  double closing_angle_charge_wgt = nBin * M_PI / _Nbins ;

  cluster.opening_angle = opening_angle;
  cluster.closing_angle = closing_angle;
  cluster.opening_angle_charge_wgt = opening_angle_charge_wgt;
  cluster.closing_angle_charge_wgt = closing_angle_charge_wgt;

  /// Compute modified hit density
  /// This is the # of hits in the cluster divided by the length of the cluster
  /// (used often for track-shower cluster discrimination)
  /// (? operator used to avoid possible divide-by-zero errors)
  cluster.modified_hit_density = cluster.length ? cluster.N_Hits / cluster.length : -1.;

  if (_verbose) {
    std::cout << "opening angle: " << opening_angle
              << "closing angle: " << closing_angle
              << "opening high charge angle: " << opening_angle_highcharge
              << "closing high charge angle: " << closing_angle_highcharge
              << "opening high charge wgt  : " << opening_angle_charge_wgt
              << "closing high charge wgt  : " << closing_angle_charge_wgt
              << "fCoarseChargeProfile     : " << Q_begin
              << ", " << Q_end << std::endl;
  }
}

} // cluster

#endif
