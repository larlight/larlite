/**
 * \file QWeightPoint.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class QWeightPoint
 *
 * @author kazuhiro
 */

/** \addtogroup Algorithms

    @{*/
#ifndef OPT0FINDER_QWEIGHTPOINT_H
#define OPT0FINDER_QWEIGHTPOINT_H

#include "OpT0Finder/Base/BaseFlashMatch.h"

namespace flashana {
  
  /**
     \class QWeightPoint
     Implementation of flashana::BaseFlashHypothesis algorithm class. \n
     Given a TPC object (flashana::QCluster_t), it calcultes a list of flash hypothesis \n
     points based on charge deposition and its geometrical position. Each energy deposition \n
     point is weighted by its charge and inverse-squared-x position. As the absolute \n
     x-position is not known by a TPC object, it uses a relative position for each point \n
     w.r.t. the closest point to the wire plane (x=0). The algorithm then assigns an overall \n
     absolute x-position offset in a successive step of _x_step_size value, assigned by a user, \n
     to compute possible flash hypothesis points.\n
  */
  class QWeightPoint : public BaseFlashMatch {
    
  public:
    
    /// Default constructor
    QWeightPoint(const double x_step_size=-1);
    
    QWeightPoint( const std::vector<double>& pos_x,
		  const std::vector<double>& pos_y,
		  const std::vector<double>& pos_z,
		  const double x_step_size=-1);
    
    /// Default destructor
    ~QWeightPoint(){}

    FlashMatch_t Match(const QCluster_t&, const Flash_t&);

    void SetStepSize(const double x)  { _x_step_size = x; }

    void SetMaxZDiff(const double dz) { _zdiff_max = dz;  }

  private:

    std::vector<double> _pos_x;
    std::vector<double> _pos_y;
    std::vector<double> _pos_z;
    double _x_step_size;
    double _zdiff_max; ///< allowed diff in z-direction to be considered as a match
  };
}
#endif
/** @} */ // end of doxygen group 

