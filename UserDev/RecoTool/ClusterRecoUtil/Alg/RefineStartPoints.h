/**
 * \file RefineStartPoints.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class RefineStartPoints
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef REFINESTARTPOINTS_H
#define REFINESTARTPOINTS_H

#include <iostream>
#include <string>
#include "ParamsAlgBase.h"

namespace cluster {


  /**
     \class RefineStartPoints
     User defined class RefineStartPoints ... these comments are used to generate
     doxygen documentation!
   */
  class RefineStartPoints : public ParamsAlgBase {

  public:

    /// Default constructor
    RefineStartPoints();

    /// Default destructor
    ~RefineStartPoints(){}

    void do_params_fill(cluster_params &);

    void SetMaxPerpDist(double d) { _max_d_perp = d; }

  private:

    /// maximum perpendicular distance squared
    /// to the start_dir line anchored at the start point
    double _max_d_perp;

  };

} // cluster

#endif
/** @} */ // end of doxygen group 

