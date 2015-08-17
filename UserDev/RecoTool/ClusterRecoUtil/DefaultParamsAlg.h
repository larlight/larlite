/**
 * \file DefaultParamsAlg.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class DefaultParamsAlg
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef DEFAULTPARAMSALG_H
#define DEFAULTPARAMSALG_H

#include <iostream>
#include "ClusterParamsAlg.h"
#include "Alg/GetAverages.h"
#include "Alg/FillPolygon.h"
#include "Alg/GetRoughStartPoint.h"
#include "Alg/RefineDirection.h"
#include "Alg/RefineStartPoints.h"
#include "Alg/FindShoweringPoint.h"

namespace cluster {


  /**
     \class DefaultParamsAlg
     User defined class DefaultParamsAlg ... these comments are used to generate
     doxygen documentation!
   */
  class DefaultParamsAlg : public ClusterParamsAlg {

  public:

    /// Default constructor
    DefaultParamsAlg();

    /// Default destructor
    ~DefaultParamsAlg(){}

  };


} // cluster


#endif
/** @} */ // end of doxygen group 

