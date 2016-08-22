#ifndef SELECTSTARTPOINT_CXX
#define SELECTSTARTPOINT_CXX

#include "SelectStartPoint.h"
#include "CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>

namespace cluster {

void SelectStartPoint::do_params_fill(cluster_params & cluster) {

  // Look at all the start point candidates and pick the one that is best
  // A start point candidate already is close to the min avgTan angle
  // and has passed a requirement that it's interior angle is less than pi/2
  //
  // Next, fit a chi2 of the hits against the start dir and pick the one with the min
  // chi2.  If values are close (within x2), pick the point that is closest to the mean
  // of the cluster.
  //
  // If there is only one candidate, then just set the value in cluster params and leave

  auto const& hits = cluster.hit_vector;
  auto & starts = cluster.start_point_cand;
  auto & shwrs = cluster.shwr_point_cand;
  auto & dirs = cluster.start_dir_cand;

  if (starts.size() != shwrs.size()) {
    return;
  }
  if (starts.size() != dirs.size()) {
    return;
  }

  if (starts.size() == 0) {
    // cluster.start_point;
    return;
  }

  if (_verbose) {
    std::cout << "Candidate start points are: \n";
    for (size_t i = 0; i < starts.size(); i++) {
      std::cout << "\t(" << starts.at(i).w << ", " << starts.at(i).t << ")";
      std::cout << ", dir: (" << dirs.at(i).w << ", " << dirs.at(i).t << ")\n";
    }
    std::cout << std::endl;
  }




  if (starts.size() == 1) {
    // cluster.start_point;
    return;
  }

  auto geoHelper = larutil::GeometryHelper::GetME();

  // Need a place to store the list of scores:
  std::vector<float> chi2Score(starts.size(), 0);
  // Now loop over the hits and add the values to the score
  for (auto const& hit : hits) {
    for (size_t i = 0; i < starts.size(); i ++) {
      chi2Score.at(i) += geoHelper -> DistanceToLine2D(starts.at(i), dirs.at(i), hit);
    }
  }

  // Select the point with the minimum chi2:
  float min = chi2Score.front();
  size_t min_index(0);
  for (size_t i = 0; i < chi2Score.size(); i++) {
    if (chi2Score.at(i) < min) {
      min = chi2Score.at(i);
      min_index = i;
    }
  }


  // Find out how many are within x2 of the min:
  int n_remaining_cand = 0;
  for (auto & val : chi2Score)
    if (val < min * 2) n_remaining_cand++;

  // If there is only one left, set things and leave:
  if (n_remaining_cand == 1) {
    cluster.start_point = starts.at(min_index);
    cluster.showering_point = shwrs.at(min_index);
    cluster.start_dir.resize(2);
    cluster.start_dir[0] = dirs.at(min_index).w;
    cluster.start_dir[1] = dirs.at(min_index).t;

    if (_verbose)
      std::cout << "Selected point " << min_index << std::endl;
    auto poly = cluster.PolyObject;
    auto nEdges = poly.Size();
    // Also fix the end point:
    // NEXT STEP:
    // Assign the end point
    // take the polygon point furthest from the start point
    // and project it down onto the direction vector
    // that will be our end point
    double maxDist = 0;
    size_t endEdge = 0;
    for (size_t n = 0; n < nEdges; n++) {
      // if (n == nStart)
      //   continue;
      double dist = ( (poly.Point(n).first - cluster.start_point.w) * (poly.Point(n).first - cluster.start_point.w) +
                      (poly.Point(n).second - cluster.start_point.t) * (poly.Point(n).second - cluster.start_point.t) );
      if (dist > maxDist) {
        maxDist = dist;
        endEdge = n;
      }
    }
    larutil::Point2D endPolyPoint(cluster.plane_id.Plane, poly.Point(endEdge).first, poly.Point(endEdge).second);
    // project this point onto the line determined by the slope and the start-point
    larutil::Point2D endPoint;
    geoHelper->GetPointOnLine(cluster.slope_2d, cluster.start_point, endPolyPoint, endPoint);
    cluster.end_point = endPoint;

    return;
  }


  // Else, compare all the points within x2 and select the one with the smallest
  // distance to the center of the cluster (charge weighted)
  float min_dist = 99999;
  for (size_t i = 0; i < chi2Score.size(); i++) {
    if (chi2Score.at(i) < 2 * min) {
      float dist = pow((starts.at(i).w - cluster.mean_x), 2) + pow((starts.at(i).t - cluster.mean_y), 2);
      if (dist < min_dist) {
        min_dist = dist;
        min_index = i;
      }
    }
  }

  // Now take the final point and use it.
  if (_verbose)
    std::cout << "Selected point " << min_index << std::endl;
  cluster.start_point = starts.at(min_index);
  cluster.showering_point = shwrs.at(min_index);
  cluster.start_dir.resize(2);
  cluster.start_dir[0] = dirs.at(min_index).w;
  cluster.start_dir[1] = dirs.at(min_index).t;
  auto poly = cluster.PolyObject;
  auto nEdges = poly.Size();
  // Also fix the end point:
  // NEXT STEP:
  // Assign the end point
  // take the polygon point furthest from the start point
  // and project it down onto the direction vector
  // that will be our end point
  double maxDist = 0;
  size_t endEdge = 0;
  for (size_t n = 0; n < nEdges; n++) {
    // if (n == nStart)
    //   continue;
    double dist = ( (poly.Point(n).first - cluster.start_point.w) * (poly.Point(n).first - cluster.start_point.w) +
                    (poly.Point(n).second - cluster.start_point.t) * (poly.Point(n).second - cluster.start_point.t) );
    if (dist > maxDist) {
      maxDist = dist;
      endEdge = n;
    }
  }
  larutil::Point2D endPolyPoint(cluster.plane_id.Plane, poly.Point(endEdge).first, poly.Point(endEdge).second);
  // project this point onto the line determined by the slope and the start-point
  larutil::Point2D endPoint;
  geoHelper->GetPointOnLine(cluster.slope_2d, cluster.start_point, endPolyPoint, endPoint);
  cluster.end_point = endPoint;




  return;
}

} // cluster

#endif
