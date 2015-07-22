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


    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();


    // Start by making a guess:
    TVector3 pointOnAxis,direction(0,0,1);

    ///\todo temporary: set the minimum error hardcoded:
    float _error_minimum = 1.0;  // [units are cm, averaged over all showers]

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


    std::cout << "Detector dimensions: ( "
              << 2* geom -> DetHalfWidth() << ", "
              << 2* geom -> DetHalfHeight() << ", "
              << geom -> DetLength() << ")\n";

    std::cout << "\n\nStarting guess is ("
              << pointOnAxis.X() << ", "
              << pointOnAxis.Y() << ", "
              << pointOnAxis.Z() << ")\n";

    bool converged = false;
    int max_iterations = 5;
    // int max_iterations = 500;
    int current_iteration = 0; 
    
    int nPoints = 100;
    

    // Another attempt:  Try to find a line that is most parallel to the clusters.
    // Make an educated guess: Take a cluster, use it's 2D angle as the angle between X and Z,
    // Set the angle between Y and Z to 0 to start.

    direction.SetMag(1);
    // Theta is the angle away from the Z axis.  So, if there is no angle between Y and Z, theta is the 2D angle
    // Be careful with the "edges" - if w is negative and t is positive, that's theta between pi/2 and pi
    // If w is positive and t is negative, that's theta between 0 and pi/2 BUT phi is set to pi instead of 0

    float w = inputShowers.front().principal_dir[0];
    float t = inputShowers.front().principal_dir[1];

    if (w > 0 && t > 0){
      // Positive slope in 2D, so pointing away from wires
      direction.SetTheta(atan(t/w));
      direction.SetPhi(0.0);
    }
    if (w > 0 && t < 0){
      float theta = atan(t/w);
      // That value is negative, but we want the positive value
      theta = fabs(theta);
      direction.SetTheta(theta);
      direction.SetPhi(PI);
    }
    if ( w < 0 && t < 0){
      // Here, the direction is towards the wire planes but at a high angle
      direction.SetTheta(atan(t/w));
      direction.SetPhi(PI);
    }
    if ( w < 0 && t > 0){
      float theta = atan(t/w);
      // Again, that's negative.  Fix by subtracting from pi:
      theta = PI - theta;
      direction.SetTheta(theta);
      direction.SetPhi(0.0);
    }



    // Try to determine the 3D axis iteratively.
    // Some notes:
    //   -> If the projected slope in a plane is higher than the actual slope (using absolute values), that means either
    //      a) The 3D angle is too steep in the X-Z plane (that is, pointing away from the wires too aggressively)
    //      b) The 3D angle needs to be rotated towards the direction that would make it parallel with the wires

    // Changes to the direction vector are most simply accomplished with rotations around N + 1 overlapping
    // but complete axes:
    //  1) Rotations that increase or decrease X direction (this is a rotation around the Y axis)
    //  2) Rotations that bring a vector closer or farther from being parallel to a particular wire plane
    //    a)  This is a rotation around the axis that is perpendicular to the wire plane AND perpendicular to 
    //        the wires themselves

    TVector3 xAxis (1.0, 0.0, 0.0);
    TVector3 yAxis (0.0, 1.0, 0.0);
    TVector3 zAxis (0.0, 0.0, 1.0);
    // Construct the others by finding the wire's angle to vertical:
    std::vector<TVector3> wirePlaneRotationAxes;
    for (auto & shower : inputShowers){
      float angleToVert = geom -> WireAngleToVertical(geom -> PlaneToView(shower.plane_id.Plane)) * PI / 180.0;
      // Start along the z axis
      TVector3 planeVec(zAxis);
      // Rotate it up to the correct angle:
      planeVec.Rotate(angleToVert,xAxis);
      // Now we need the vector that is perpendicular to both this vector AND the Z axis
      planeVec = planeVec.Cross(zAxis);
      planeVec *= 1.0 / planeVec.Mag();
      wirePlaneRotationAxes.push_back(planeVec);
    }

    // Ok, start iterating over this 3D direction vector
    float rot_angle = 1.0;
    while (! converged )
    {

      std::cout << "Vector is ( "
                << direction.X() << ", "
                << direction.Y() << ", "
                << direction.Z() << ")\n\n";

      std::vector<float> w_diff, t_diff,slope_diff;
      for (auto & shower : inputShowers ){
        larutil::PxPoint start2D, dir2D;
        geomHelper -> Line_3Dto2D(pointOnAxis, direction, shower.plane_id.Plane, start2D, dir2D);
        w_diff.push_back(shower.principal_dir[0] - dir2D.w);
        t_diff.push_back(shower.principal_dir[1] - dir2D.t);
        slope_diff.push_back( (shower.principal_dir[1] / shower.principal_dir[0]) - (dir2D.t / dir2D.w) );
      }

      // Calculate the total, absolute error:
      float tot_error = 0.0;
      for (unsigned int i = 0; i < w_diff.size(); i++ ){
        tot_error += w_diff[i]*w_diff[i];
        tot_error += t_diff[i]*t_diff[i];
      }

      std::cout << " Diffs are: \nw\t\tt\t\ts\n";

      for (unsigned int i = 0; i < w_diff.size(); i++ ){
        std::cout << std::setw(3) << std::setprecision(3) << "" << w_diff[i] << "\t" << t_diff[i] << "\t\t" << slope_diff[i] <<  "\n";
      }
      std::cout << "Total error is: " << tot_error << std::endl;

      // Try to improve things ...
      // Use the plane that is most incorrect to fix things:
      int worst_plane = 0;
      float worst_val = 0.0;
      for (unsigned int i = 0; i < slope_diff.size(); i++){
        if (fabs(slope_diff.at(i)) > worst_val){
          worst_plane = i;
          worst_val = fabs(slope_diff.at(i));
        }
      }

      // The improvement can be made either by rotating towards or away from the wires OR up or down from Z
      // I don't know which to do, really, so try both and keep the one that improves things more

      TVector3 leftRight = direction;
      TVector3 upDown = direction;

      upDown.Rotate(rot_angle*atan(worst_val), xAxis);
      leftRight.Rotate(rot_angle*atan(worst_val), yAxis);
      // Rotate around the vector for this plane:
      // 

      std::vector<float> w_diff_post1, t_diff_post1,slope_diff_post1;
      std::vector<float> w_diff_post2, t_diff_post2,slope_diff_post2;
      for (auto & shower : inputShowers ){
        larutil::PxPoint start2D, dir2D;
        geomHelper -> Line_3Dto2D(pointOnAxis, leftRight, shower.plane_id.Plane, start2D, dir2D);
        w_diff_post1.push_back(shower.principal_dir[0] - dir2D.w);
        t_diff_post1.push_back(shower.principal_dir[1] - dir2D.t);
        slope_diff_post1.push_back( (shower.principal_dir[1] / shower.principal_dir[0]) - (dir2D.t / dir2D.w) );
        geomHelper -> Line_3Dto2D(pointOnAxis, upDown, shower.plane_id.Plane, start2D, dir2D);
        w_diff_post2.push_back(shower.principal_dir[0] - dir2D.w);
        t_diff_post2.push_back(shower.principal_dir[1] - dir2D.t);
        slope_diff_post2.push_back( (shower.principal_dir[1] / shower.principal_dir[0]) - (dir2D.t / dir2D.w) );
      }

      // Calculate the total, absolute error:
      float tot_error_post1 = 0.0, tot_error_post2 = 0.0 ;
      for (unsigned int i = 0; i < w_diff.size(); i++ ){
        tot_error_post1 += w_diff_post1[i]*w_diff_post1[i];
        tot_error_post1 += t_diff_post1[i]*t_diff_post1[i];
        tot_error_post2 += w_diff_post2[i]*w_diff_post2[i];
        tot_error_post2 += t_diff_post2[i]*t_diff_post2[i];
      }

      std::cout << "--------------\nPost Diffs are: \nw\t\tt\t\ts\n";

      for (unsigned int i = 0; i < w_diff_post1.size(); i++ ){
        std::cout << std::setw(3) << std::setprecision(3) << "" << w_diff_post1[i] << "\t" << t_diff_post1[i] << "\t\t" << slope_diff_post1[i] <<  "\n";
      }
      std::cout << "Total error is: " << tot_error_post1 << std::endl;
      std::cout << "Error Changed by " << tot_error_post1 - tot_error << "\n\n\n" << std::endl;
      for (unsigned int i = 0; i < w_diff_post2.size(); i++ ){
        std::cout << std::setw(3) << std::setprecision(3) << "" << w_diff_post2[i] << "\t" << t_diff_post2[i] << "\t\t" << slope_diff_post2[i] <<  "\n";
      }
      std::cout << "Total error is: " << tot_error_post2 << std::endl;
      std::cout << "Error Changed by " << tot_error_post2 - tot_error << "\n------------\n\n\n" << std::endl;

      if (tot_error_post1 < tot_error_post2 && tot_error_post1 < tot_error) direction = leftRight;
      else if (tot_error_post2 < tot_error) direction = upDown;
      
      else {
        // Didn't improve anything!  Just break
        std::cout << "Failed to improve!  breaking." << std::endl;
        break;
      }
      if (current_iteration > max_iterations) break;
      current_iteration ++;
      // converged = true;
    }
    for (auto & shower : inputShowers ){
      larutil::PxPoint start2D, dir2D;
      geomHelper -> Line_3Dto2D(pointOnAxis, direction, shower.plane_id.Plane, start2D, dir2D);
      std::cout << "FINAL RESULT: In plane " << shower.plane_id.Plane << ", the projection is "
                << "(" << dir2D.w << ", " << dir2D.t << "), the target is "
                << "(" << shower.principal_dir[0] << ", " << shower.principal_dir[1] << ").\n";
    }

    return;

  }



} //showerreco

#endif
