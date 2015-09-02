#ifndef AXIS3DMODULE_CXX
#define AXIS3DMODULE_CXX

#include <iomanip>

#include "Axis3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

#define PI           3.14159265358979323846  /* pi */

namespace showerreco{

  void Axis3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    
    // This function takes the shower cluster set and computes the best fit 3D axis
    // and then assigns it to the shower.

    // std::cout << "Got here 1\n";

    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();


    // Start by making a guess:
    TVector3 pointOnAxis,direction(0,0,1.0);


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
    // TVector3 knownAxis(17,8.0,-6.0);
    // knownAxis *= 1.0 / knownAxis.Mag();

    // for (unsigned int i = 0; i < inputShowers.size();i++){
    //   planes.push_back(inputShowers.at(i).plane_id.Plane);
    //   float slope = geomHelper -> Slope_3Dto2D(knownAxis,planes[i]);
    //   slopeByPlane.push_back(slope);
    //   std::cout << "Pushed back slope == " << slopeByPlane.back() << std::endl;
    //   planes.push_back(inputShowers.at(i).plane_id.Plane);
    // }

    // ///########################################################
    // ///End development section
    // ///########################################################


    ///########################################################
    ///For real
    ///########################################################
    // for (unsigned int i = 0; i < inputShowers.size();i++){
    //   planes.push_back(inputShowers.at(i).plane_id.Plane);
    //   float slope = inputShowers.at(i).angle_2d;
    //   std::cout << "Slope is " << slope << " with strength " << inputShowers.at(i).eigenvalue_principal << std::endl;
    //   slopeByPlane.push_back(slope);
    // }

    // Try to determine the 3D axis iteratively.
    // Here's how it is, I have NO IDEA how to rotate things to fix things.  So instead, just
    // make a bunch of guesses and narrow in on the result.

    bool converged = false;
    int current_iteration = 0; 

    float thetaRange = PI / 2.0;
    int nSteps_current = fNStepsMin;

    float prevError = 99999;

    while ( ! converged ){
      // Get a set of seed vectors:
      std::vector<TVector3> seedVectors;
      generateSeedVectors(direction, thetaRange, nSteps_current, seedVectors);
      int min_index = findBestVector(seedVectors, slopeByPlane,planes);
      direction = seedVectors.at(min_index);
      // Print out an update:
      float error = getErrorOfProjection(direction,slopeByPlane,planes);
      // std::cout << "Current vector: ("
      //           << direction.X() << ", "
      //           << direction.Y() << ", "
      //           << direction.Z() << ")., error: "
      //           << error
      //           << ", thetaRange: " << thetaRange 
      //           << ", steps: " << nSteps_current
      //           << std::endl;

      // Only reduce the range of theta if the error is getting good:
      if (error < 1 ){
        // Narrow down if we're getting better
        if (error < prevError){
          thetaRange *= 0.75;
          nSteps_current = fNStepsMin;
        }
        // Go finer if it's good but not getting better
        else
          nSteps_current += fNStepsMin;
      }
      // Otherwise, go for finer sampling to find a good spot:
      else{
        nSteps_current += 4*fNStepsMin;
      }

      prevError = error;

      error /= inputShowers.size();

      if (error < fTargetError) converged = true;
      current_iteration ++;
      if (current_iteration > fMaxIterations) break;
    }


    // std::cout << "FINAL RESULT: \n";

    // std::cout 
    //           // << "True vector: ("
    //           // << knownAxis.X() << ", "
    //           // << knownAxis.Y() << ", "
    //           // << knownAxis.Z() << "), "
    //           << "Calculated vector: ("
    //           << direction.X() << ", "
    //           << direction.Y() << ", "
    //           << direction.Z() << ").\n";
    //           // << " Difference is " << acos(knownAxis.Dot(direction)) << " radians.\n";

    // for (unsigned int i = 0; i < inputShowers.size();i++){
    //   larutil::PxPoint start2D, dir2D;
    //   geomHelper -> Line_3Dto2D(pointOnAxis, direction, planes[i], start2D, dir2D);
    //   std::cout << "\tIn plane " << planes[i] << ", the projection is "
    //             << atan(dir2D.t / dir2D.w) << ", the target is "
    //             << slopeByPlane[i] << ".\n";
    // }

    // Compare the 3D vector and the *known* 3D vector:
    
    resultShower.fDCosStart = direction;
    resultShower.fXYZStart = pointOnAxis;


    return;

  }


    void Axis3DModule::generateSeedVectors(const TVector3 & initialVector, 
                                                                    float thetaRange, 
                                                                    int nSteps,
                                                                    std::vector<TVector3> & result)
    {

      // Initialize the result
      result.clear();
      result.reserve(nSteps*nSteps);

      // Do some safety checks:
      if (initialVector.Mag() == 0){
        std::cerr << "ERROR - Axis3DModule::generateSeedVectors: can not have initial vector be zero!\n";
        return;
      }
      if (nSteps == 0){
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
      TVector3 perp(workerVector.X() - 1,workerVector.Y() + 1,workerVector.Z()+2);
      perp = perp.Cross(workerVector);
      perp *= 1.0 / perp.Mag();

      for (float i = 1.0; i <= nSteps; i++){
        TVector3 seedVector = workerVector;
        // Rotate up a fraction of the 90 degrees needed:
        seedVector.Rotate( (thetaRange / nSteps) * (i), perp );
        // Rotate around the center vector a full rotation
        for (float j = 0; j <= nSteps; j++){
          seedVector.Rotate( 2*PI * (j / nSteps), workerVector);
          result.push_back(seedVector);
        }
      }
      return;

    }
    int Axis3DModule::findBestVector( const std::vector<TVector3> seedVectors, 
                                      const std::vector<float> & slopesByPlane, 
                                      const std::vector<int> & planes){

      // Loop over the vectors, calculate the error, and return the one with smallest error
      std::vector<float> errs;
      errs.reserve(seedVectors.size());
      for (auto & vec : seedVectors){
        errs.push_back(getErrorOfProjection(vec,slopesByPlane,planes));
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
      for (unsigned int i = 0; i < slopesByPlane.size(); i++){
        // Get the 2D slope for this plane, and compute the error as squared difference
        float slope = atan(geomHelper -> Slope_3Dto2D(inputVector,planes[i]));
        float error = (slope - slopesByPlane[i]);
        // float error = (slope - slopesByPlane[i])/(slopesByPlane[i]);
        // float error = 1 - fabs(slope / slopesByPlane[i]);
        // Weight this error by the inverse value of the target slope (so that higher slopes don't get higher weight)
        err += fabs(error);
        // wgt += 1.0 / slopesByPlane[i];
      }



      return err;

    }




} //showerreco

#endif
