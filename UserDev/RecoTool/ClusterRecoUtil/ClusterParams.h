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
#include <climits>
#include <limits>
#include "Polygon2D.h"
#include "Base/GeoTypes.h"


#include "LArUtil/PxUtils.h"
typedef larutil::PxHit Hit2D;
typedef larutil::PxPoint Point2D;

namespace cluster{


  /// Utility: maximum value for double 
  const double kDOUBLE_MAX = std::numeric_limits<double>::max();
  
  /// Utility: minimum value for double
  const double kDOUBLE_MIN = std::numeric_limits<double>::min();

  /// Utility: maximum value for int
  const int    kINT_MAX    = std::numeric_limits<int>::max();
  
  /// Utility: maximum value for unsigned int
  const unsigned int kUINT_MAX    = std::numeric_limits<unsigned int>::max();

  /// Utility: maximum value for size_t
  const size_t kSIZE_MAX   = std::numeric_limits<size_t>::max();


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

    /// Hold the list of hits for this particular cluster, for convenience 
    std::vector< Hit2D > hit_vector;
    // The plane of this cluster, all hits are from this plane
    ::larlite::geo::PlaneID     plane_id;



    Polygon2D PolyObject;               ///< Polygon Object...see Polygon2D.hh

    Point2D start_point;      ///< start point 
    Point2D end_point;        ///< end point 

    double sum_charge;                 ///< Sum charge of hits in ADC
    double mean_charge;                ///< Mean (average) charge of hits in ADC
    double mean_x;                     ///< Mean of hits along x, peaks only
    double mean_y;                     ///< Mean of hits along y, peaks only
    double rms_x;                      ///< rms of hits along x (wires)
    double rms_y;                      ///< rms of hits along y, (time)
    double charge_wgt_x;               ///< Mean of hits along x, charge weighted
    double charge_wgt_y;               ///< Mean of hits along y, charge weighted
    double slope_2d_high_q;           ///< Linear best fit to high-charge hits in the cluster
    double slope_2d;                   ///< The slope of the cluster in this plane
    double opening_angle;              ///< Width of angular distubtion wrt vertx
    double opening_angle_charge_wgt;   ///< Same for charge_wgt
    double closing_angle;              ///< Width of angular distubtion wrt endpoint
    double closing_angle_charge_wgt;   ///< Same for charge_wgt
    double eigenvalue_principal;       ///< the principal eigenvalue from PCA
    double eigenvalue_secondary;       ///< the secondary eigenvalue from PCA
    double verticalness;               ///< ???
    std::vector<float> principal_dir;  ///< The direction of the principal eigenvalue


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

    void Clear();

    void Report(std::ostream & os = std::cout) const;

    void ReportDiff(const cluster_params & otherParams);

  };
}

#endif
/** @} */ // end of doxygen group 
