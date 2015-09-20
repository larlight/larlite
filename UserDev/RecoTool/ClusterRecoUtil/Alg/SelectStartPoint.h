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
    SelectStartPoint(){_name = "SelectStartPoint"; _frac = 0.95;}

    /// Default destructor
    ~SelectStartPoint(){}

    void do_params_fill(cluster_params &);

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

