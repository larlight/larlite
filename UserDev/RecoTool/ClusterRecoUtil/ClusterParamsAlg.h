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
#include "Alg/ParamsAlgBase.h"
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

    void attachAlg(ParamsAlgBase * alg){_modules.push_back(alg);}

    void FillParams();

    /**
     * @brief set the hits of the params object
     * @details These functions will convert, if necessary, the hits and
     * then save them into the params object
     * 
     * @param a vector of hits, either pointer to larlite hits, Hit2D
     */
    int SetHits(const std::vector<Hit2D> &);
    int SetHits(const std::vector< ::larlite::hit> &);

    void SetVerbose(bool yes=true){ _verbose = yes;}
    void SetDebug(bool yes=true)  { _debug = yes;}

  private:

    std::vector<::cluster::ParamsAlgBase * > _modules;

    bool _verbose;
    bool _debug;

    cluster_params fParams;

  };

} // cluster

#endif
/** @} */ // end of doxygen group 

