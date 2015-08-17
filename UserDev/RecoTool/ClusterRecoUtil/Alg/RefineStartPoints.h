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
    RefineStartPoints(){_name = "RefineStartPoints";}

    /// Default destructor
    ~RefineStartPoints(){}


    /**
     * Calculates the following variables:
     * length
     * width
     * hit_density_1D
     * hit_density_2D
     * direction
     * start_point
     * end_point
     * @param override [description]
     */
    void do_params_fill(cluster_params &);

  private:

    // The following variables are used to select the local hit list
    float linearDistance;
    float orthogDistance;

  };

} // cluster

#endif
/** @} */ // end of doxygen group 

