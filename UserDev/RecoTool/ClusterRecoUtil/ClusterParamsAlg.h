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
#include "TStopwatch.h"

namespace cluster{

  /**
     \class ClusterParamsAlg
     User defined class ClusterParamsAlg ... these comments are used to generate
     doxygen documentation!
   */
  class ClusterParamsAlg{

  public:

    /// Default constructor
    ClusterParamsAlg(){_nClusters=0;}

    /// Default destructor
    ~ClusterParamsAlg(){}

    void attachAlg(ParamsAlgBase * alg){_modules.push_back(alg); _moduleTimes.push_back(0); }

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

    cluster_params GetParams() { return fParams; }

    /**
     * @brief Report average time spent per algorithm
     */
    void ReportTimes() const;

  private:

    std::vector<::cluster::ParamsAlgBase * > _modules;
    
    /// Time it takes each module to run
    /// times are added continuously -> average at the end
    std::vector<double> _moduleTimes;

    /// number of clusters scanned (for stopwatch time calculation)
    int _nClusters;

    bool _verbose;
    bool _debug;

    cluster_params fParams;

  };

} // cluster

#endif
/** @} */ // end of doxygen group 

