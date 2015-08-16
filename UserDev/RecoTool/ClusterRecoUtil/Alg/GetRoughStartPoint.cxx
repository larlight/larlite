#ifndef GETROUGHSTARTPOINT_CXX
#define GETROUGHSTARTPOINT_CXX

#include "GetRoughStartPoint.h"
#include "CRUException.h"
#include <map>
#include <iomanip>

namespace cluster {

  GetRoughStartPoint::GetRoughStartPoint()
  {
    _name = "GetRoughStartPoint";
    _N    = 20;
    _angleMatrix = std::vector<std::vector<float> >(_N,std::vector<float>(_N));

  }

  void GetRoughStartPoint::do_params_fill(cluster_params & cluster, bool verbose){



    // The idea:
    // find N highest-charge hits
    // measure angle between segment connecting hits and slope_2d for each pair
    // the hit with the smallest spread in this parameter (summing over all angles for that hit)
    // will be the start point

    if (_verbose) { std::cout << "2d slope (found previously) is " << cluster.slope_2d << std::endl; }

    auto const& poly = cluster.PolyObject;
    size_t nEdges = poly.Size();
    _angleMatrix = std::vector<std::vector<float> >(nEdges,std::vector<float>(nEdges));

    // loop over nEdges twice:
    for (size_t n1 = 0; n1 < nEdges; n1++){
      for (size_t n2 = 0; n2 < nEdges; n2++){
	if (n1 == n2){
	  _angleMatrix[n1][n2] = 0;
	  continue;
	}
	Point2D p1(0,poly.Point(n1).first,poly.Point(n1).second);
	Point2D p2(0,poly.Point(n2).first,poly.Point(n2).second);
	auto const tanangle = GetAngle(p1,p2,cluster.slope_2d);
	_angleMatrix[n1][n2] = tanangle;
      }// for second loop over Poly points
    }// for 1st loop over Poly points

    /*
    // get hit-list
    auto const& hits = cluster.hit_vector;

    // get ordered hit list (from highest to lowest Q)
    // and save only the first N (or up to the last hit in the cluster)
    // Map[Charge] -> position of hit in hit vector
    std::map<double,size_t> hitmap;
    for(size_t i=0; i < hits.size(); i++)
      hitmap.insert(std::pair<double,size_t>(hits[i].charge,i));

    size_t nadded = 0;
    _highQT.clear();
    _highQW.clear();
    for (auto hiter = hitmap.rbegin(); hiter != hitmap.rend() && nadded < _N; hiter++){
      _highQT.push_back(hits[hiter->second].t);
      _highQW.push_back(hits[hiter->second].w);
      nadded += 1;
    }

    size_t nScanned1 = 0;
    size_t nScanned2 = 0;
    // loop through map of Q->hit in reverse order
    for (auto hiter1 = hitmap.rbegin(); hiter1 != hitmap.rend() && nScanned1 < _N; hiter1++){
      nScanned2 = 0;
      // second loop over hits
      for (auto hiter2 = hitmap.rbegin(); hiter2 != hitmap.rend() && nScanned2 < _N; hiter2++){
	// if same iterator -> angle is 0
	if (hiter1 == hiter2){
	  _angleMatrix[nScanned1][nScanned2] = 0;
	  continue;
	}
	// calculate tha angle between these two hits of interest
	//if (_verbose) { std::cout << "Checking entry in matrix: (" << nScanned1 << ", " << nScanned2 << ")" << std::endl; }
	//if (_verbose) { std::cout << "compare hits (" << hiter1->second << ", " << hiter2->second << ")"; }
	auto const tanangle = GetAngle(hits[hiter1->second],hits[hiter2->second],cluster.slope_2d);
	//if (_verbose) { std::cout << "\ttan(angle): " << tanangle << std::endl; }
	_angleMatrix[nScanned1][nScanned2] = tanangle;
	nScanned2 += 1;
      }// 2nd loop over hits
      nScanned1 += 1;
    }// 1st loop over hits
    */

    std::cout << std::setprecision(4) << std::fixed;

    // print out matrix
    if (_verbose){
      std::cout << "Matrix of angles between edges:" << std::endl;
      for (size_t n1 = 0; n1 < nEdges; n1++){
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
    double avgTan = kDOUBLE_MAX;
    size_t nScanned = 0;
    //for (auto hiter = hitmap.rbegin(); hiter != hitmap.rend() && nScanned < _N; hiter++){
    for (size_t n1 = 0; n1 < nEdges; n1++){
      double avg = 0;
      for (size_t n2 = 0; n2 < nEdges; n2++){
	double deg = _angleMatrix[n1][n2];
	if (deg < 0) avg -= deg;
	else avg += deg;
	//avg += _angleMatrix[n1][n2]*_angleMatrix[n1][n2];
      }
      // if we found the best average (lowest value)
      if (avg < avgTan){
	// save this hit as the start point
	start = Point2D(0,poly.Point(n1).first,poly.Point(n1).second);
	avgTan = avg;
      }
      nScanned += 1;
    }// for all rows in the matrix

    cluster.start_point = start;
    
    if (_verbose) { std::cout << "BEST HIT: (" << cluster.start_point.w << ", " << cluster.start_point.t << ")" << std::endl; }
    
    return;
  }
  
  const double GetRoughStartPoint::GetAngle(const Point2D& h1, const Point2D& h2, const double& slope){

    // Geometry Utilities
    auto geomHelper = ::larutil::GeometryHelper::GetME();

    // get slope between hits 1 and 2
    if ( (h2.w-h1.w) == 0) // vertical line
     // then the slope is 1/slope
      return 1/slope;

    double slopeHits = (h2.t-h1.t)/(h2.w-h1.w);
    //if (_verbose) { std::cout << "\tslope between hits: " << slopeHits; }
    
    return geomHelper->GetTanAngleBetweenLines(slopeHits,slope);
  }
  
} // cluster

#endif
