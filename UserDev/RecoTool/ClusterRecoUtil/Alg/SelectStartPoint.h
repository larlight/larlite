/**
 * \file SelectStartPoint.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class SelectStartPoint
 *
 * @author corey adams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef SELECTSTARTPOINT_H
#define SELECTSTARTPOINT_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class SelectStartPoint
     User defined class SelectStartPoint ... these comments are used to generate
     doxygen documentation!
   */
  class SelectStartPoint : public ParamsAlgBase {

  public:

    /// Default constructor
    SelectStartPoint(){_name = "SelectStartPoint";}

    /// Default destructor
    ~SelectStartPoint(){}

    void do_params_fill(cluster_params &);

  private:
    
  };

} // cluster


#endif
/** @} */ // end of doxygen group 

