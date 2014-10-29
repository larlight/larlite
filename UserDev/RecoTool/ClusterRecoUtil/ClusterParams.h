/**
 * \file ClusterParams.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ClusterParams
 *
 * @author andrzejs
 */

/** \addtogroup ClusterRecoUtil

    @{*/

#ifndef RECOTOOL_CLUSTERPARAMS_H
#define RECOTOOL_CLUSTERPARAMS_H

#include <ostream>
#include "LArUtil/PxUtils.h"
#include "Polygon2D.h"

namespace cluster{

  /**
     \class cluster_params
     (Detailed) information holder for 2D cluster, computed by ClusterParamsAlg
  */
  class cluster_params
  {
  public:
    cluster_params(){
      //Initialize all values.
      Clear();
    } 

    ~cluster_params(){};

    Polygon2D PolyObject;               ///< Polygon Object...see Polygon2D.hh

    larutil::PxPoint start_point;      ///< start point 
    larutil::PxPoint end_point;        ///< end point 

    double sum_charge;                 ///< Sum charge of hits in ADC
    double mean_charge;                ///< Mean (average) charge of hits in ADC
    double mean_x;                     ///< Mean of hits along x, peaks only
    double mean_y;                     ///< Mean of hits along y, peaks only
    double rms_x;                      ///< rms of hits along x (wires)
    double rms_y;                      ///< rms of hits along y, (time)
    double charge_wgt_x;               ///< Mean of hits along x, charge weighted
    double charge_wgt_y;               ///< Mean of hits along y, charge weighted
    double cluster_angle_2d;           ///< Linear best fit to high-charge hits in the cluster
    double angle_2d;                   ///< Angle of axis in wire/hit view
    double opening_angle;              ///< Width of angular distubtion wrt vertx
    double opening_angle_charge_wgt;   ///< Same for charge_wgt
    double closing_angle;              ///< Width of angular distubtion wrt endpoint
    double closing_angle_charge_wgt;   ///< Same for charge_wgt
    double eigenvalue_principal;       ///< the principal eigenvalue from PCA
    double eigenvalue_secondary;       ///< the secondary eigenvalue from PCA
    double verticalness;               ///< ???

    double length;
    double width;

    double hit_density_1D;
    double hit_density_2D;
    double multi_hit_wires;
    double N_Wires;
    double modified_hit_density;
    double N_Hits;
    double N_Hits_HC;

    double modmeancharge;
    double RMS_charge;
    
    /**
       +1 means shower is "forward - start wire < end wire"
       -1 is backwards, 0 is undecided
    */
    int    direction;

    double showerness;                 ///< got heart
    double trackness;                  ///< got soul                                       
    double offaxis_hits;               ///< got brain

    void Clear(){
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

    void Report(std::ostream & os = std::cout){

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

  };
}

#endif
/** @} */ // end of doxygen group 
