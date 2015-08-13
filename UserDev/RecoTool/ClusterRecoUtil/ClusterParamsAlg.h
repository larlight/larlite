/**
 * \file ClusterParamsAlg.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class ClusterParamsAlg
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CLUSTERPARAMSALG_H
#define CLUSTERPARAMSALG_H

#include <iostream>
#include <vector>
#include "ClusterParams.h"
#include "DataFormat/hit.h"

namespace cluster{

  /**
     \class ClusterParamsAlg
     User defined class ClusterParamsAlg ... these comments are used to generate
     doxygen documentation!
   */
  class ClusterParamsAlg{

  public:

    /// Default constructor
    ClusterParamsAlg(){}

    /// Default destructor
    ~ClusterParamsAlg(){}

    void FillParams(cluster_params & cluster);

    void SetHits(const std::vector<Hit2D> &);
    int SetHits(const std::vector<const ::larlite::hit*> &);

  private:

    bool verbose;
    bool debug;

  };

} // cluster

#endif
/** @} */ // end of doxygen group 

