/**
 * \file Default3DParamsAlg.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class Default3DParamsAlg
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef DEFAULT3DPARAMSALG_H
#define DEFAULT3DPARAMSALG_H

#include <iostream>
#include "Cluster3DRecoUtil/Base/Cluster3DParamsAlg.h"
#include "GetAverages3D.h"


namespace cluster3D {


  /**
     \class Default3DParamsAlg
     User defined class Default3DParamsAlg ... these comments are used to generate
     doxygen documentation!
   */
  class Default3DParamsAlg : public Cluster3DParamsAlg {

  public:

    /// Default constructor
    Default3DParamsAlg();

    /// Default destructor
    ~Default3DParamsAlg(){}

  };


} // cluster3D


#endif
/** @} */ // end of doxygen group 

