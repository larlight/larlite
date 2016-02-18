#ifndef OTHERSTARTPOINT3D_CXX
#define OTHERSTARTPOINT3D_CXX

#include "OtherStartPoint3D.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"

namespace showerreco {

std::vector<std::vector<double>> OtherStartPoint3D::calculatePoints(const std::vector<double> & start,
const double & dist               ) {

//    std::cout<<"New Calculation! "<<std::endl ;

  std::vector<std::vector<double>> tempCoords ;
  tempCoords.resize(7);

  ///Coords are: start, x varied, y varied, z varied
  tempCoords[0] = start ;
  tempCoords[1] = { start[0] - dist, start[1], start[2] };
  tempCoords[2] = { start[0] + dist, start[1], start[2] };
  tempCoords[3] = { start[0], start[1] - dist, start[2] };
  tempCoords[4] = { start[0], start[1] + dist, start[2] };
  tempCoords[5] = { start[0], start[1], start[2] - dist };
  tempCoords[6] = { start[0], start[1], start[2] + dist };

  //  for(int i=0; i < tempCoords.size();i++)
//  std::cout<<"Points: "<<tempCoords[i][0]<<", " <<tempCoords[i][1]<<", "<<tempCoords[i][2]<<std::endl ;

  return tempCoords;
}

void OtherStartPoint3D::do_reconstruction(const ProtoShower & proto_shower,
    Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

//  std::cout<<"\n\n********************************New event!***************************** "<<std::endl ;

  // This function takes the matched shower cluster set and computes the best fit 3D start point
  // and then assigns it to the shower
  auto geomHelper = larutil::GeometryHelper::GetME();
  // auto detProp = larutil::DetectorProperties::GetME() ;


  /**
  *  Calculate worst planes-- gives you the 2 best planes
  */
  float minClusDist      =  999999 ;
  int worstPlane  = -1      ;
  int planeTemp   =  0      ;
  for ( auto const & c : clusters ) {
    float distTemp = abs ( c.start_point.w - c.end_point.w );

    if ( distTemp < minClusDist ) {
      minClusDist = distTemp ;
      worstPlane = planeTemp;
    }
    planeTemp ++ ;
  }

  double time = 0. ;
  for ( auto const & c : clusters ) {
    if ((int) c.plane_id.Plane != worstPlane) time += c.start_point.t;
  }

  /**
  *  Calculate first of all the points!
  */
  auto pts = calculatePoints({time / 2, 0, 1036. / 2}, _initDist);


  /**
  *  Project points into best 2 planes
  */
  double distFromStart = _initDist;
  int level = 1;
  int minDistIt = -1 ; //Keep track of the iterator with smallest error
  double minDist ;

  while ( distFromStart > 0.5 ) {
    minDist  = 99999999. ;
    for ( auto const c : clusters ) {
      if ((int) c.plane_id.Plane != worstPlane ) {
        for (unsigned int i = 0; i < pts.size(); i++) {

          auto testPoint = geomHelper->Point_3Dto2D(pts[i], c.plane_id.Plane) ;
          auto dist = sqrt ( pow(c.start_point.t - testPoint.t, 2) +
                             pow(c.start_point.w - testPoint.w, 2) ) ;

          if ( dist < minDist ) {
            minDistIt = i ;
            minDist   = dist ;
          }

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




  }
  //    std::cout<<"min dist : "<<minDist<<std::endl ;



  resultShower.fXYZStart = {0, 1, 2} ; //anchorCoords[minDistIt] ;

}

bool OtherStartPoint3D::inTPC(const TVector3 & pt ) {

  if ( pt[0] < 0      || pt[0] > 256.35 ||
       pt[1] < -116.5 || pt[1] > 116.5  ||
       pt[2] < 0      || pt[2] > 1036.8  )

    return false ;

  else
    return true;

}

} //showerreco

#endif
