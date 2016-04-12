#ifndef GEOMODULE_CXX
#define GEOMODULE_CXX

#include "GeoModule.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

void GeoModule::do_reconstruction(const ProtoShower & proto_shower,
                                  Shower_t& resultShower) {
  
  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
  }
  
  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower

  // Calculate the length and opening angle of the shower.

  // First, make sure the axis is pointing in the right direction
  // checkAxisDir(clusters, resultShower);

  // Length:
  fillLength(clusters, resultShower);

  // Get the opening angle:
  fillAngle(clusters, resultShower);

  // Take points that start at the 3D start point, and walk along the 3D axis towards the shower

}


// void GeoModule::checkAxisDir(const ShowerClusterSet_t & clusters, Shower_t & resultShower) {
//   // Method: take the start point and start direction, make two points.  THen project
//   // into the planes of the clusters and see if any are backwards.
//   //
//   // Essentially, compute the dotproduct of start -> end  vs start -> point further along axis
//   //
//   // Skip planes that are bad
//   //
//   //
//   TVector3 secondPoint = resultShower.fXYZStart + resultShower.fDCosStart;

//   // Keep track of the dot product in each plane
//   size_t nForward(0), nBackward(0)
//   for (size_t i = 0; i < clusters.size(); i++) {
//     // Check that this plane isn't bad
//     if (resultShower.fPlaneIsBad.at(i))
//       continue;
//     // Now it's confirmed good plane
//     // Get the 3 Point2D needed:
//     auto startPoint2D = geoHelper->Point_3Dto2D(resultShower.fXYZStart);
//     auto secondPoint2D = geoHelper -> Point_3Dto2D(secondPoint);

//     float dotProd = (startPoint2D.w - secondPoint2D.w) * (startPoint2D.w - clusters.at(i).mean_x);
//     dotProd += (startPoint2D.t - secondPoint2D.t) * (startPoint2D.t - clusters.at(i).mean_y);
//     if (dotProd > 0)
//       nForward ++;
//     else
//       nBackward ++;

//   }

//   // Now compare nForward to nBackward:
//   if (nBackward > nForward)
//     resultShower.fDCosStart *= -1.0;

//   return;
// }

void GeoModule::fillLength(const ShowerClusterSet_t & clusters, Shower_t & resultShower) {

  // Now calculate the length in 3D.  Do this by walking along the 3D vector, projecting
  // the point, calculating the dist to the real end point.

  auto geoHelper = larutil::GeometryHelper::GetME();

  float length = 10;
  float sign = 1;
  float stepSize = 10;

  TVector3 endPoint3D;

  bool converged = false;
  float prevError(-1);
  while (! converged) {
    endPoint3D = resultShower.fXYZStart + length * resultShower.fDCosStart;
    // Get the summed error of good planes
    float error = 0.0;
    for (size_t i = 0; i < clusters.size(); i++) {
      if (resultShower.fPlaneIsBad.at(i))
        continue;
      else {
        Point2D tempPoint = geoHelper->Point_3Dto2D(endPoint3D, clusters.at(i).plane_id.Plane);
        error += geoHelper -> Get2DDistanceSqrd(tempPoint, clusters.at(i).end_point);
      }
    }
    if (prevError < 0) {
      length += sign * stepSize;
      prevError = error;
      continue;
    }
    if (error < prevError) {
      length += sign * stepSize;
    }
    else {
      sign *= -1;
      stepSize *= 0.5;
      length += sign * stepSize;
    }
    prevError = error;

    // std::cout << "stepSize is " << stepSize << ", error is " << error << std::endl;

    if (stepSize < 0.5) {
      converged = true;
    }
  }

  if (length < 0) {
    resultShower.fLength = - length;
    resultShower.fDCosStart *= -1;
  }
  else {
    resultShower.fLength = length;
  }


}




void GeoModule::fillAngle(const ShowerClusterSet_t & clusters, Shower_t & resultShower) {

  resultShower.fOpeningAngle = clusters.front().opening_angle;

}

} //showerreco

#endif
