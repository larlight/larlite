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
#include "TString.h"


#include "LArUtil/PxUtils.h"

namespace cluster {


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
  cluster_params() {
    //Initialize all values.
    Clear();
  }

  ~cluster_params() {};

  /// Hold the list of hits for this particular cluster, for convenience
  std::vector< Hit2D > hit_vector;
  // The plane of this cluster, all hits are from this plane
  ::larlite::geo::PlaneID     plane_id;


  // data about the cluster that this params was producer from,
  // used if you need to get associations for this object:
  //
  // The cluster producer that made the cluster that has associations to it
  std::string original_producer;
  // The index of those associations
  std::vector<unsigned short> original_indexes;


  Polygon2D PolyObject;              ///< Polygon Object...see Polygon2D.hh

  Point2D start_point;               ///< start point
  Point2D showering_point;           ///< point at which showering of cluster begins
  Point2D end_point;                 ///< end point

  std::vector<Point2D > start_point_cand;     ///<Worker variable saves the list of candidate start points between modules
  std::vector<Point2D > shwr_point_cand;     ///<Worker variable saves the list of candidate start points between modules
  std::vector<Point2D > start_dir_cand;       ///candidate start point directions



  double sum_charge;                 ///< Sum charge of hits in ADC
  double mean_charge;                ///< Mean (average) charge of hits in ADC
  double rms_charge;                 ///< RMS (standard deviation of sample) of charge of hits in ADC
  double sum_ADC;                    ///< Sum charge of ADC counts of hits, in ADC
  double mean_ADC;                   ///< Mean (average) of ADC counts of hits, in ADC
  double rms_ADC;                    ///< RMS (standard deviation of sample) of ADC counts of hits in ADC
  double mean_x;                     ///< Mean of hits along x, peaks only
  double mean_y;                     ///< Mean of hits along y, peaks only
  double rms_x;                      ///< rms of hits along x (wires)
  double rms_y;                      ///< rms of hits along y, (time)
  double charge_wgt_x;               ///< Mean of hits along x, charge weighted
  double charge_wgt_y;               ///< Mean of hits along y, charge weighted
  double slope_2d_high_q;            ///< Linear best fit to high-charge hits in the cluster
  double slope_2d;                   ///< The slope of the cluster in this plane
  double angle_2d;                   ///< The slope of the cluster converted to an angle, in radians
  double opening_angle;              ///< Width of angular distubtion wrt vertx
  double opening_angle_charge_wgt;   ///< Same for charge_wgt
  double closing_angle;              ///< Width of angular distubtion wrt endpoint
  double closing_angle_charge_wgt;   ///< Same for charge_wgt
  double start_charge;               ///< Charge at the start of the cluster
  double end_charge;                 ///< Charge at the (other) end of the cluster
  double eigenvalue_principal;       ///< the principal eigenvalue from PCA
  double eigenvalue_secondary;       ///< the secondary eigenvalue from PCA
  double verticalness;               ///< ???
  std::vector<float> principal_dir;  ///< The direction of the principal eigenvalue
  std::vector<float> start_dir;      ///< The slope of the cluster right at the start


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

  double showerness;                 ///< Unused
  double trackness;                  ///< Unused
  double offaxis_hits;               ///< Unused


  /**
   * @brief set the hits of the params object
   * @details These functions will convert, if necessary, the hits and
   * then save them into the params object
   *
   * @param a vector of hits, either pointer to larlite hits, Hit2D
   */

  int SetHits(const std::vector< Hit2D> & inputHits);


  /**
   * @brief Clear all content
   * @details Sets the values of every variable in the class to a default, nonsensical value
   */
  void Clear();

  /**
   * @brief Print the contents of the class
   * @details Displays in text the value of every member of the class
   *
   * @param os Can optionally pass a different outstream than std::cout
   */
  void Report(std::ostream & os = std::cout) const;


  /**
   * @brief Compare two cluster_params objects and report the difference
   * @details Compares the members of each cluster params and prints out
   *          only the differences between the two sets.
   *
   * @param otherParams Another instance of cluster_params.s
   */
  void ReportDiff(const cluster_params & otherParams);

  /**
   * This function returns a feature vector suitable for a neural net
   * This function uses the data from cluster_params but packages it
   * up in a different way, and so is inappropriate to include in
   * clusterParams.hh.   That's why it's here.
   * @param  data  takes a reference to a vector< float>
   */
  void  GetFANNVector(std::vector<float> & data) const;

  std::vector<std::string> GetFANNVectorTitle(){
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



  /**
   * For debugging purposes, prints the result of GetFANNVector
   * in a nicely formatted form.
   * @return [description]
   */
  void  PrintFANNVector();


};

}

#endif
/** @} */ // end of doxygen group
