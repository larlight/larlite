/**
 * \file RefineDirection.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class RefineDirection
 *
 * @author david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef REFINEDIRECTION_H
#define REFINEDIRECTION_H

#include <iostream>
#include "ParamsAlgBase.h"

namespace cluster {

  /**
     \class RefineDirection
     User defined class RefineDirection ... these comments are used to generate
     doxygen documentation!
   */
  class RefineDirection : public ParamsAlgBase {

  public:

    /// Default constructor
    RefineDirection();

    /// Default destructor
    ~RefineDirection(){}

    void do_params_fill(cluster_params &);

    /**
     * @brief set percentage of hits to be used
     */
    void setPercentage(const double p) { _percentage = p; }

    /**
     * @brief Set number of bins to use
     */
    void setNBins(const int n) { _Nbins = n; }

    /**
     * @brief Set minimum charge for hits to be considered
     */
    void setQMin(const double q) { _Qmin = q; }

  private:

    /// Fraction of cluster charge that needs to be included in the polygon
    double _percentage;
    /// Number of bins to use
    int    _Nbins;
    /// minimum charge for a hit to be considered in this calculation
    double _Qmin;

  };

} // cluster


#endif
/** @} */ // end of doxygen group 

