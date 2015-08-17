/**
 * \file GetAverages.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class GetAverages
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef GETAVERAGES_H
#define GETAVERAGES_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class GetAverages
     User defined class GetAverages ... these comments are used to generate
     doxygen documentation!
   */
  class GetAverages : public ParamsAlgBase {

  public:

    /// Default constructor
    GetAverages(){_name = "GetAverages";}

    /// Default destructor
    ~GetAverages(){}

    /**
       Calculates the following variables:
       mean_charge
       mean_x
       mean_y
       rms_x
       rms_y
       RMS_charge
       N_Hits_HC
       charge_wgt_x
       charge_wgt_y
       eigenvalue_principal
       eigenvalue_secondary
       multi_hit_wires
       N_Wires
       slope_2d
       slope_2d_high_q
      */
    void do_params_fill(cluster_params &);

  };

} // cluster


#endif
/** @} */ // end of doxygen group 

