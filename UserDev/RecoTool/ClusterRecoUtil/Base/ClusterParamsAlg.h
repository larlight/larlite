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
#include "ClusterRecoUtil/Alg/ParamsAlgBase.h"
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
    ClusterParamsAlg(){_nClusters=0;_verbose=false;_debug=false;_min_hits=2;}

    /// Default destructor
    ~ClusterParamsAlg();

    void attachAlg(ParamsAlgBase * alg){_modules.push_back(alg); _moduleTimes.push_back(0); }

    void FillParams(cluster_params &);



    void SetVerbose(bool yes=true){ _verbose = yes;}
    void SetDebug(bool yes=true)  { _debug = yes;}

    void SetMinHits(int i = 1)    { _min_hits = i;}

    /**
     * @brief Report average time spent per algorithm
     */
    void ReportTimes() const;

  protected:

    std::vector<::cluster::ParamsAlgBase * > _modules;
    
    /// Time it takes each module to run
    /// times are added continuously -> average at the end
    std::vector<double> _moduleTimes;

    /// number of clusters scanned (for stopwatch time calculation)
    int _nClusters;

    int _min_hits;

    bool _verbose;
    bool _debug;


  };

} // cluster

#endif
/** @} */ // end of doxygen group 

