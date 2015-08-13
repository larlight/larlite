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

} // cluster

#endif
