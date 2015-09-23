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
    FindShoweringPoint();

    /// Default destructor
    ~FindShoweringPoint(){}


    void do_params_fill(cluster_params &);

    // set how far along the shower trunk we can go to search
    // for the showering point before stopping
    void SetMaxShoweringDistance(double d) { _dMax = d; }

    // set the minimum cosine required for the trunk
    // to be assumed to proceed in a straight line
    void SetMinCosine(double c) { _minCosine = c; }

  private:

    // maximum showering distance allowed
    double _dMax;

    // minimum value of cosine between directions of consecutive
    // hits along the trunk.
    // if the cosine becomes smaller than this value,
    // we assume we have exited a straight-line region
    // and stop searching
    double _minCosine;

  };

} // cluster


#endif
/** @} */ // end of doxygen group 
