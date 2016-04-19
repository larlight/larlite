#ifndef GETROUGHSTARTPOINT_CXX
#define GETROUGHSTARTPOINT_CXX

#include "GetRoughStartPoint.h"
#include "ClusterRecoUtil/Base/CRUException.h"
#include "LArUtil/GeometryHelper.h"
#include <map>
#include <iomanip>

namespace cluster {

GetRoughStartPoint::GetRoughStartPoint()
{
  _name = "GetRoughStartPoint";
}

void GetRoughStartPoint::do_params_fill(cluster_params & cluster) {

  // Geometry Utilities
  auto geomHelper = ::larutil::GeometryHelper::GetME();


  // The idea:
  // find N highest-charge hits
  // measure angle between segment connecting hits and slope_2d for each pair
  // the hit with the smallest spread in this parameter (summing over all angles for that hit)
  // will be the start point

  // what plane are we on?
  int plane = cluster.hit_vector[0].plane;

  if (_verbose) { std::cout << "2d slope (found previously) is " << cluster.slope_2d << std::endl; }

  auto const& poly = cluster.PolyObject;
  size_t nEdges = poly.Size();
  _angleMatrix.clear();
  _angleMatrix = std::vector<std::vector<float> >(nEdges, std::vector<float>(nEdges));
  std::vector<float> interiorAngles;
  interiorAngles.resize(nEdges);
  // loop over nEdges twice:
  for (size_t n1 = 0; n1 < nEdges; n1++) {
    for (size_t n2 = 0; n2 < nEdges; n2++) {
      if (n1 == n2) {
        _angleMatrix[n1][n2] = 0;
        continue;
      }
      Point2D p1(0, poly.Point(n1).first, poly.Point(n1).second);
      Point2D p2(0, poly.Point(n2).first, poly.Point(n2).second);
      auto const tanangle = GetAngle(p1, p2, cluster.slope_2d);
      _angleMatrix[n1][n2] = tanangle;
    }// for second loop over Poly points
    interiorAngles.at(n1) = cluster.PolyObject.InteriorAngle(n1);
    // std::cout << "InteriorAngle is " << interiorAngles.at(n1) <<std::endl;
  }// for 1st loop over Poly points

  if (_verbose)
    std::cout << std::setprecision(4) << std::fixed;

  // print out matrix
  if (_verbose) {
    std::cout << "Matrix of angles between edges:" << std::endl;
    for (size_t n1 = 0; n1 < nEdges; n1++) {
      std::cout << "Point: (" << poly.Point(n1).first << ", " << poly.Point(n1).second << ")" << std::endl;
      for (size_t n2 = 0; n2 < nEdges; n2++)
        std::cout << _angleMatrix[n1][n2] << "\t";
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  // NEXT STEP:
  // now find the point with the smallest average tanangle entries
  Point2D start;
  size_t nStart = 0;
  std::vector<float> averageList;
  averageList.reserve(poly.Size());
  //for (auto hiter = hitmap.rbegin(); hiter != hitmap.rend() && nScanned < _N; hiter++){
  for (size_t n1 = 0; n1 < nEdges; n1++) {
    double avg = 0;
    for (size_t n2 = 0; n2 < nEdges; n2++) {
      double deg = _angleMatrix[n1][n2];
      if (deg < 0) avg -= deg;
      else avg += deg;
      //avg += _angleMatrix[n1][n2]*_angleMatrix[n1][n2];
    }
    averageList.push_back(avg);
  }

  // Figure out what the minimum average is:
  float minAvg = kDOUBLE_MAX;
  for (size_t n1 = 0; n1 < nEdges; n1++) {
    if (averageList[n1] < minAvg && interiorAngles[n1] < M_PI / 2.0) {
      minAvg = averageList[n1];
    }
  }

  // Take the avgTan and multiply by 4, then save all points where the candidates are
  float cutoffAvg = 4 * minAvg;
  for (size_t n1 = 0; n1 < nEdges; n1++) {
    // std::cout << "avg is " << averageList[n1]  << ", angle is " << interiorAngles[n1]
    //           << " at (" << poly.Point(n1).first
    //           << ", " << poly.Point(n1).second << ")" << std::endl;
    if (averageList[n1] < cutoffAvg && interiorAngles[n1] < M_PI / 2.0) {
      cluster.start_point_cand.push_back(Point2D(plane, poly.Point(n1).first, poly.Point(n1).second));
    }
    if (averageList[n1] == minAvg && interiorAngles[n1] < M_PI / 2.0) {
      start = Point2D(plane, poly.Point(n1).first, poly.Point(n1).second);
      nStart = n1;
    }
  }

  // if we found the best average (lowest value)



  cluster.start_point = start;


  // NEXT STEP:
  // Assign the end point
  // take the polygon point furthest from the start point
  // and project it down onto the direction vector
  // that will be our end point
  double maxDist = 0;
  size_t endEdge = 0;
  for (size_t n = 0; n < nEdges; n++) {
    if (n == nStart)
      continue;
    double dist = ( (poly.Point(n).first - poly.Point(nStart).first) * (poly.Point(n).first - poly.Point(nStart).first) +
                    (poly.Point(n).second - poly.Point(nStart).second) * (poly.Point(n).second - poly.Point(nStart).second) );
    if (dist > maxDist) {
      maxDist = dist;
      endEdge = n;
    }
  }
  Point2D endPolyPoint(plane, poly.Point(endEdge).first, poly.Point(endEdge).second);
  // project this point onto the line determined by the slope and the start-point
  Point2D endPoint;
  geomHelper->GetPointOnLine(cluster.slope_2d, cluster.start_point, endPolyPoint, endPoint);
  cluster.end_point = endPoint;

  if (_verbose) { std::cout << "BEST HIT: (" << cluster.start_point.w << ", " << cluster.start_point.t << ")" << std::endl; }

  return;
}

double GetRoughStartPoint::GetAngle(const Point2D& h1, const Point2D& h2, const double& slope) {

  // Geometry Utilities
  auto geomHelper = ::larutil::GeometryHelper::GetME();

  // get slope between hits 1 and 2
  if ( (h2.w - h1.w) == 0) // vertical line
    // then the slope is 1/slope
    return 1 / slope;

  double slopeHits = (h2.t - h1.t) / (h2.w - h1.w);
  //if (_verbose) { std::cout << "\tslope between hits: " << slopeHits; }

  return geomHelper->GetTanAngleBetweenLines(slopeHits, slope);
}

} // cluster

#endif
