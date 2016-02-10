/**
 * \file Cluster3DParams.h
 *
 * \ingroup ClusterRecoUtil
 *
 * \brief Class def header for a class Cluster3DParams
 *
 * @author cadams
 */

/** \addtogroup Cluster3DRecoUtil

    @{*/

#ifndef RECOTOOL_CLUSTER3DPARAMS_H
#define RECOTOOL_CLUSTER3DPARAMS_H

#include <iostream>
#include <climits>
#include <limits>
#include <vector> 

#include "TVector3.h" 

namespace cluster3D {

class Point3D {


public:

  Point3D(){x = y = z = 0.0;}
  ~Point3D(){}

  Point3D(float x, float y, float z) :
  x(x),
  y(y),
  z(z)
  {}

  float X() const {return x;}
  float Y() const {return y;}
  float Z() const {return z;}
  float x;
  float y;
  float z;

  void Clear(){x = y = z = 0.0;}

  bool operator==(const Point3D & other){
    return (this->x == other.x)&&(this->y == other.y)&&(this->y == other.y);
  }
  bool operator!=(const Point3D & other){return !(*this == other);}

};


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
   \class cluster3D_params
   (Detailed) information holder for 2D cluster, computed by ClusterParamsAlg
*/
class cluster3D_params
{
public:
  cluster3D_params() {
    //Initialize all values.
    Clear();
  }

  ~cluster3D_params() {};

  /// Hold the list of spacepoints for this particular cluster, for convenience
  std::vector< Point3D > point_vector;

  Point3D start_point;               ///< start point
  Point3D showering_point;           ///< point at which showering of cluster begins
  Point3D end_point;                 ///< end point

  std::vector<Point3D >  start_point_cand;     ///<Worker variable saves the list of candidate start points between modules
  std::vector<Point3D >  shwr_point_cand;     ///<Worker variable saves the list of candidate start points between modules
  std::vector<TVector3 > start_dir_cand;       ///candidate start point directions

  // Keep an orderd list of the indexes of the points at the start of the cluster3D
  std::vector<size_t> starting_point_indexes;

  double mean_x;                     ///< Mean of points along x
  double mean_y;                     ///< Mean of points along y
  double mean_z;                     ///< Mean of points along z
  double rms_x;                      ///< rms of points along x
  double rms_y;                      ///< rms of points along y
  double rms_z;                      ///< rms of points along z

  double opening_angle;              ///< Width of angular distubtion wrt vertx
  double closing_angle;              ///< Width of angular distubtion wrt endpoint

  double eigenvalue_principal;       ///< the principal eigenvalue from PCA
  double eigenvalue_secondary;       ///< the secondary eigenvalue from PCA
  double eigenvalue_tertiary;        ///< the third eigenvalue from PCA

  TVector3 principal_dir;            ///< The direction of the principal axis, normalized to 1
  TVector3 secondary_dir;            ///< The secondary axis, perp to first axis, normalized
  TVector3 tertiary_dir;             ///< Third axis, perp to first two, normalized.

  TVector3 start_dir;                ///< The direction of the start only of this cluster


  double length;
  double width_secondary;
  double width_tertiary;

  double N_points;


  /**
   * @brief set the hits of the params object
   * @details Only accepts a list of Point3D
   *
   * @param a vector of points, Point3D
   */

  int SetPoints(const std::vector< Point3D> & inputHits);


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
   * @brief Compare two cluster3D_params objects and report the difference
   * @details Compares the members of each cluster params and prints out
   *          only the differences between the two sets.
   *
   * @param otherParams Another instance of cluster3D_params.s
   */
  void ReportDiff(const cluster3D_params & otherParams);

private:

  std::string vectorPrint(const TVector3 & vec) const;
  std::string point3DPrint(const Point3D & vec) const;

};

}

#endif
/** @} */ // end of doxygen group
