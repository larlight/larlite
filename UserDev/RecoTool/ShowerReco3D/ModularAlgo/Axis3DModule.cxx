#ifndef AXIS3DMODULE_CXX
#define AXIS3DMODULE_CXX

#include <iomanip>

#include "Axis3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"


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
  std::vector<Status>   convergeStatus;
  std::vector<int>      convergeNumber;
  int                   n_converged(0);

  float errorCutoff = _seedVectorErrorCutoff;

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
    errorCutoff += _seedVectorErrorCutoff;
  }

  // Now the list of seed vectors is reduced to a few candidate vectors that are close

  int vecIndex = 0;
  for (auto & vec : seedVectors) {
    Status exitStatus = kNStatus;
    int n_iterations = 0;
    errorVector.at(vecIndex) = optimzeVector(vec, exitStatus, n_iterations, slopeByPlane, planes);
    convergeStatus.push_back(exitStatus);
    convergeNumber.push_back(n_iterations);
    if (exitStatus == kNormal)
      n_converged++;
    vecIndex ++;
  }

  // Keep the best vector with the best error;
  int bestIndex = -1;
  float bestError = 9999;

  // Print out the final vectors and their errors:
  vecIndex = 0;
  for (auto & vec : seedVectors) {
    // Print out info about this vector:
    if (_verbose) {
      std::cout << "Error: " << errorVector.at(vecIndex)
                << "\tVec: (" << vec.X() << ", "
                << vec.Y() << ", "
                << vec.Z() << ")"
                << "\tStatus: ";
      if (convergeStatus.at(vecIndex) == kNormal)
        std::cout << "normal";
      if (convergeStatus.at(vecIndex) == kIterMaxOut)
        std::cout << "maxout";
      if (convergeStatus.at(vecIndex) == kNotOptFit)
        std::cout << "nonopt";
      std::cout << "\tN: " << convergeNumber.at(vecIndex);
      std::cout << std::endl;
      for (unsigned int p = 0; p < planes.size(); p++) {
        float slope = geomHelper -> Slope_3Dto2D(vec, planes[p]);
        std::cout << "\t" << p << " - \tgoal:\t" << slopeByPlane.at(p) << "\tact:\t" << slope << "\n";
      }
    }



    if ( convergeStatus.at(vecIndex) == kNormal ) {
      if (errorVector.at(vecIndex) < bestError) {
        bestIndex = vecIndex;
        bestError = errorVector.at(vecIndex);
      }
    }
    vecIndex ++;
  }


  // Set the best vector to direction:
  if (bestIndex != -1)
    resultShower.fDCosStart = seedVectors.at(bestIndex);
  // else {
  //   // In this case, none of the seeds converged.  Maxout seed vectors aren't useful, but
  //   // the nonopt ones are.  We can try removing each plane and refitting all the vectors
  //   // that were returned to see if they converge.
  //   vecIndex = 0;
  //   for (auto & vec : seedVectors ) {
  //     // check the return value, skip max out vectors.:
  //     if (convergeStatus.at(vecIndex) == kIterMaxOut)
  //       continue;
  //     // else {
  //     //   std::cout << "Going into alternative fitting options" <<std::endl;
  //     //   // try to fit this vector against the planes but remove one plane.
  //     //   for (unsigned int i = 0; i < slopeByPlane.size(); i ++) {
  //     //     // Make a temp slope and temp plane object for this case:
  //     //     std::vector<float> tempSlopes;
  //     //     std::vector<int> tempPlane;
  //     //     for (unsigned int j = 0; j < slopeByPlane.size(); j++) {
  //     //       if (i == j) continue;
  //     //       tempSlopes.push_back(slopeByPlane.at(j));
  //     //       tempPlane.push_back(planes.at(j));
  //     //     }
  //     //     // Now, try to make a fit of this vector:
  //     //     Status exitStatus = kNStatus;
  //     //     int n_iterations = 0;
  //     //     errorVector.at(vecIndex) = optimzeVector(vec, exitStatus, n_iterations, tempSlopes, tempPlane);
  //     //     convergeStatus.at(vecIndex) = exitStatus;
  //     //     convergeNumber.at(vecIndex) = n_iterations;
  //     //     // Print out what happened:
  //     //     // Print out info about this vector:
  //     //     std::cout << "Error: " << errorVector.at(vecIndex)
  //     //               << "\tVec: (" << vec.X() << ", "
  //     //               << vec.Y() << ", "
  //     //               << vec.Z() << ")"
  //     //               << "\tStatus: ";
  //     //     if (convergeStatus.at(vecIndex) == kNormal)
  //     //       std::cout << "normal";
  //     //     if (convergeStatus.at(vecIndex) == kIterMaxOut)
  //     //       std::cout << "maxout";
  //     //     if (convergeStatus.at(vecIndex) == kNotOptFit)
  //     //       std::cout << "nonopt";
  //     //     std::cout << "\tN: " << convergeNumber.at(vecIndex);
  //     //     std::cout << std::endl;
  //     //     for (unsigned int p = 0; p < tempPlane.size(); p++) {
  //     //       float slope = geomHelper -> Slope_3Dto2D(vec, tempPlane[p]);
  //     //       std::cout << "\t" << p << " - \tgoal:\t" << tempSlopes.at(p) << "\tact:\t" << slope << "\n";
  //     //     }
  //     //   }
  //     // }
  //   }
  //   vecIndex ++;
  // }


  resultShower.fXYZStart = pointOnAxis;


  if (_verbose) {
    std::cout << "FINAL RESULT: \n";

    std::cout
    // << "True vector: ("
    // << knownAxis.X() << ", "
    // << knownAxis.Y() << ", "
    // << knownAxis.Z() << "), "
        << "Calculated vector: ("
        << resultShower.fDCosStart.X() << ", "
        << resultShower.fDCosStart.Y() << ", "
        << resultShower.fDCosStart.Z() << ").\n";
    // << " Difference is " << acos(knownAxis.Dot(resultShower.fDCosStart)) << " radians.\n";

    for (unsigned int i = 0; i < inputShowers.size(); i++) {
      larutil::PxPoint start2D, dir2D;
      geomHelper -> Line_3Dto2D(pointOnAxis, resultShower.fDCosStart, planes[i], start2D, dir2D);
      std::cout << "\tIn plane " << planes[i] << ", the projection is "
                << (dir2D.t / dir2D.w) << ", the target is "
                << slopeByPlane[i] << ".\n";
    }
  }

  return;

}

float Axis3DModule::optimzeVector(TVector3 & inputVector,
                                  Status & exitStatus,
                                  int & n_iterations,
                                  const std::vector<float> & slopeByPlane,
                                  const std::vector<int> & planes )
{

  // Some variables needed for this loop:
  n_iterations = 0;



  float nStepsMax = 4 * _nStepsStart;

  float current_error = getErrorOfProjection(inputVector, slopeByPlane, planes);

  // For each seed vector, compare the error of it vs. it's neighbors.
  // If the error at a neighbor is better, move to that.

  float thetaRange = _thetaRangeStart;
  float nSteps = _nStepsStart;


  while ( n_iterations < fMaxIterations ) {
    std::vector<TVector3> neighborVectors;
    // Make the list of the neighbor vectors:
    generateNearbyVectors(inputVector, thetaRange, nSteps, neighborVectors);
    // Find out which neighbor vector is the best:
    int bestNeighborIndex = findBestVector(neighborVectors, slopeByPlane, planes);
    // Compare the error of the best neighbor to the error of the inputVector:
    float error = getErrorOfProjection(neighborVectors[bestNeighborIndex], slopeByPlane, planes);

    // Now we have the best neighbor vector.
    // If things improved, just make the new vector the seed and iterate.
    //
    // If things didn't improve, make thetaRange and nSteps bigger and try again.
    //
    // If things didn't improve and thetaRange or nSteps are maxed out, bail.

    if (error < current_error) {
      inputVector = neighborVectors[bestNeighborIndex];
      current_error = error;
      // If there was improvement, make sure that nSteps is
      // set to its min value:
      nSteps = _nStepsStart;
      // For theta range, since that is the convergence speed, set it to
      // the max of it's min value and _alpha*currentValue:
      thetaRange = fmin(_thetaRangeStart, _alpha * thetaRange);
      if (thetaRange < _thetaRangeMin) thetaRange = _thetaRangeMin;
    }
    // Else things didn't improve
    else {
      // This step didn't help. Try to increase the granularity first:
      if (nSteps < nStepsMax) {
        nSteps += _nStepsStart;
      }
      // If that fails, perhaps need to look at smaller angles:
      else if (thetaRange > _thetaRangeMin) {
        thetaRange *= _alpha;
      }
      else {
        // Both nSteps and theta range are maxed out.  Bail on this point.
        // std::cout << "bailing because both maxed out.\n";
        exitStatus = kNotOptFit;
        return current_error;
      }
    }
    // Check if we've reached the desired current_error:
    if (current_error < fTargetError) {
      exitStatus = kNormal;
      return current_error;
    }

    n_iterations ++;
  }

  // The only way to make it out of that loop without returning is n_iterations > fMaxIterations
  exitStatus = kIterMaxOut;
  return current_error;

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
    seedVector.Rotate( (M_PI / nSteps) * (j), workerVector);
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
      seedVector.Rotate( (M_PI / nSteps) * (j), workerVector);
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
