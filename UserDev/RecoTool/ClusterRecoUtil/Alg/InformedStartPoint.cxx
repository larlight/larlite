#ifndef INFORMEDSTARTPOINT_CXX
#define INFORMEDSTARTPOINT_CXX

#include "InformedStartPoint.h"
#include "ClusterRecoUtil/Base/CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>
#include <iomanip>

namespace cluster {

InformedStartPoint::InformedStartPoint()
{
  _name = "InformedStartPoint";
  _has_current_guess = false;
  _min_distance = 2.0;
}

void InformedStartPoint::set_start_point_guess(const larutil::Hit2D & point) {
  _guess_start_point.w = point.w;
  _guess_start_point.t = point.t;
  _has_current_guess = true;
}
void InformedStartPoint::set_start_point_guess(const larutil::Point2D & point) {
  _guess_start_point.w = point.w;
  _guess_start_point.t = point.t;
  _has_current_guess = true;
}


void InformedStartPoint::do_params_fill(cluster_params & cluster) {

  if (! _has_current_guess) {
    throw CRUException("Can't use inform start point without setting a guess!");
  }

  // Geometry Utilities
  auto geomHelper = ::larutil::GeometryHelper::GetME();


  if (_verbose)
    std::cout << std::setprecision(4) << std::fixed;

  // Since there is a start point candidate, loop over all the hits
  // and select the ones that are within the min distance to the suggested start point.
  //
  // The one that is actually closest is set to the start point, while the rest are
  // set as candidate start points

  if (_verbose) {
    std::cout << "Start point suggestion is (" << _guess_start_point.w
              << ", " << _guess_start_point.t  << ")"
              << std::endl;
  }

  larutil::Hit2D best_hit;
  float _closest_dist = 9999;

  float dist_sqrd = _min_distance * _min_distance;
  for (auto & hit : cluster.hit_vector) {
    float dist = geomHelper -> Get2DDistanceSqrd(hit, _guess_start_point);
    // std::cout << "Dist is " << dist << std::endl;
    if (dist < dist_sqrd) {
      cluster.start_point_cand.push_back(hit);
      if (dist < _closest_dist) {
        cluster.start_point = hit;
        _closest_dist = dist;
      }
    }

  }

  if (_closest_dist == 9999){
    throw CRUException("Close hit not found to suggested start point!");

  }



  // NEXT STEP:
  // Assign the end point
  // take the polygon point furthest from the start point
  // and project it down onto the direction vector
  // that will be our end point
  double maxDist = 0;
  size_t endEdge = 0;
  for (size_t n = 0; n < cluster.PolyObject.Size(); n++) {

    double dist = ( (cluster.PolyObject.Point(n).first - cluster.start_point.w) *
                    (cluster.PolyObject.Point(n).first - cluster.start_point.w) +
                    (cluster.PolyObject.Point(n).second - cluster.start_point.t) *
                    (cluster.PolyObject.Point(n).second - cluster.start_point.t) );
    if (dist > maxDist) {
      maxDist = dist;
      endEdge = n;
    }
  }
  larutil::Point2D endPolyPoint(cluster.plane_id.Plane,
                       cluster.PolyObject.Point(endEdge).first,
                       cluster.PolyObject.Point(endEdge).second);
  // project this point onto the line determined by the slope and the start-point
  larutil::Point2D endPoint;
  geomHelper->GetPointOnLine(cluster.slope_2d,
                             cluster.start_point,
                             endPolyPoint,
                             endPoint);
  cluster.end_point = endPoint;

  if (_verbose) { std::cout << "BEST HIT: (" << cluster.start_point.w << ", " << cluster.start_point.t << ")" << std::endl; }

  _has_current_guess = false;

  return;
}


} // cluster

#endif
