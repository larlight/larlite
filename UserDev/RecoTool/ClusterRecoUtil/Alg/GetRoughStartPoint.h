/**
 * \file GetRoughStartPoint.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class GetRoughStartPoint
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef GETROUGHSTARTPOINT_H
#define GETROUGHSTARTPOINT_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class GetRoughStartPoint
     User defined class GetRoughStartPoint ... these comments are used to generate
     doxygen documentation!
   */
  class GetRoughStartPoint : public ParamsAlgBase {

  public:

    /// Default constructor
    GetRoughStartPoint(){_name = "GetRoughStartPoint";}

    /// Default destructor
    ~GetRoughStartPoint(){}

    void do_params_fill(cluster_params &, bool verbose = false);

    /**
     * @brief Set fraction of charge that needs to be enclosed by polygon
     */
    void setFrac(const double f) { _frac = f; }

  private:

    /// Fraction of cluster charge that needs to be included in the polygon
    double _frac;

  };

} // cluster


#endif
/** @} */ // end of doxygen group 

