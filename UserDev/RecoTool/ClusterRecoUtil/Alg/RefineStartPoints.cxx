#ifndef REFINESTARTPOINTS_CXX
#define REFINESTARTPOINTS_CXX

#include "RefineStartPoints.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/GeometryHelper.h"

namespace cluster {

RefineStartPoints::RefineStartPoints()
{
  _name = "RefineStartPoints";
  _max_d_perp = 1.;
}


void RefineStartPoints::do_params_fill(cluster_params & cluster) {

  // Geometry Helper
  auto geomHelper = ::larutil::GeometryHelper::GetME();

  // the idea is to take a start point (wire,time) and find
  // a possible better start point looking at neighboring hits
  // this algorithm assumes that the start & end point
  // are approximately right
  // and the shower_dir direction is also ok

  auto const& hits  = cluster.hit_vector;
  auto const& start = cluster.start_point;
  auto const& end   = cluster.end_point;
  auto const& dir   = cluster.start_dir[1] / cluster.start_dir[0];

  // first find a subset of hits close to the start point
  std::vector<Point2D> close_hits;
  for (auto const& h : hits) {
    double distSq = ( (h.w - cluster.start_point.w) * (h.w - cluster.start_point.w) +
                      (h.t - cluster.start_point.t) * (h.t - cluster.start_point.t) );

    if (distSq < 25)
      close_hits.push_back(h);
  }

  // include the start point in this list
  close_hits.push_back(start);

  // for each 'close hit' calculate
  // 1) perpendicular distance to line with slope = dir and passing through start point
  // 2) distance from end point
  // a hit with small enough perpendicular distance
  // and furthest from the end point should be
  // the new start point

  if (_verbose) { std::cout << "Start : [" << start.w << ", " << start.t << "]" << std::endl; }

  // keep track of maximum distance from end point
  double maxDist = kDOUBLE_MIN;

  for (auto const& h : close_hits) {

    // perpendicular distance
    double d_perp = geomHelper->PerpendicularDistance(h, dir, start);

    if (_verbose) { std::cout << "perp distance is: " << d_perp << std::endl; }

    if (d_perp < _max_d_perp) {

      // distance from end point
      double distSq = ( (h.w - end.w) * (h.w - end.w) +
                        (h.t - end.t) * (h.t - end.t) );

      if (_verbose) { std::cout << "dist to end point is : " << distSq << std::endl; }

      if (distSq > maxDist)
      {
        if (_verbose) { std::cout << "new start point [" << h.w << ", " << h.t << "]" << std::endl; }
        maxDist = distSq;
        cluster.start_point = h;
      }
    }// if the point is close enough to the direction line
  }// for all hits

  if (_verbose) { std::cout << std::endl; }

  return;
}

} // cluster

#endif
