#ifndef CLUSTERPARAMS_CXX
#define CLUSTERPARAMS_CXX

#include "ClusterParams.h"

namespace cluster {

  void cluster_params::Clear(){
      start_point.Clear();
      end_point.Clear();
      sum_charge                        = -999.999 ;
      mean_charge                       = -999.999 ;
      mean_x                            = -999.999 ;
      mean_y                            = -999.999 ;
      rms_x                             = -999.999 ;
      rms_y                             = -999.999 ;
      charge_wgt_x                      = -999.999 ;
      charge_wgt_y                      = -999.999 ;
      cluster_angle_2d                  = -999.999 ;
      angle_2d                          = -999.999 ;
      eigenvalue_principal              = -999.999 ;
      eigenvalue_secondary              = -999.999 ;
      multi_hit_wires                   = -999.999 ;
      N_Wires                           = -999.999 ;
      verticalness                      = -999.999 ;
      opening_angle                     = -999.999 ;
      opening_angle_charge_wgt          = -999.999 ;
      closing_angle                     = -999.999 ;
      closing_angle_charge_wgt          = -999.999 ;
      offaxis_hits                      = -999.999 ;
      modmeancharge                     = -999.999 ;
      RMS_charge                        = -999.999 ;
      modified_hit_density              = -999.999 ;    
      N_Hits                            = -999.999 ;    
      N_Hits_HC                         = -999.999 ;    
      length                            = -999.999 ;
      width                             = -999.999 ;
      hit_density_1D                    = -999.999 ;
      hit_density_2D                    = -999.999 ;
      direction                         =  0       ;
      showerness                        = -999.999 ;
      trackness                         = -999.999 ;
    }

  void cluster_params::Report(std::ostream & os) const {

      os << "ClusterParams report: \n"
         << "   start_point  ................ : (" << start_point.w <<","<<start_point.t<<")\n"
         << "   end_point  .................. : (" << end_point.w <<","<<end_point.t<<")\n"
         << "   sum_charge  ................. : "<< sum_charge << "\n"
         << "   mean_charge   ............. : " << mean_charge << "\n"
         << "   mean_x  ................... : " << mean_x << "\n"
         << "   mean_y  ................... : " << mean_y << "\n"
         << "   rms_x  .................... : " << rms_x << "\n"
         << "   rms_y  .................... : " << rms_y << "\n"
         << "   charge_wgt_x  ............. : " << charge_wgt_x << "\n"
         << "   charge_wgt_y  ............. : " << charge_wgt_y << "\n"
         << "   cluster_angle_2d  ......... : " << cluster_angle_2d << "\n"
         << "   angle_2d  ................. : " << angle_2d         << "\n"
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
         << "   direction  ................ : " << direction << "\n";
    }

  void cluster_params::ReportDiff(const cluster_params & otherCluster){


    if (this -> start_point.w != otherCluster.start_point.w ||
        this -> start_point.t != otherCluster.start_point.t ){
      std::cout << "start_point is different: " 
                << "(" << this->start_point.w << ", " << this->start_point.t << ") vs. "
                << "(" << otherCluster.start_point.w << ", " << otherCluster.start_point.t << ")\n";
    }

    if (this -> end_point.w != otherCluster.end_point.w ||
        this -> end_point.t != otherCluster.end_point.t ){
      std::cout << "end_point is different: " 
                << "(" << this->end_point.w << ", " << this->end_point.t << ") vs. "
                << "(" << otherCluster.end_point.w << ", " << otherCluster.end_point.t << ")\n";
    }

    if (this -> sum_charge               != otherCluster.sum_charge){
      std::cout << "sum_charge is different: "
                << this -> sum_charge << " vs. "
                << otherCluster.sum_charge << "\n";
    }
    if (this -> mean_charge              != otherCluster.mean_charge){
      std::cout << "mean_charge is different: "
                << this -> mean_charge << " vs. "
                << otherCluster.mean_charge << "\n";
    }
    if (this -> mean_x                   != otherCluster.mean_x){
      std::cout << "mean_x is different: "
                << this -> mean_x << " vs. "
                << otherCluster.mean_x << "\n";
    }
    if (this -> mean_y                   != otherCluster.mean_y){
      std::cout << "mean_y is different: "
                << this -> mean_y << " vs. "
                << otherCluster.mean_y << "\n";
    }
    if (this -> rms_x                    != otherCluster.rms_x){
      std::cout << "rms_x is different: "
                << this -> rms_x << " vs. "
                << otherCluster.rms_x << "\n";
    }
    if (this -> rms_y                    != otherCluster.rms_y){
      std::cout << "rms_y is different: "
                << this -> rms_y << " vs. "
                << otherCluster.rms_y << "\n";
    }
    if (this -> charge_wgt_x             != otherCluster.charge_wgt_x){
      std::cout << "charge_wgt_x is different: "
                << this -> charge_wgt_x << " vs. "
                << otherCluster.charge_wgt_x << "\n";
    }
    if (this -> charge_wgt_y             != otherCluster.charge_wgt_y){
      std::cout << "charge_wgt_y is different: "
                << this -> charge_wgt_y << " vs. "
                << otherCluster.charge_wgt_y << "\n";
    }
    if (this -> cluster_angle_2d         != otherCluster.cluster_angle_2d){
      std::cout << "cluster_angle_2d is different: "
                << this -> cluster_angle_2d << " vs. "
                << otherCluster.cluster_angle_2d << "\n";
    }
    if (this -> angle_2d                 != otherCluster.angle_2d){
      std::cout << "angle_2d is different: "
                << this -> angle_2d << " vs. "
                << otherCluster.angle_2d << "\n";
    }
    if (this -> eigenvalue_principal     != otherCluster.eigenvalue_principal){
      std::cout << "eigenvalue_principal is different: "
                << this -> eigenvalue_principal << " vs. "
                << otherCluster.eigenvalue_principal << "\n";
    }
    if (this -> eigenvalue_secondary     != otherCluster.eigenvalue_secondary){
      std::cout << "eigenvalue_secondary is different: "
                << this -> eigenvalue_secondary << " vs. "
                << otherCluster.eigenvalue_secondary << "\n";
    }
    if (this -> multi_hit_wires          != otherCluster.multi_hit_wires){
      std::cout << "multi_hit_wires is different: "
                << this -> multi_hit_wires << " vs. "
                << otherCluster.multi_hit_wires << "\n";
    }
    if (this -> N_Wires                  != otherCluster.N_Wires){
      std::cout << "N_Wires is different: "
                << this -> N_Wires << " vs. "
                << otherCluster.N_Wires << "\n";
    }
    if (this -> N_Hits                   != otherCluster.N_Hits){
      std::cout << "N_Hits is different: "
                << this -> N_Hits << " vs. "
                << otherCluster.N_Hits << "\n";
    }
    if (this -> N_Hits_HC                != otherCluster.N_Hits_HC){
      std::cout << "N_Hits_HC is different: "
                << this -> N_Hits_HC << " vs. "
                << otherCluster.N_Hits_HC << "\n";
    }
    if (this -> verticalness             != otherCluster.verticalness){
      std::cout << "verticalness is different: "
                << this -> verticalness << " vs. "
                << otherCluster.verticalness << "\n";
    }
    if (this -> opening_angle            != otherCluster.opening_angle){
      std::cout << "opening_angle is different: "
                << this -> opening_angle << " vs. "
                << otherCluster.opening_angle << "\n";
    }
    if (this -> opening_angle_charge_wgt != otherCluster.opening_angle_charge_wgt){
      std::cout << "opening_angle_charge_wgt is different: "
                << this -> opening_angle_charge_wgt << " vs. "
                << otherCluster.opening_angle_charge_wgt << "\n";
    }
    if (this -> closing_angle            != otherCluster.closing_angle){
      std::cout << "closing_angle is different: "
                << this -> closing_angle << " vs. "
                << otherCluster.closing_angle << "\n";
    }
    if (this -> closing_angle_charge_wgt != otherCluster.closing_angle_charge_wgt){
      std::cout << "closing_angle_charge_wgt is different: "
                << this -> closing_angle_charge_wgt << " vs. "
                << otherCluster.closing_angle_charge_wgt << "\n";
    }
    if (this -> offaxis_hits             != otherCluster.offaxis_hits){
      std::cout << "offaxis_hits is different: "
                << this -> offaxis_hits << " vs. "
                << otherCluster.offaxis_hits << "\n";
    }
    if (this -> modified_hit_density     != otherCluster.modified_hit_density){
      std::cout << "modified_hit_density is different: "
                << this -> modified_hit_density << " vs. "
                << otherCluster.modified_hit_density << "\n";
    }
    if (this -> modmeancharge            != otherCluster.modmeancharge){
      std::cout << "modmeancharge is different: "
                << this -> modmeancharge << " vs. "
                << otherCluster.modmeancharge << "\n";
    }
    if (this -> RMS_charge               != otherCluster.RMS_charge){
      std::cout << "RMS_charge is different: "
                << this -> RMS_charge << " vs. "
                << otherCluster.RMS_charge << "\n";
    }
    if (this -> length                   != otherCluster.length){
      std::cout << "length is different: "
                << this -> length << " vs. "
                << otherCluster.length << "\n";
    }
    if (this -> width                    != otherCluster.width){
      std::cout << "width is different: "
                << this -> width << " vs. "
                << otherCluster.width << "\n";
    }
    if (this -> hit_density_1D           != otherCluster.hit_density_1D){
      std::cout << "hit_density_1D is different: "
                << this -> hit_density_1D << " vs. "
                << otherCluster.hit_density_1D << "\n";
    }
    if (this -> hit_density_2D           != otherCluster.hit_density_2D){
      std::cout << "hit_density_2D is different: "
                << this -> hit_density_2D << " vs. "
                << otherCluster.hit_density_2D << "\n";
    }
    if (this -> showerness               != otherCluster.showerness){
      std::cout << "showerness is different: "
                << this -> showerness << " vs. "
                << otherCluster.showerness << "\n";
    }
    if (this -> trackness                != otherCluster.trackness){
      std::cout << "trackness is different: "
                << this -> trackness << " vs. "
                << otherCluster.trackness << "\n";
    }
    if (this -> direction                != otherCluster.direction){
      std::cout << "direction is different: "
                << this -> direction << " vs. "
                << otherCluster.direction << "\n";
    }

  }

} // cluster

#endif
