#ifndef CLUSTERPARAMS_CXX
#define CLUSTERPARAMS_CXX

#include "ClusterParams.h"

namespace cluster {


int cluster_params::SetHits(const std::vector< Hit2D> & inputHits) {

  Clear();

  plane_id.Plane = inputHits.front().plane;

  hit_vector = inputHits;

  return hit_vector.size();

}

void cluster_params::Clear() {
  start_point.Clear();
  end_point.Clear();
  showering_point.Clear();
  PolyObject.Clear();
  start_point_cand.clear(); start_point_cand.reserve(5);
  shwr_point_cand.clear(); shwr_point_cand.reserve(5);
  start_dir_cand.clear(); start_dir_cand.reserve(5);
  sum_charge                        = kDOUBLE_MIN ;
  mean_charge                       = kDOUBLE_MIN ;
  mean_x                            = kDOUBLE_MIN ;
  mean_y                            = kDOUBLE_MIN ;
  rms_x                             = kDOUBLE_MIN ;
  rms_y                             = kDOUBLE_MIN ;
  charge_wgt_x                      = kDOUBLE_MIN ;
  charge_wgt_y                      = kDOUBLE_MIN ;
  slope_2d_high_q                   = kDOUBLE_MIN ;
  slope_2d                          = kDOUBLE_MIN ;
  angle_2d                          = kDOUBLE_MIN ;
  start_dir.clear(); start_dir.resize(2);
  principal_dir.clear(); principal_dir.resize(2);
  eigenvalue_principal              = kDOUBLE_MIN ;
  eigenvalue_secondary              = kDOUBLE_MIN ;
  multi_hit_wires                   = kDOUBLE_MIN ;
  N_Wires                           = kDOUBLE_MIN ;
  verticalness                      = kDOUBLE_MIN ;
  opening_angle                     = kDOUBLE_MIN ;
  opening_angle_charge_wgt          = kDOUBLE_MIN ;
  closing_angle                     = kDOUBLE_MIN ;
  closing_angle_charge_wgt          = kDOUBLE_MIN ;
  offaxis_hits                      = kDOUBLE_MIN ;
  modmeancharge                     = kDOUBLE_MIN ;
  RMS_charge                        = kDOUBLE_MIN ;
  modified_hit_density              = kDOUBLE_MIN ;
  N_Hits                            = kDOUBLE_MIN ;
  N_Hits_HC                         = kDOUBLE_MIN ;
  length                            = kDOUBLE_MIN ;
  width                             = kDOUBLE_MIN ;
  hit_density_1D                    = kDOUBLE_MIN ;
  hit_density_2D                    = kDOUBLE_MIN ;
  direction                         =  0       ;
  showerness                        = kDOUBLE_MIN ;
  trackness                         = kDOUBLE_MIN ;
}

void cluster_params::Report(std::ostream & os) const {

  os << "ClusterParams report: \n"
     << "   plane  .................... : " << plane_id.Plane << "\n"
     << "   start_point  .............. : (" << start_point.w << "," << start_point.t << ")\n"
     << "   end_point  ................ : (" << end_point.w << "," << end_point.t << ")\n"
     << "   showering_point  .......... : (" << showering_point.w << "," << showering_point.t << ")\n"
     << "   sum_charge  ............... : " << sum_charge << "\n"
     << "   mean_charge   ............. : " << mean_charge << "\n"
     << "   mean_x  ................... : " << mean_x << "\n"
     << "   mean_y  ................... : " << mean_y << "\n"
     << "   rms_x  .................... : " << rms_x << "\n"
     << "   rms_y  .................... : " << rms_y << "\n"
     << "   charge_wgt_x  ............. : " << charge_wgt_x << "\n"
     << "   charge_wgt_y  ............. : " << charge_wgt_y << "\n"
     << "   slope_2d_high_q  .......... : " << slope_2d_high_q << "\n"
     << "   slope_2d  ................. : " << slope_2d         << "\n"
     << "   angle_2d  ................. : " << angle_2d         << "\n"
     << "   start_dir  ................ : (" << start_dir[0] << "," << start_dir[1] << ")\n"
     << "   principal_dir  ............ : (" << principal_dir[0] << "," << principal_dir[1] << ")\n"
     << "   eigenvalue_principal  ..... : " << eigenvalue_principal << "\n"
     << "   eigenvalue_secondary  ..... : " << eigenvalue_secondary << "\n"
     << "   multi_hit_wires  .......... : " << multi_hit_wires << "\n"
     << "   N_Wires  .................. : " << N_Wires << "\n"
     << "   N_Hits  ................... : " << N_Hits << "\n"
     << "   N_Hits_HC  ................ : " << N_Hits_HC << "\n"
     << "   verticalness  ............. : " << verticalness << "\n"
     << "   opening_angle  ............ : " << opening_angle << "\n"
     << "   opening_angle_charge_wgt  . : " << opening_angle_charge_wgt << "\n"
     << "   closing_angle  ............ : " << closing_angle << "\n"
     << "   closing_angle_charge_wgt  . : " << closing_angle_charge_wgt << "\n"
     << "   offaxis_hits  ............. : " << offaxis_hits << "\n"
     << "   modified_hit_density  ..... : " << modified_hit_density << "\n"
     << "   modified mean charge ...... : " << modmeancharge << "\n"
     << "   charge_RMS ................ : " << RMS_charge << "\n"
     << "   length  ................... : " << length << "\n"
     << "   width  .................... : " << width << "\n"
     << "   hit_density_1D  ........... : " << hit_density_1D << "\n"
     << "   hit_density_2D  ........... : " << hit_density_2D << "\n"
     << "   showerness  ............... : " << showerness << "\n"
     << "   trackness  ................ : " << trackness << "\n"
     << "   direction  ................ : " << direction << "\n"
     << "   Polygon  .................. : " << PolyObject << "\n";

}


void  cluster_params::GetFANNVector(std::vector<float> & data) const {
  unsigned int length = 9;
  if (data.size() != length) {
    data.clear();
    data.reserve(length);
  }
  data.push_back( -opening_angle / M_PI + closing_angle / M_PI );
  // data.push_back( -opening_angle_charge_wgt / M_PI + closing_angle_charge_wgt / M_PI );
  data.push_back( -log(1 - eigenvalue_principal) / 10.0 );
  // data.push_back( -log(eigenvalue_secondary) / 10.0 );
  data.push_back( width / length );
  // data.push_back( hit_density_1D / modified_hit_density );
  data.push_back( multi_hit_wires / N_Wires );
  data.push_back( modified_hit_density );
  data.push_back( RMS_charge / mean_charge );
  data.push_back( log(sum_charge / length) / 10.0 );
  return;
}


std::vector<std::string> cluster_params::GetFANNVectorTitle() {
  std::vector<std::string> FannLegend;
  FannLegend.push_back("Opening - Closing Angle (normalized)");
  // FannLegend.push_back("Opening - Closing Angle charge weight");
  // FannLegend.push_back("Closing Angle (normalized)");
  // FannLegend.push_back("Closing Angle charge weight");
  FannLegend.push_back("Principal Eigenvalue");
  // FannLegend.push_back("Secondary Eigenvalue");
  FannLegend.push_back("Width / Length");
  // FannLegend.push_back("Hit Density / M.H.D.");
  FannLegend.push_back("Percent MultiHit Wires");
  // FannLegend.push_back("Percent High Charge Hits");
  FannLegend.push_back("Modified Hit Density");
  FannLegend.push_back("Charge RMS / Mean Charge");
  FannLegend.push_back("log(Sum Charge / Length)");
  return FannLegend;

}


void  cluster_params::PrintFANNVector() {
  std::vector<float> data;
  GetFANNVector(data);
  // if(verbose){
  int i = 0;
  std::cout << "Printing FANN input vector:\n";
  std::cout << "   Opening - Closing Angle (normalized)  ... : " << data[i] << "\n"; i++;
  // std::cout << "   Opening - Closing Angle charge weight  .. : " << data[i] << "\n"; i++;
  // std::cout << "   Closing Angle (normalized)  ............. : " << data[i] << "\n"; i++;
  // std::cout << "   Closing Angle charge weight  ............ : " << data[i] << "\n"; i++;
  std::cout << "   Principal Eigenvalue  ................... : " << data[i] << "\n"; i++;
  // std::cout << "   Secondary Eigenvalue  ................... : " << data[i] << "\n"; i++;
  std::cout << "   Width / Length  ......................... : " << data[i] << "\n"; i++;
  // std::cout << "   Hit Density / M.H.D.  ................... : " << data[i] << "\n"; i++;
  std::cout << "   Percent MultiHit Wires  ................. : " << data[i] << "\n"; i++;
  // std::cout << "   Percent High Charge Hits  ............... : " << data[i] << "\n"; i++;
  std::cout << "   Modified Hit Density  ................... : " << data[i] << "\n"; i++;
  std::cout << "   Charge RMS / Mean Charge ................ : " << data[i] << "\n"; i++;
  std::cout << "   log(Sum Charge / Length) ................ : " << data[i] << "\n"; i++;
  // }
  return;
}

void cluster_params::ReportDiff(const cluster_params & otherCluster) {



  if (this -> start_point.w != otherCluster.start_point.w ||
      this -> start_point.t != otherCluster.start_point.t ) {
    std::cout << "   start_point  .............. : "
              << "(" << this->start_point.w << ", " << this->start_point.t << ") vs. "
              << "(" << otherCluster.start_point.w << ", " << otherCluster.start_point.t << ")\n";
  }

  if (this -> end_point.w != otherCluster.end_point.w ||
      this -> end_point.t != otherCluster.end_point.t ) {
    std::cout << "   end_point  ................ : "
              << "(" << this->end_point.w << ", " << this->end_point.t << ") vs. "
              << "(" << otherCluster.end_point.w << ", " << otherCluster.end_point.t << ")\n";
  }

  if (this -> showering_point.w != otherCluster.showering_point.w ||
      this -> showering_point.t != otherCluster.showering_point.t ) {
    std::cout << "   showering_point  .......... : "
              << "(" << this->showering_point.w << ", " << this->showering_point.t << ") vs. "
              << "(" << otherCluster.showering_point.w << ", " << otherCluster.showering_point.t << ")\n";
  }

  if (this -> sum_charge               != otherCluster.sum_charge) {
    std::cout << "   sum_charge  ............... : "
              << this -> sum_charge << " vs. "
              << otherCluster.sum_charge << "\n";
  }
  if (this -> mean_charge              != otherCluster.mean_charge) {
    std::cout << "   mean_charge   ............. : "
              << this -> mean_charge << " vs. "
              << otherCluster.mean_charge << "\n";
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
  if (this -> charge_wgt_x             != otherCluster.charge_wgt_x) {
    std::cout << "   charge_wgt_x  ............. : "
              << this -> charge_wgt_x << " vs. "
              << otherCluster.charge_wgt_x << "\n";
  }
  if (this -> charge_wgt_y             != otherCluster.charge_wgt_y) {
    std::cout << "   charge_wgt_y  ............. : "
              << this -> charge_wgt_y << " vs. "
              << otherCluster.charge_wgt_y << "\n";
  }
  if (this -> slope_2d_high_q         != otherCluster.slope_2d_high_q) {
    std::cout << "   slope_2d_high_q  .......... : "
              << this -> slope_2d_high_q << " vs. "
              << otherCluster.slope_2d_high_q << "\n";
  }
  if (this -> slope_2d                 != otherCluster.slope_2d) {
    std::cout << "   slope_2d  ................. : "
              << this -> slope_2d << " vs. "
              << otherCluster.slope_2d << "\n";
  }
  if (this -> angle_2d                 != otherCluster.angle_2d) {
    std::cout << "   angle_2d  ................. : "
              << this -> angle_2d << " vs. "
              << otherCluster.angle_2d << "\n";
  }
  if (this -> start_dir[0] != otherCluster.start_dir[0] ||
      this -> start_dir[1] != otherCluster.start_dir[1] ) {
    std::cout << "   start_dir  ................ : "
              << "(" << this->start_dir[0] << ", " << this->start_dir[1] << ") vs. "
              << "(" << otherCluster.start_dir[0] << ", " << otherCluster.start_dir[1] << ")\n";
  }
  if (this -> principal_dir[0] != otherCluster.principal_dir[0] ||
      this -> principal_dir[1] != otherCluster.principal_dir[1] ) {
    std::cout << "   principal_dir  ............ : "
              << "(" << this->principal_dir[0] << ", " << this->principal_dir[1] << ") vs. "
              << "(" << otherCluster.principal_dir[0] << ", " << otherCluster.principal_dir[1] << ")\n";
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
  if (this -> multi_hit_wires          != otherCluster.multi_hit_wires) {
    std::cout << "   multi_hit_wires  .......... : "
              << this -> multi_hit_wires << " vs. "
              << otherCluster.multi_hit_wires << "\n";
  }
  if (this -> N_Wires                  != otherCluster.N_Wires) {
    std::cout << "   N_Wires  .................. : "
              << this -> N_Wires << " vs. "
              << otherCluster.N_Wires << "\n";
  }
  if (this -> N_Hits                   != otherCluster.N_Hits) {
    std::cout << "   N_Hits  ................... : "
              << this -> N_Hits << " vs. "
              << otherCluster.N_Hits << "\n";
  }
  if (this -> N_Hits_HC                != otherCluster.N_Hits_HC) {
    std::cout << "   N_Hits_HC  ................ : "
              << this -> N_Hits_HC << " vs. "
              << otherCluster.N_Hits_HC << "\n";
  }
  if (this -> verticalness             != otherCluster.verticalness) {
    std::cout << "   verticalness  ............. : "
              << this -> verticalness << " vs. "
              << otherCluster.verticalness << "\n";
  }
  if (this -> opening_angle            != otherCluster.opening_angle) {
    std::cout << "   opening_angle  ............ : "
              << this -> opening_angle << " vs. "
              << otherCluster.opening_angle << "\n";
  }
  if (this -> opening_angle_charge_wgt != otherCluster.opening_angle_charge_wgt) {
    std::cout << "   opening_angle_charge_wgt  . : "
              << this -> opening_angle_charge_wgt << " vs. "
              << otherCluster.opening_angle_charge_wgt << "\n";
  }
  if (this -> closing_angle            != otherCluster.closing_angle) {
    std::cout << "   closing_angle  ............ : "
              << this -> closing_angle << " vs. "
              << otherCluster.closing_angle << "\n";
  }
  if (this -> closing_angle_charge_wgt != otherCluster.closing_angle_charge_wgt) {
    std::cout << "   closing_angle_charge_wgt  . : "
              << this -> closing_angle_charge_wgt << " vs. "
              << otherCluster.closing_angle_charge_wgt << "\n";
  }
  if (this -> offaxis_hits             != otherCluster.offaxis_hits) {
    std::cout << "   offaxis_hits  ............. : "
              << this -> offaxis_hits << " vs. "
              << otherCluster.offaxis_hits << "\n";
  }
  if (this -> modified_hit_density     != otherCluster.modified_hit_density) {
    std::cout << "   modified_hit_density  ..... : "
              << this -> modified_hit_density << " vs. "
              << otherCluster.modified_hit_density << "\n";
  }
  if (this -> modmeancharge            != otherCluster.modmeancharge) {
    std::cout << "   modified mean charge ...... : "
              << this -> modmeancharge << " vs. "
              << otherCluster.modmeancharge << "\n";
  }
  if (this -> RMS_charge               != otherCluster.RMS_charge) {
    std::cout << "   charge_RMS ................ : "
              << this -> RMS_charge << " vs. "
              << otherCluster.RMS_charge << "\n";
  }
  if (this -> length                   != otherCluster.length) {
    std::cout << "   length  ................... : "
              << this -> length << " vs. "
              << otherCluster.length << "\n";
  }
  if (this -> width                    != otherCluster.width) {
    std::cout << "   width  .................... : "
              << this -> width << " vs. "
              << otherCluster.width << "\n";
  }
  if (this -> hit_density_1D           != otherCluster.hit_density_1D) {
    std::cout << "   hit_density_1D  ........... : "
              << this -> hit_density_1D << " vs. "
              << otherCluster.hit_density_1D << "\n";
  }
  if (this -> hit_density_2D           != otherCluster.hit_density_2D) {
    std::cout << "   hit_density_2D  ........... : "
              << this -> hit_density_2D << " vs. "
              << otherCluster.hit_density_2D << "\n";
  }
  if (this -> showerness               != otherCluster.showerness) {
    std::cout << "   showerness  ............... : "
              << this -> showerness << " vs. "
              << otherCluster.showerness << "\n";
  }
  if (this -> trackness                != otherCluster.trackness) {
    std::cout << "   trackness  ................ : "
              << this -> trackness << " vs. "
              << otherCluster.trackness << "\n";
  }
  if (this -> direction                != otherCluster.direction) {
    std::cout << "   direction  ................ : "
              << this -> direction << " vs. "
              << otherCluster.direction << "\n";
  }

  if (this -> PolyObject               != otherCluster.PolyObject) {
    std::cout << "   polygon ................... : "
              << this->PolyObject << " vs. "
              << otherCluster.PolyObject << "\n";
  }
  ////Polygon


}

} // cluster

#endif
