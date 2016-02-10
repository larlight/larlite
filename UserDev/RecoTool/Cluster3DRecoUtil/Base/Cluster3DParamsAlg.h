/**
 * \file Cluster3DParamsAlg.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class Cluster3DParamsAlg
 *
 * @author cadams
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef CLUSTER3DPARAMSALG_H
#define CLUSTER3DPARAMSALG_H

#include <iostream>
#include <vector>
#include "Cluster3DParams.h"
#include "Params3DAlgBase.h"
#include "DataFormat/hit.h"
#include "TStopwatch.h"

namespace cluster3D{

  /**
     \class Cluster3DParamsAlg
     User defined class Cluster3DParamsAlg ... these comments are used to generate
     doxygen documentation!
   */
  class Cluster3DParamsAlg{

  public:

    /// Default constructor
    Cluster3DParamsAlg(){_nClusters=0;_verbose=false;_debug=false;_min_hits=2;}

    /// Default destructor
    ~Cluster3DParamsAlg();

    void attachAlg(ParamsAlgBase * alg){_modules.push_back(alg); _moduleTimes.push_back(0); }

    void FillParams(cluster3D_params &);



    void SetVerbose(bool yes=true){ _verbose = yes;}
    void SetDebug(bool yes=true)  { _debug = yes;}

    void SetMinHits(int i = 1)    { _min_hits = i;}

    /**
     * @brief Report average time spent per algorithm
     */
    void ReportTimes() const;

  protected:

    std::vector<::cluster3D::ParamsAlgBase * > _modules;
    
    /// Time it takes each module to run
    /// times are added continuously -> average at the end
    std::vector<double> _moduleTimes;

    /// number of clusters scanned (for stopwatch time calculation)
    int _nClusters;

    size_t _min_hits;

    bool _verbose;
    bool _debug;


  };

} // cluster

#endif
/** @} */ // end of doxygen group 

