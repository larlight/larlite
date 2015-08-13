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
    GetAverages(){}

    /// Default destructor
    ~GetAverages(){}

    void do_params_fill(cluster_params &, bool verbose = false);

  };

} // cluster


#endif
/** @} */ // end of doxygen group 

