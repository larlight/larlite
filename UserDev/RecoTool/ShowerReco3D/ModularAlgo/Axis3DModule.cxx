#ifndef AXIS3DMODULE_CXX
#define AXIS3DMODULE_CXX

#include <iomanip>

#include "Axis3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"


namespace showerreco {

void Axis3DModule::initialize() {

  if (_tree) delete _tree;
  _tree = new TTree(_name.c_str(), "Axis3D info tree");
  _tree->Branch("_vec_exists",   &_vec_exists,   "_vec_exists/B");
  _tree->Branch("_vec_error",    &_vec_error,    "_vec_error/F");
  _tree->Branch("_vec_status",   &_vec_status,   "_vec_status/I");
  _tree->Branch("_vec_x",        &_vec_x,        "_vec_x/F");
  _tree->Branch("_vec_y",        &_vec_y,        "_vec_y/F");
  _tree->Branch("_vec_z",        &_vec_z,        "_vec_z/F");
  _tree->Branch("_slope_0",      &_slope_0,      "_slope_0/F");
  _tree->Branch("_slope_1",      &_slope_1,      "_slope_1/F");
  _tree->Branch("_slope_2",      &_slope_2,      "_slope_2/F");
  _tree->Branch("_slope_0_true", &_slope_0_true, "_slope_0_true/F");
  _tree->Branch("_slope_1_true", &_slope_1_true, "_slope_1_true/F");
  _tree->Branch("_slope_2_true", &_slope_2_true, "_slope_2_true/F");
}

void Axis3DModule::do_reconstruction(const ProtoShower & proto_shower,
                                     Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower.


  // std::cout << "Got here 1\n";

  //auto geom = larutil::Geometry::GetME();
  auto geomHelper = larutil::GeometryHelper::GetME();


  // Start by making a guess:
  TVector3 direction(0, 0, 1.0);
  resultShower.fDCosStart = TVector3(0, 0, 0);


  // Determine the shower projections in 2D by plane (slopes):
  std::vector<float> slopeByPlane;
  std::vector<int> planes;

  // ///########################################################
  // // For algorithm development:
  // ///########################################################
  // // Start with a *known* 3D axis and project that into the plane:
  // TVector3 knownAxis(0.5, 0.5, 0.5);
  // knownAxis *= 1.0 / knownAxis.Mag();

  // for (unsigned int i = 0; i < clusters.size(); i++) {
  //   planes.push_back(clusters.at(i).plane_id.Plane);
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
  for (unsigned int i = 0; i < clusters.size(); i++) {
    planes.push_back(clusters.at(i).plane_id.Plane);
    // float slope = clusters.at(i).angle_2d;
    float slope = 0;
    if (clusters.at(i).start_dir[0] != 0) {
      // std::cout << "Using (" << clusters.at(i).start_dir[0] << ", " << clusters.at(i).start_dir[1] << ")\n";
      slope = clusters.at(i).start_dir[1] / clusters.at(i).start_dir[0];
    }
    if (slope != 0) goodSlopes ++;
    // std::cout << "Slope is " << slope << std::endl;
    slopeByPlane.push_back(slope);
  }

  if (goodSlopes < 2) {
    return;
  }


  // Try to determine the 3D axis iteratively.
  // Here's how it is, I have NO IDEA how to rotate things to fix things.  So instead, just
  // make a bunch of guesses and narrow in on the result.



  if (_globalSeedVectors.size() == 0)
    generateSeedVectors(direction, M_PI / 2.0, fNStepsInitial, _globalSeedVectors);

  // maintain a list of the good seeds and the errors of the seed vectors too:
  std::vector<SeedVector> seedVectors;

  // Keep the best vector with the best error;
  SeedVector bestVector;

  findSeedVectors(seedVectors, planes, slopeByPlane);
  optimizeSeedVectors(seedVectors, planes, slopeByPlane);
  printVectors(seedVectors, planes, slopeByPlane);
  bestVector = findOptimalVector(seedVectors);

  // Set the best vector to direction:
  if (bestVector._exists)
    resultShower.fDCosStart = bestVector._vector;
  else {
    // In this case, none of the seeds converged.  Maxout seed vectors aren't useful, but
    // the nonopt ones are.  We can try removing each plane and refitting all the vectors
    // that were returned to see if they converge.

    // Loop over all possible pair of planes.

    if (_verbose) {
      std::cout << "Running alternate axis finding..." << std::endl;
    }

    std::vector<SeedVector> bestVectorAlt;

    for (unsigned int i = 0; i < planes.size(); i++) {

      // maintain a list of the good seeds and the errors of the seed vectors too:
      std::vector<SeedVector> seedVectorsAlt;

      SeedVector bestVectorTemp;

      std::vector<float> slopeByPlaneAlt;
      std::vector<int> planesAlt;

      for (unsigned int j = 0; j < planes.size(); j++) {
        if (i == j) continue;
        else {
          planesAlt.push_back(planes.at(j));
          slopeByPlaneAlt.push_back(slopeByPlane.at(j));
        }
      }

      if (_verbose) {
        std::cout << "Omitting plane " << planes.at(i) << std::endl;
      }

      // find any potential seed vector candidates, optimise them & print them. then find the best.
      findSeedVectors(seedVectorsAlt, planesAlt, slopeByPlaneAlt);
      optimizeSeedVectors(seedVectorsAlt, planesAlt, slopeByPlaneAlt);
      printVectors(seedVectorsAlt, planesAlt, slopeByPlaneAlt);
      bestVectorTemp = findOptimalVector(seedVectorsAlt);
      bestVectorAlt.push_back(bestVectorTemp);
    }

    if (_verbose) {
      std::cout << "SUMMARY OF ALTERNATIVE AXES:" << std::endl;
      int i = 0;
      for (auto & vec : bestVectorAlt) {
        std::cout << "Best vector omitting plane " << i << ":"
                  << "  Error " << vec._error << " with vector (" << vec._vector.X() << ", "
                  << vec._vector.Y() << ", " << vec._vector.Z() << ")" << std::endl;
        i++;
      }
    }
    // Make a selection from bestVectorAlt
    // Easy easy way: figure out if one of the start points is much farther from the others in time.
    // This most often happens if a shower start point is backwards
    std::vector<float> startTimeDiffs;

    // The following code should only run if we have clusters in all 3 planes -- jeremy
    if (planes.size() > 2) {
      for (size_t i = 0; i < planes.size(); i++) {
        int point1, point2;
        if (i == 0) {
          point1 = 1;
          point2 = 2;
        }
        else if ( i == 1) {
          point1 = 0;
          point2 = 2;
        }
        else {
          point1 = 0;
          point2 = 1;
        }
        startTimeDiffs.push_back(clusters.at(point1).start_point.t - clusters.at(point2).start_point.t);
      }
    }
    // Figure out is one is much smaller than the other two.  If so, keep that pair.
    float minVal(99999), nextMin(99999);
    size_t min_index = -1;
    size_t i = 0;
    for (auto & val : startTimeDiffs) {
      if (fabs(val) < minVal) {
        nextMin = minVal;
        minVal = fabs(val);
        min_index = i;
      }
      i++;
    }
    if (i != -1 && 3 * minVal < nextMin && bestVectorAlt.at(min_index)._exists) {
      // In this case, there is clearly a shower with a bad start point
      // Pick the vector that ignores it.
      bestVector = bestVectorAlt.at(min_index);
      bestVector._status = kTwoPlane;
      resultShower.fDCosStart = bestVectorAlt.at(min_index)._vector;
      resultShower.fPlaneIsBad.at(min_index) = true;
    }

    else {
      // give up, just average it ...
      resultShower.fDCosStart.SetX(0.0);
      resultShower.fDCosStart.SetY(0.0);
      resultShower.fDCosStart.SetZ(0.0);

      bool averaged = false;
      for (auto & vec : bestVectorAlt) {
        if (vec._exists) {
          averaged = true;
          resultShower.fDCosStart += vec._vector;
        }
      }
      if (averaged == true) {
        resultShower.fDCosStart *= 1.0 / resultShower.fDCosStart.Mag();
        bestVector._vector = resultShower.fDCosStart;
        bestVector._status = kAveraged;
        bestVector._exists = true;
      }
    }

    // and if that doesn't work, just get the best candidate, even if it's a bad one
    if (!bestVector._exists) {
      bestVector._status = kNotOptFit;
      float error = 99999;
      for (auto & vec : seedVectors) {
        vec._error = getErrorOfProjection(vec._vector, slopeByPlane, planes);
        if (vec._error < error)
          bestVector = vec;
      }
      resultShower.fDCosStart = bestVector._vector;
    }

  } // alternate fitting options


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

    for (unsigned int i = 0; i < clusters.size(); i++) {
      larutil::PxPoint start2D, dir2D;
      float slope = geomHelper -> Slope_3Dto2D(resultShower.fDCosStart, planes[i]);
      std::cout << "\tIn plane " << planes[i] << ", the projection is "
                << slope << ", the target is "
                << slopeByPlane[i] << ".\n";
    }
  }

  /*
  // Save info on best vector to tree
  _vec_exists = bestVector._exists;
  _vec_error  = (bestVector._error == 99999) ? -1 : bestVector._error;
  _vec_status = bestVector._status;
  _vec_x      = bestVector._vector.X();
  _vec_y      = bestVector._vector.Y();
  _vec_z      = bestVector._vector.Z();
  _slope_0    = geomHelper -> Slope_3Dto2D(bestVector._vector, planes[0]);
  _slope_1    = geomHelper -> Slope_3Dto2D(bestVector._vector, planes[1]);
  _slope_2    = geomHelper -> Slope_3Dto2D(bestVector._vector, planes[2]);
  _slope_0_true = slopeByPlane[0];
  _slope_1_true = slopeByPlane[1];
  _slope_2_true = slopeByPlane[2];
  _tree->Fill();
  */

  return;

}

// ok i've been staring at code too long, here's a playlist
// feel free to delete this if you find it

//                                  MAYFLOWER DRIVE                                   //
// 1.                                  ooo -- karen o                                 //
// 2.                             the move -- laura stevenson                         //
// 3.                        i saw my twin -- hop along                               //
// 4.                  phones and machines -- b. fleischmann                          //
// 5.                         closing time -- tom waits                               //
// 6.                           nighthawks -- erik friedlander                        //
// 7.                       sunday morning -- noname gypsy                            //
// 8.                           levitation -- beach house                             //
// 9.                              nervous -- wavves x cloud nothings                 //
// 10.                                 car -- built to spill                          //
// 11. the rabbit, the bat, & the reindeer -- dr. dog                                 //
// 12.                          ethiopians -- bomb the music industry!                //

float Axis3DModule::optimizeVector(TVector3 & inputVector,
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
        // Determine whether this fit reached the acceptable range or not:
        if (current_error < _normalErrorRange)
          exitStatus = kNormal;
        else
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

void Axis3DModule::findSeedVectors(std::vector<SeedVector> & seedVectors,
                                   const std::vector<int> & planes,
                                   const std::vector<float> & slopeByPlane)
{
  float errorCutoff = _seedVectorErrorCutoff;

  // Find out the seed vectors with reasonable error, and reject all the others
  while (seedVectors.size() == 0) {
    for (auto & vec : _globalSeedVectors) {
      float error = getErrorOfProjection(vec, slopeByPlane, planes);
      if (error < errorCutoff) {
        seedVectors.push_back(SeedVector(vec, error));
      }
    }
    // If no seed vectors were found, relax the cutoff and try again
    errorCutoff += _seedVectorErrorCutoff;
  }
}

void Axis3DModule::optimizeSeedVectors(std::vector<SeedVector> & seedVectors,
                                       const std::vector<int> & planes,
                                       const std::vector<float> & slopeByPlane)
{
  int n_converged(0);
  // Loop over each candidate vector
  for (auto & vec : seedVectors) {
    Status exitStatus = kNStatus;
    int n_iterations = 0;
    // Optimize the vector
    vec._error = optimizeVector(vec._vector, exitStatus, n_iterations, slopeByPlane, planes);
    vec._status = exitStatus;
    vec._iterations = n_iterations;
    if (exitStatus == kNormal)
      n_converged++;
  }
}

void Axis3DModule::printVectors(std::vector<SeedVector> & seedVectors,
                                const std::vector<int> & planes,
                                const std::vector<float> & slopeByPlane)
{
  auto geomHelper = larutil::GeometryHelper::GetME();
  // Print out the final vectors and their errors:
  for (auto & vec : seedVectors) {
    // Print out info about this vector:
    if (_verbose) {
      std::cout << "Error: " << vec._error
                << "\tVec: (" << vec._vector.X() << ", "
                << vec._vector.Y() << ", "
                << vec._vector.Z() << ")"
                << "\tStatus: ";
      if (vec._status == kNormal)
        std::cout << "normal";
      if (vec._status == kIterMaxOut)
        std::cout << "maxout";
      if (vec._status == kNotOptFit)
        std::cout << "nonopt";
      std::cout << "\tN: " << vec._iterations;
      std::cout << std::endl;
      for (unsigned int p = 0; p < planes.size(); p++) {
        float slope = geomHelper -> Slope_3Dto2D(vec._vector, planes[p]);
        std::cout << "\t" << p << " - \tgoal:\t" << slopeByPlane.at(p) << "\tact:\t" << slope << "\n";
      }
    }
  }
}

SeedVector Axis3DModule::findOptimalVector(std::vector<SeedVector> & seedVectors)
{
  SeedVector bestVector;
  bestVector._error = 99999;
  for (auto & vec : seedVectors) {
    if (vec._status == kNormal ) {
      if (vec._error < bestVector._error) {
        bestVector = vec;
      }
    }
  }
  return bestVector;
}

} //showerreco

#endif
