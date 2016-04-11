#ifndef OTHERSTARTPOINT3D_CXX
#define OTHERSTARTPOINT3D_CXX

#include "OtherStartPoint3D.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"

namespace showerreco {

OtherStartPoint3D::OtherStartPoint3D() {
  _name = "OtherStartPoint3D";
  _initDist = 5;
}

std::vector<TVector3 >
OtherStartPoint3D::calculatePoints(
  const TVector3 & start,
  const double & dist)
{

//    std::cout<<"New Calculation! "<<std::endl ;

  std::vector<TVector3> tempCoords(7, start) ;

  ///Coords are: start, x varied, y varied, z varied
  tempCoords[1].SetX(tempCoords[1].X() - dist);
  tempCoords[2].SetX(tempCoords[2].X() + dist);
  tempCoords[3].SetY(tempCoords[3].Y() - dist);
  tempCoords[4].SetY(tempCoords[4].Y() + dist);
  tempCoords[5].SetZ(tempCoords[5].Z() - dist);
  tempCoords[6].SetZ(tempCoords[6].Z() + dist);


  //  for(int i=0; i < tempCoords.size();i++)
//  std::cout<<"Points: "<<tempCoords[i][0]<<", " <<tempCoords[i][1]<<", "<<tempCoords[i][2]<<std::endl ;

  return tempCoords;
}

void OtherStartPoint3D::do_reconstruction(
  const ProtoShower & proto_shower,
  Shower_t& resultShower)
{

  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
    throw ShowerRecoException(ss.str());
  }

  auto & clusters = proto_shower.params();

//  std::cout<<"\n\n********************************New event!***************************** "<<std::endl ;

  // This function takes the matched shower cluster set
  // and computes the best fit 3D start point
  // and then assigns it to the shower
  auto geomHelper = larutil::GeometryHelper::GetME();
  auto geom = larutil::Geometry::GetME();
  // auto detProp = larutil::DetectorProperties::GetME() ;


  // /**
  // *  Calculate worst planes-- gives you the 2 best planes
  // */
  // float minClusDist      =  999999 ;
  // int worstPlane  = -1      ;
  // int planeTemp   =  0      ;
  // for ( auto const & c : clusters ) {
  //   float distTemp = abs ( c.start_point.w - c.end_point.w );

  //   if ( distTemp < minClusDist ) {
  //     minClusDist = distTemp ;
  //     worstPlane = planeTemp;
  //   }
  //   planeTemp ++ ;
  // }

  // double time = 0. ;
  // for ( auto const & c : clusters ) {
  //   if ((int) c.plane_id.Plane != worstPlane) time += c.start_point.t;
  // }

  /**
  *  Calculate first of all the points!
  */


  // Use the current start point as a seed to this algorithm.

  TVector3 currentBestPoint = resultShower.fXYZStart;
  // if the current start point isn't set, set it to the
  // middle of the detector
  if (currentBestPoint.Mag() < 0.01) {
    currentBestPoint.SetX(geom -> DetHalfWidth());
    currentBestPoint.SetY(0.0);
    currentBestPoint.SetZ(0.5 * geom -> DetLength());
  }

  // std::cout << "Starting point is ("
  //           << currentBestPoint.X() << ", "
  //           << currentBestPoint.Y() << ", "
  //           << currentBestPoint.Z() << ")" << std::endl;


  auto pts = calculatePoints(currentBestPoint, _initDist);


  /**
  *  Project points into best 2 planes
  */
  double distFromStart = 999;
  int level = 1;
  int minDistIt = -1 ; //Keep track of the iterator with smallest error
  double minDist ;

  int max_interations = 200;
  int iteration_counter = 0;

  while ( distFromStart > 0.01  && iteration_counter < max_interations) {
    minDist  = 99999999. ;
    for ( auto const c : clusters ) {
      for (unsigned int i = 0; i < pts.size(); i++) {

        auto testPoint = geomHelper
                         ->Point_3Dto2D(pts[i], c.plane_id.Plane) ;
        auto dist =  pow(c.start_point.t - testPoint.t, 2) +
                     pow(c.start_point.w - testPoint.w, 2);

        // std::cout << "Iteration " << iteration_counter
        //           << ", Point " << i
        //           << ", dist is " << dist << std::endl;

        if ( dist < minDist ) {
          minDistIt = i ;
          minDist   = dist ;
        }

//        std::cout<<"\nCluster points: "<<c.start_point.t<<", "<<c.start_point.w<<std::endl;
//        std::cout<<"Test points : "<<geomHelper->Point_3Dto2D(pts[minDistIt],c.plane_id.Plane).t<<", "
//                 <<geomHelper->Point_3Dto2D(pts[minDistIt],c.plane_id.Plane).t<<std::endl;
//        std::cout<<"Min iterator: "<<minDistIt<<std::endl;
      }//if not worst plane
    }

    if ( minDistIt == 0 ) {
      distFromStart = _initDist / (level + 1);
      pts = calculatePoints(pts[minDistIt], distFromStart ) ;
      level++;
    }
    else
      pts = calculatePoints(pts[minDistIt], _initDist / level) ;

    iteration_counter ++;
  }
  //    std::cout<<"min dist : "<<minDist<<std::endl ;


  // std::cout << "Ending point is ("
  //           << pts[minDistIt].X() << ", "
  //           << pts[minDistIt].Y() << ", "
  //           << pts[minDistIt].Z() << "), error is "
  //           << minDist
  //           << std::endl;

  // std::cout << "Start point in each plane is: \n";
  // for (auto & clust : clusters) {
  //   auto testPoint = geomHelper
  //                    ->Point_3Dto2D(pts[minDistIt],
  //                                   clust.plane_id.Plane) ;
  //   std::cout << "\tPlane " << clust.plane_id.Plane
  //             << ", startpoint=(" << clust.start_point.w
  //             << ", " << clust.start_point.t << ")"
  //             << ", reco= (" << testPoint.w
  //             << ", " << testPoint.t << ")" << std::endl;
  // }


  resultShower.fXYZStart = pts[minDistIt] ; //anchorCoords[minDistIt] ;

}



} //showerreco

#endif
