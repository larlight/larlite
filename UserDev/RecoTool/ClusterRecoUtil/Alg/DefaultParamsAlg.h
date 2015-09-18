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
#include "ClusterRecoUtil/Base/ClusterParamsAlg.h"
#include "GetAverages.h"
#include "FillPolygon.h"
#include "GetRoughStartPoint.h"
#include "RefineStartPoints.h"
#include "SelectStartPoint.h"
#include "FindShoweringPoint.h"
#include "FillGeomParams.h"

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

