/**
 * \file FindShoweringPoint.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class FindShoweringPoint
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef FINDSHOWERINGPOINT_H
#define FINDSHOWERINGPOINT_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class FindShoweringPoint
     User defined class FindShoweringPoint ... these comments are used to generate
     doxygen documentation!
   */
  class FindShoweringPoint : public ParamsAlgBase {

  public:

    /// Default constructor
    FindShoweringPoint(){_name = "FindShoweringPoint";}

    /// Default destructor
    ~FindShoweringPoint(){}

    void do_params_fill(cluster_params &);

  private:

  };

} // cluster


#endif
/** @} */ // end of doxygen group 
