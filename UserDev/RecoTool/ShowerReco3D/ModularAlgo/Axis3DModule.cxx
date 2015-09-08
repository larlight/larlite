#ifndef AXIS3DMODULE_CXX
#define AXIS3DMODULE_CXX

#include <iomanip>

#include "Axis3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

#define PI           3.14159265358979323846  /* pi */

namespace showerreco {

void Axis3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower) {

  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower.

  // std::cout << "Got here 1\n";

  auto geom = larutil::Geometry::GetME();
  auto geomHelper = larutil::GeometryHelper::GetME();


  // Start by making a guess:
  TVector3 pointOnAxis, direction(0, 0, 1.0);


  // Seed the pointOnAxis variable here.
  pointOnAxis.SetX( 0.0 );
  pointOnAxis.SetY( 0.0 );
  pointOnAxis.SetZ( geom -> DetLength() / 2.0 );



  // Also want the max dimensions of the detector easily reachable:
  std::vector<float> maxDim, minDim;
  maxDim.push_back(geom -> DetHalfWidth());
  maxDim.push_back(geom -> DetHalfHeight());
  maxDim.push_back(geom -> DetLength());
  minDim.push_back(-geom -> DetHalfWidth());
  minDim.push_back(-geom -> DetHalfHeight());
  minDim.push_back(0.0);


  // Determine the shower projections in 2D by plane (slopes):
  std::vector<float> slopeByPlane;
  std::vector<int> planes;

  // ///########################################################
  // // For algorithm development:
  // ///########################################################
  // // Start with a *known* 3D axis and project that into the plane:
  // TVector3 knownAxis(0.5, 0.5, 0.5);
  // knownAxis *= 1.0 / knownAxis.Mag();

  // for (unsigned int i = 0; i < inputShowers.size(); i++) {
  //   planes.push_back(inputShowers.at(i).plane_id.Plane);
  //   float slope = geomHelper -> Slope_3Dto2D(knownAxis, planes[i]);
  //   slopeByPlane.push_back(slope);
  //   std::cout << "Pushed back slope == " << slopeByPlane.back() << std::endl;
  // }

  // //########################################################
  // //End development section
  // //########################################################


  //########################################################
  //For real
  //########################################################
  int goodSlopes = 0;
  for (unsigned int i = 0; i < inputShowers.size(); i++) {
    planes.push_back(inputShowers.at(i).plane_id.Plane);
    // float slope = inputShowers.at(i).angle_2d;
    float slope = 0;
    if (inputShowers.at(i).start_dir[0] != 0) {
      // std::cout << "Using (" << inputShowers.at(i).start_dir[0] << ", " << inputShowers.at(i).start_dir[1] << ")\n";
      slope = inputShowers.at(i).start_dir[1] / inputShowers.at(i).start_dir[0];
    }
    if (slope != 0) goodSlopes ++;
    // std::cout << "Slope is " << slope << std::endl;
    slopeByPlane.push_back(slope);
  }

  if (goodSlopes < 2) {
    resultShower.fDCosStart = TVector3(0, 0, 0);
    return;
  }

  // Try to determine the 3D axis iteratively.
  // Here's how it is, I have NO IDEA how to rotate things to fix things.  So instead, just
  // make a bunch of guesses and narrow in on the result.


  TVector3 prevDir = direction;

  std::vector<TVector3> tempSeedVectors;
  generateSeedVectors(direction, M_PI / 2.0, fNStepsInitial, tempSeedVectors);

  // maintain a list of the good seeds and the errors of the seed vectors too:
  std::vector<TVector3> seedVectors;
  std::vector<float>    errorVector;
  std::vector<bool>     convergeStatus;
  std::vector<int>      convergeNumber;

  float errorCutoff = 0.5;

  // Find out the seed vectors with reasonable error, and reject all the others
  while (seedVectors.size() == 0) {
    for (auto & vec : tempSeedVectors) {
      float error = getErrorOfProjection(vec, slopeByPlane, planes);
      if (error < errorCutoff) {
        seedVectors.push_back(vec);
        errorVector.push_back(error);
        // std::cout << "vector "
        //           << vec.X() << ", "
        //           << vec.Y() << ", "
        //           << vec.Z() << ")., error " << error
        //           << " was kept\n";
      }
      // else {
      //   std::cout << "vector "
      //             << vec.X() << ", "
      //             << vec.Y() << ", "
      //             << vec.Z() << ")., error " << error
      //             << " was rejected\n";
      // }
    }
    errorCutoff += 0.1;
  }

  // Now the list of seed vectors is reduced to a few candidate vectors that are close

  // For each seed vector, compare the error of it vs. it's neighbors.
  // If the error at a neighbor is better, move to that.


  // Some variables needed for this loop:
  bool converged = false;
  int n_converged = 0;
  int current_iteration = 0;

  float thetaRangeStart = 0.05;
  float thetaRangeMax  = M_PI / 8;
  float thetaRangeMin  = 0.0005;

  float nStepsStart = 8;
  float nStepsMax = 4 * nStepsStart;

  float alpha = 0.85;

  int vecIndex = 0;
  for (auto & vec : seedVectors) {
    float thetaRange = thetaRangeStart;
    float nSteps = nStepsStart;
    converged = false;
    current_iteration = 0;
    while ( ! converged ) {
      std::vector<TVector3> neighborVectors;
      // Make the list of the neighbor vectors:
      generateNearbyVectors(vec, thetaRange, nSteps, neighborVectors);
      // Find out which neighbor vector is the best:
      int bestNeighborIndex = findBestVector(neighborVectors, slopeByPlane, planes);
      // Compare the error of the best neighbor to the error of the vec:
      float error = getErrorOfProjection(neighborVectors[bestNeighborIndex], slopeByPlane, planes);



      // Now we have the best neighbor vector.
      // If things improved, just make the new vector the seed and iterate.
      //
      // If things didn't improve, make thetaRange and nSteps bigger and try again.
      //
      // If things didn't improve and thetaRange or nSteps are maxed out, bail.

      if (error < errorVector.at(vecIndex)) {
        vec = neighborVectors[bestNeighborIndex];
        errorVector.at(vecIndex) = error;
        // If there was improvement, make sure that nSteps is
        // set to its min value:
        nSteps = nStepsStart;
        // For theta range, since that is the convergence speed, set it to
        // the max of it's min value and alpha*currentValue:
        thetaRange = fmin(thetaRangeStart, alpha * thetaRange);
        if (thetaRange < thetaRangeMin) thetaRange = thetaRangeMin;
      }
      // Else things didn't improve
      else {
        // This step didn't help. Try to increase the granularity first:
        if (nSteps < nStepsMax) {
          nSteps += nStepsStart;
        }
        // If that fails, perhaps need to look at smaller angles:
        else if (thetaRange > thetaRangeMin) {
          thetaRange *= alpha;
        }
        else {
          // Both nSteps and theta range are maxed out.  Bail on this point.
          // std::cout << "bailing because both maxed out.\n";
          break;
        }
      }
      // Check if we've reached the desired error:
      if (error < fTargetError) {
        vec = neighborVectors[bestNeighborIndex];
        errorVector.at(vecIndex) = error;
        converged = true;
      }

      current_iteration ++;
      if (current_iteration > fMaxIterations) {
        converged = false;
        break;
      }
    }
    // Keep track of whether or not the previous point converged
    convergeStatus.push_back(converged);
    convergeNumber.push_back(current_iteration);
    if (converged) n_converged ++;
    vecIndex ++;
  }

  // Keep the best vector with the best error;
  int bestIndex = -1;
  float bestError = 9999;

  // Print out the final vectors and their errors:
  vecIndex = 0;
  for (auto & vec : seedVectors) {
    if (n_converged == 0 || convergeStatus.at(vecIndex) ) {
      if (errorVector.at(vecIndex) < bestError) {
        bestIndex = vecIndex;
        bestError = errorVector.at(vecIndex);
      }
    }
    // std::cout << "Seed vector " <<  vecIndex << ": ("
    //           << vec.X() << ", "
    //           << vec.Y() << ", "
    //           << vec.Z() << ")., error: "
    //           << errorVector.at(vecIndex) << " did ";
    // if (! convergeStatus.at(vecIndex)) std::cout << "not ";
    // std::cout << "converge after " << convergeNumber.at(vecIndex) << " iterations."
    //           << std::endl;
    vecIndex ++;
  }


  // Set the best vector to direction:
  if (bestIndex != -1)
    resultShower.fDCosStart = seedVectors.at(bestIndex);

  // while ( ! converged ) {
  //   // Get a set of seed vectors:

  //   int min_index = findBestVector(seedVectors, slopeByPlane, planes);
  //   prevDir = direction;
  //   direction = seedVectors.at(min_index);

  //   // Print out an update:
  //   float error = getErrorOfProjection(direction, slopeByPlane, planes);
  //   std::cout << "Current vector: ("
  //             << direction.X() << ", "
  //             << direction.Y() << ", "
  //             << direction.Z() << ")., error: "
  //             << error
  //             << ", thetaRange: " << thetaRange
  //             << ", steps: " << nSteps_current
  //             << std::endl;

  //   ///TODO:
  //   // Keep track of all vectors with better error than last iteration, or best N
  //   // Which ever is lower.
  //   // Then, make the ranges around those vectors based on some angle
  //   // Continue keeping track of multiple vectors
  //   // so that it doesn't get stuck in a local maximum


  //   // Only reduce the range of theta if the error is getting good:
  //   if (error < 1 ) {
  //     // Narrow down if we're getting better
  //     if (error < prevError) {
  //       // Use the taylor series for arccosine
  //       thetaRange = 1.2 * (PI / 2 - prevDir.Dot(direction) - pow(prevDir.Dot(direction), 3) / 6);
  //       std::cout << "Theta range is " << thetaRange << std::endl;
  //       nSteps_current = fNStepsMin;
  //     }
  //     // Go finer if it's good but not getting better
  //     else
  //       nSteps_current += fNStepsMin + 1;
  //   }
  //   // Otherwise, go for finer sampling to find a good spot:
  //   else {
  //     nSteps_current += 2 * fNStepsMin + 1;
  //   }

  //   prevError = error;

  //   // Compare the 3D vector and the *known* 3D vector:



  //   error /= inputShowers.size();

  //   if (error < fTargetError) converged = true;
  //   current_iteration ++;
  //   if (current_iteration > fMaxIterations) break;
  // }

  resultShower.fXYZStart = pointOnAxis;


  // std::cout << "FINAL RESULT: \n";

  // std::cout
  // // << "True vector: ("
  // // << knownAxis.X() << ", "
  // // << knownAxis.Y() << ", "
  // // << knownAxis.Z() << "), "
  //     << "Calculated vector: ("
  //     << resultShower.fDCosStart.X() << ", "
  //     << resultShower.fDCosStart.Y() << ", "
  //     << resultShower.fDCosStart.Z() << ").\n";
  // // << " Difference is " << acos(knownAxis.Dot(resultShower.fDCosStart)) << " radians.\n";

  // for (unsigned int i = 0; i < inputShowers.size(); i++) {
  //   larutil::PxPoint start2D, dir2D;
  //   geomHelper -> Line_3Dto2D(pointOnAxis, resultShower.fDCosStart, planes[i], start2D, dir2D);
  //   std::cout << "\tIn plane " << planes[i] << ", the projection is "
  //             << (dir2D.t / dir2D.w) << ", the target is "
  //             << slopeByPlane[i] << ".\n";
  // }


  return;

}


void Axis3DModule::generateNearbyVectors( const TVector3 & initialVector,
    float thetaRange,
    int nSteps,
    std::vector<TVector3> & result)
{
  // Initialize the result
  result.clear();
  result.reserve(nSteps);

  // Do some safety checks:
  if (initialVector.Mag() == 0) {
    std::cerr << "ERROR - Axis3DModule::generateSeedVectors: can not have initial vector be zero!\n";
    return;
  }
  if (nSteps == 0) {
    std::cerr << "ERROR - Axis3DModule::generateSeedVectors: can not generate a list of zero steps!\n";
    return;
  }



  // Need one vector to rotate around and such.  Make it start as a copy of the initialVector
  TVector3 workerVector(initialVector);
  // Make sure it's normalized:
  workerVector *= 1.0 / workerVector.Mag();


  // Now, find a vector that is perpendicular to the initialVector:
  // Need to make sure it's not zero, and not parallel to the workerVector
  TVector3 perp(workerVector.X() - 1, workerVector.Y() + 1, workerVector.Z() + 2);
  perp = perp.Cross(workerVector);
  perp *= 1.0 / perp.Mag();


  TVector3 seedVector = workerVector;
  // Rotate up a fraction of the 90 degrees needed:
  seedVector.Rotate(thetaRange, perp );
  // Rotate around the center vector a full rotation
  for (float j = 0; j < nSteps - 1; j++) {
    seedVector.Rotate( (PI / nSteps) * (j), workerVector);
    result.push_back(seedVector);
  }


  return;
}

void Axis3DModule::generateSeedVectors(const TVector3 & initialVector,
                                       float thetaRange,
                                       int nSteps,
                                       std::vector<TVector3> & result)
{

  // Initialize the result
  result.clear();
  result.reserve(nSteps * nSteps);

  // Do some safety checks:
  if (initialVector.Mag() == 0) {
    std::cerr << "ERROR - Axis3DModule::generateSeedVectors: can not have initial vector be zero!\n";
    return;
  }
  if (nSteps == 0) {
    std::cerr << "ERROR - Axis3DModule::generateSeedVectors: can not generate a list of zero steps!\n";
    return;
  }



  // Need one vector to rotate around and such.  Make it start as a copy of the initialVector
  TVector3 workerVector(initialVector);
  // Make sure it's normalized:
  workerVector *= 1.0 / workerVector.Mag();

  // Push back the initial vector, since it ought to be included:
  result.push_back(workerVector);

  // Now, find a vector that is perpendicular to the initialVector:
  // Need to make sure it's not zero, and not parallel to the workerVector
  TVector3 perp(workerVector.X() - 1, workerVector.Y() + 1, workerVector.Z() + 2);
  perp = perp.Cross(workerVector);
  perp *= 1.0 / perp.Mag();

  for (float i = 1.0; i <= nSteps; i++) {
    TVector3 seedVector = workerVector;
    // Rotate up a fraction of the 90 degrees needed:
    seedVector.Rotate( (thetaRange / nSteps) * (i), perp );
    // Rotate around the center vector a full rotation
    for (float j = 0; j < nSteps - 1; j++) {
      seedVector.Rotate( (PI / nSteps) * (j), workerVector);
      result.push_back(seedVector);
    }
  }
  return;

}
int Axis3DModule::findBestVector( const std::vector<TVector3> seedVectors,
                                  const std::vector<float> & slopesByPlane,
                                  const std::vector<int> & planes) {

  // Loop over the vectors, calculate the error, and return the one with smallest error
  std::vector<float> errs;
  errs.reserve(seedVectors.size());
  for (auto & vec : seedVectors) {
    errs.push_back(getErrorOfProjection(vec, slopesByPlane, planes));
    // std::cout << "vector: ("
    //           << vec.X() << ", "
    //           << vec.Y() << ", "
    //           << vec.Z() << ")., error: "
    //           << errs.back() << std::endl;
  }
  // Use a standard function to find the min
  int min_index = min_element(errs.begin(), errs.end()) - errs.begin();
  return min_index;

}
float Axis3DModule::getErrorOfProjection( const TVector3 & inputVector,
    const std::vector<float> & slopesByPlane,
    const std::vector<int> & planes)
{

  auto geomHelper = larutil::GeometryHelper::GetME();
  float err = 0;
  // float wgt = 0;
  for (unsigned int i = 0; i < slopesByPlane.size(); i++) {
    // Get the 2D slope for this plane, and compute the error as squared difference
    float slope = geomHelper -> Slope_3Dto2D(inputVector, planes[i]);
    // float slope = atan(geomHelper -> Slope_3Dto2D(inputVector, planes[i]));
    // float error = (slope - slopesByPlane[i]);
    float error = 0;
    if (slopesByPlane[i] != 0) {
      error = (slope - slopesByPlane[i]) / (slopesByPlane[i]);
      err += fabs(error);
    }
    // float error = 1 - fabs(slope / slopesByPlane[i]);
    // Weight this error by the inverse value of the target slope (so that higher slopes don't get higher weight)
    // wgt += 1.0 / slopesByPlane[i];
  }

  err /= planes.size();

  return err;

}




} //showerreco

#endif
