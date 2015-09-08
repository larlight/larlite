#ifndef FINDSHOWERINGPOINT_CXX
#define FINDSHOWERINGPOINT_CXX

#include "FindShoweringPoint.h"
#include "ClusterRecoUtil/Base/CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>
#include "TPrincipal.h"

namespace cluster {

void FindShoweringPoint::do_params_fill(cluster_params & cluster) {

  ///\ToDo: Make sure we don't try too hard to find showering Start Points on clusters
  ///that aren't showers, or are too small.

  // Geometry Utilities
  auto geomHelper = ::larutil::GeometryHelper::GetME();

  // get hit list
  auto const hits = cluster.hit_vector;

  // get start point
  auto const& start = cluster.start_point;

  // get end point
  auto const& end   = cluster.end_point;

  // get ordered hit list (from closest to start point to furthest)
  // save only those that are < 10 cm away
  // Map[dist] -> position of hit in hit vector
  std::map<double, size_t> hitmap;
  for (size_t i = 0; i < hits.size(); i++) {
    // distance to start point:
    double distSq = ( (hits[i].w - cluster.start_point.w) * (hits[i].w - cluster.start_point.w) +
                      (hits[i].t - cluster.start_point.t) * (hits[i].t - cluster.start_point.t) );

    if ( distSq < 25 ) {
      // check that the point is towards the shower direction (and not backwards)
      // this assumes we have got the right direction
      // the full cosine is not necessary. All we need is the sign.
      // so we just use the numerator from "GetCosAngleBetweenLines" function in GeometryHelper
      // for 3 points
      if ( ( (hits[i].w - start.w) * (end.w - start.w)
             + (hits[i].t - start.t) * (end.t - start.t) ) > 0 )
        hitmap.insert(std::pair<double, size_t>(distSq, i));
    }
  }


  // If there are no hits, we need to bail:
  if (hitmap.size() < 5) return;

  // loop through hits in distance-order to start-point
  // keep track of the cosine of the angle between
  // the line connecting points (n -> n+1) and (n+1 -> n+2)
  // if this cosine falls below a certain value -> stop and
  // mark the last point explored as the showering start point
  Point2D showering_start = start;
  size_t nPoints = 0;
  //\\\TODO: Make this slope determination done by a fit and not just connecting points.

  //typedef std::map<double,size_t>::iterator *it;
  // for (auto it = hitmap.begin(); it != hitmap.end(); it++){
  //   std::cout << "hit " << it->second << " charge: : " << it->first << std::endl;
  //   //std::cout << "this hit's charge: : " << next->first << std::endl;
  // }
  if (_verbose) std::cout << "\n\nLooping over " << hitmap.size() << " hits ..." << std::endl;
  for (auto it = hitmap.begin(); it != std::prev(std::prev(hitmap.end())); it++) {
    auto next = std::next(it);
    if (_verbose) {
      std::cout << "points: " << it->second << ", " << next->second << ", " << std::next(next)->second;
      std::cout << " (" << hits[it->second].w << ", " << hits[it->second].t << ")" << std::endl;
    }
    double cos = geomHelper->GetCosAngleBetweenLines(hits[(it)->second], hits[next->second],
                 hits[next->second], hits[std::next(next)->second]);
    if (cos > 0.7)
      showering_start = hits[std::next(next)->second];
    else
      break;
    nPoints += 1;
  }

  cluster.showering_point = showering_start;

  // Now that there is a showering point, use the start point and the showering point
  //  and the hits in between to determine the slope of the start of the shower
  //
  // If there are a few hits, use a linear regression to make a best fit line
  // Otherwise, use the difference between the two points
  //
  if (hitmap.size() < 5)
  {
    cluster.start_dir.resize(2);
    float mag = sqrt(pow((showering_start.w - cluster.start_point.w), 2) + pow((showering_start.t - cluster.start_point.t), 2));
    cluster.start_dir[0] = (showering_start.w - cluster.start_point.w) / mag;
    cluster.start_dir[1] = (showering_start.t - cluster.start_point.t) / mag;
  }
  else {

    TPrincipal fPrincipal(2, "D");
    for (auto& hit : hitmap) {
      // std::cout << "This hit has charge " <<  hit.charge << "\n";

      // Cast the wire/time information into a double array for prin. comp. analysis
      double data[2];
      data[0] = hits[hit.second].w;
      data[1] = hits[hit.second].t;
      // Add this data point to the PCA
      fPrincipal.AddRow(data);
    }

    // Run the PCA analysis
    fPrincipal.MakePrincipals();

    // Save the principal direction
    cluster.start_dir.resize(2);
    // std::cout << (* fPrincipal.GetEigenVectors())[0][0] << ", " << (* fPrincipal.GetEigenVectors())[0][1] << "\n"
    // << (* fPrincipal.GetEigenVectors())[1][0] << ", " << (* fPrincipal.GetEigenVectors())[1][1] << "\n";

    cluster.start_dir[0] = (* fPrincipal.GetEigenVectors())[0][0];
    cluster.start_dir[1] = (* fPrincipal.GetEigenVectors())[1][0];

    // float x_mean(0), y_mean(0), xy_mean(0), xx_mean(0);
    // for (auto it = hitmap.begin(); it != std::prev(std::prev(hitmap.end())); it++) {
    //   x_mean  += hits[it->second].w;
    //   y_mean  += hits[it->second].t;
    //   xy_mean += hits[it->second].w * hits[it->second].t;
    //   xx_mean += hits[it->second].w * hits[it->second].w;
    // }
    // int count = hitmap.size();
    // float slope = (count*xy_mean - x_mean * y_mean) / (count*xx_mean - x_mean * x_mean);
    // float theta = atan(slope);

    // cluster.start_dir[0] = cos(theta);
    // cluster.start_dir[1] = sin(theta);

  }


  //\\\TODO: Make this slope determination done by a fit and not just connecting points.

  return;
}

} // cluster

#endif
