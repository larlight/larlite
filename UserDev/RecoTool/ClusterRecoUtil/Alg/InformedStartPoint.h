/**
 * \file InformedStartPoint.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class InformedStartPoint
 *
 *  This class will fill the proper members of cluster params
 *  related to start point finding based off of an informed guess as to
 *  where the start point is.
 *  
 *  The start point selected is still meant to be an actual hit in the cluster
 *  so this algorithm finds the closest hit to the suggested start point
 *  
 *  This isn't meant as a "cheating" algorithm, rather it's meant to incorporate
 *  multi plane info into shower reconstruction in 2D.
 *
 * @author corey adams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef INFORMEDSTARTPOINT_H
#define INFORMEDSTARTPOINT_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class InformedStartPoint
     User defined class InformedStartPoint ... these comments are used to generate
     doxygen documentation!
   */
  class InformedStartPoint : public ParamsAlgBase {

  public:

    /// Default constructor
    InformedStartPoint();

    /// Default destructor
    ~InformedStartPoint(){}

    void do_params_fill(cluster_params &);

    void set_start_point_guess(const larutil::Hit2D & point);
    void set_start_point_guess(const larutil::Point2D & point);

    // This function returns whether or not there is a guess
    // to the start point.
    // After completion of do_params_fill, this bool is set to false
    // So that the start point guess must be refilled every time.
    bool has_start_point_guess(){return _has_current_guess;}

  private:
    
    larutil::Hit2D _guess_start_point;

    bool _has_current_guess;

    // Really, it uses min distance squared.
    float _min_distance;

  };

} // cluster


#endif
/** @} */ // end of doxygen group 

