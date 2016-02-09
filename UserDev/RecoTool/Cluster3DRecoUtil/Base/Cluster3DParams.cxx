#ifndef CLUSTER3DPARAMS_CXX
#define CLUSTER3DPARAMS_CXX

#include "Cluster3DParams.h"
#include <sstream>

namespace cluster3D {


int cluster3D_params::SetPoints(const std::vector< Point3D> & inputPoints) {

  Clear();

  point_vector = inputPoints;
  return point_vector.size();

}

void cluster3D_params::Clear() {

  // Clear the points
  point_vector.clear();

  start_point.x = 0.0; start_point.y = 0.0; start_point.z = 0.0;
  end_point.x = 0.0; end_point.y = 0.0; end_point.z = 0.0;
  showering_point.x = 0.0; showering_point.y = 0.0; showering_point.z = 0.0;

  start_point_cand.clear(); start_point_cand.reserve(5);
  shwr_point_cand.clear(); shwr_point_cand.reserve(5);
  start_dir_cand.clear(); start_dir_cand.reserve(5);


  mean_x                            = kDOUBLE_MIN ;
  mean_y                            = kDOUBLE_MIN ;
  mean_z                            = kDOUBLE_MIN ;
  rms_x                             = kDOUBLE_MIN ;
  rms_y                             = kDOUBLE_MIN ;
  rms_z                             = kDOUBLE_MIN ;

  opening_angle                     = kDOUBLE_MIN ;
  closing_angle                     = kDOUBLE_MIN ;

  eigenvalue_principal              = kDOUBLE_MIN ;
  eigenvalue_secondary              = kDOUBLE_MIN ;
  eigenvalue_tertiary               = kDOUBLE_MIN ;

  principal_dir.Clear();
  secondary_dir.Clear();
  tertiary_dir.Clear();

  start_dir.Clear();

  length                            = kDOUBLE_MIN ;
  width_secondary                   = kDOUBLE_MIN ;
  width_tertiary                    = kDOUBLE_MIN ;

  N_points                          = kDOUBLE_MIN ;
}

void cluster3D_params::Report(std::ostream & os) const {

  os << "ClusterParams report: \n"
     << "   start_point  .............. : " << point3DPrint(start_point) << "\n"
     << "   end_point  ................ : " << point3DPrint(end_point) << "\n"
     << "   showering_point  .......... : " << point3DPrint(showering_point) << "\n"

     << "   mean_x  ................... : " << mean_x << "\n"
     << "   mean_y  ................... : " << mean_y << "\n"
     << "   mean_z  ................... : " << mean_z << "\n"
     << "   rms_x  .................... : " << rms_x << "\n"
     << "   rms_y  .................... : " << rms_y << "\n"
     << "   rms_z  .................... : " << rms_z << "\n"

     << "   opening_angle  ............ : " << opening_angle << "\n"
     << "   closing_angle  ............ : " << closing_angle << "\n"
     << "   eigenvalue_principal  ..... : " << eigenvalue_principal << "\n"
     << "   eigenvalue_secondary  ..... : " << eigenvalue_secondary << "\n"
     << "   eigenvalue_tertiary  ...... : " << eigenvalue_tertiary  << "\n"

     << "   principal_dir  ............ : " << vectorPrint(principal_dir) << "\n"
     << "   secondary_dir  ............ : " << vectorPrint(secondary_dir) << "\n"
     << "   tertiary_dir  ............. : " << vectorPrint(tertiary_dir) << "\n"

     << "   length  ................... : " << length << "\n"
     << "   width_secondary  .......... : " << width_secondary << "\n"
     << "   width_tertiary  ........... : " << width_tertiary << "\n"

     << "   N_points  ................... : " << N_points << "\n";




}
void cluster3D_params::ReportDiff(const cluster3D_params & otherCluster) {

  if (this -> start_point != otherCluster.start_point  ) {
    std::cout << "   start_point  .............. : "
              << point3DPrint(start_point) << " vs. "
              << point3DPrint(otherCluster.start_point) << "\n";
  }
  if (this -> showering_point != otherCluster.showering_point  ) {
    std::cout << "   showering_point  .......... : "
              << point3DPrint(showering_point) << " vs. "
              << point3DPrint(otherCluster.showering_point) << "\n";
  }
  if (this -> end_point != otherCluster.end_point  ) {
    std::cout << "   end_point  ................ : "
              << point3DPrint(end_point) << " vs. "
              << point3DPrint(otherCluster.end_point) << "\n";
  }



  if (this -> mean_x                   != otherCluster.mean_x) {
    std::cout << "   mean_x  ................... : "
              << this -> mean_x << " vs. "
              << otherCluster.mean_x << "\n";
  }
  if (this -> mean_y                   != otherCluster.mean_y) {
    std::cout << "   mean_y  ................... : "
              << this -> mean_y << " vs. "
              << otherCluster.mean_y << "\n";
  }
  if (this -> mean_z                   != otherCluster.mean_z) {
    std::cout << "   mean_z  ................... : "
              << this -> mean_z << " vs. "
              << otherCluster.mean_z << "\n";
  }
  if (this -> rms_x                    != otherCluster.rms_x) {
    std::cout << "   rms_x  .................... : "
              << this -> rms_x << " vs. "
              << otherCluster.rms_x << "\n";
  }
  if (this -> rms_y                    != otherCluster.rms_y) {
    std::cout << "   rms_y  .................... : "
              << this -> rms_y << " vs. "
              << otherCluster.rms_y << "\n";
  }
  if (this -> rms_z                    != otherCluster.rms_z) {
    std::cout << "   rms_z  .................... : "
              << this -> rms_z << " vs. "
              << otherCluster.rms_z << "\n";
  }



  if (this -> opening_angle            != otherCluster.opening_angle) {
    std::cout << "   opening_angle  ............ : "
              << this -> opening_angle << " vs. "
              << otherCluster.opening_angle << "\n";
  }

  if (this -> closing_angle            != otherCluster.closing_angle) {
    std::cout << "   closing_angle  ............ : "
              << this -> closing_angle << " vs. "
              << otherCluster.closing_angle << "\n";
  }


  if (this -> eigenvalue_principal     != otherCluster.eigenvalue_principal) {
    std::cout << "   eigenvalue_principal  ..... : "
              << this -> eigenvalue_principal << " vs. "
              << otherCluster.eigenvalue_principal << "\n";
  }
  if (this -> eigenvalue_secondary     != otherCluster.eigenvalue_secondary) {
    std::cout << "   eigenvalue_secondary  ..... : "
              << this -> eigenvalue_secondary << " vs. "
              << otherCluster.eigenvalue_secondary << "\n";
  }
  if (this -> eigenvalue_tertiary     != otherCluster.eigenvalue_tertiary) {
    std::cout << "   eigenvalue_tertiary  ...... : "
              << this -> eigenvalue_tertiary << " vs. "
              << otherCluster.eigenvalue_tertiary << "\n";
  }



  if (this -> principal_dir != otherCluster.principal_dir ) {
    std::cout << "   principal_dir  ................ : "
              << vectorPrint(principal_dir) << " vs. "
              << vectorPrint(otherCluster.principal_dir) << "\n";
  }
  if (this -> secondary_dir != otherCluster.secondary_dir ) {
    std::cout << "   secondary_dir  ................ : "
              << vectorPrint(secondary_dir) << " vs. "
              << vectorPrint(otherCluster.secondary_dir) << "\n";
  }
  if (this -> tertiary_dir != otherCluster.tertiary_dir ) {
    std::cout << "   tertiary_dir  ................ : "
              << vectorPrint(tertiary_dir) << " vs. "
              << vectorPrint(otherCluster.tertiary_dir) << "\n";
  }
  if (this -> start_dir != otherCluster.start_dir ) {
    std::cout << "   start_dir  ................ : "
              << vectorPrint(start_dir) << " vs. "
              << vectorPrint(otherCluster.start_dir) << "\n";
  }



  if (this -> length != otherCluster.length) {
    std::cout << "   length  ................... : "
              << this -> length << " vs. "
              << otherCluster.length << "\n";
  }
  if (this -> width_secondary != otherCluster.width_secondary) {
    std::cout << "   width_secondary  .......... : "
              << this -> width_secondary << " vs. "
              << otherCluster.width_secondary << "\n";
  }
  if (this -> width_tertiary != otherCluster.width_tertiary) {
    std::cout << "   width_tertiary  ........... : "
              << this -> width_tertiary << " vs. "
              << otherCluster.width_tertiary << "\n";
  }



  if (this -> N_points                   != otherCluster.N_points) {
    std::cout << "   N_points  ................... : "
              << this -> N_points << " vs. "
              << otherCluster.N_points << "\n";
  }

}


std::string cluster3D_params::vectorPrint(const TVector3 & vec) const {
  std::ostringstream ss;
  ss << "(" << vec.X() << ", "  << vec.Y() << ", " << vec.Z() << ")";
  return ss.str();
}
std::string cluster3D_params::point3DPrint(const Point3D & point) const {
  std::ostringstream ss;
  ss << "(" << point.X() << ", "  << point.Y() << ", " << point.Z() << ")";
  return ss.str();
}



} // cluster

#endif
