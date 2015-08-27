#ifndef CLUSTERPARAMSALG_CXX
#define CLUSTERPARAMSALG_CXX

#include "ClusterParamsAlg.h"
#include "LArUtil/GeometryHelper.h"

namespace cluster{

  void ClusterParamsAlg::FillParams(cluster_params & fParams){

    // increase cluster counter by one
    _nClusters += 1;

    // prepare stopwatch
    TStopwatch localWatch;


    // Make sure the result cluster has the right size of all it's elements
    // auto geom = larutil::Geometry::GetME();
    // int nPlanes = geom -> Nplanes();
    // result.fTotalEnergy.resize(nPlanes);
    // result.fSigmaTotalEnergy.resize(nPlanes);
    // result.fdEdx.resize(nPlanes);
    // result.fSigmadEdx.resize(nPlanes);
    // result.fTotalMIPEnergy.resize(nPlanes);
    // result.fSigmaTotalMIPEnergy.resize(nPlanes);
    
    cluster_params localCopy;
    if (_debug)
      localCopy = fParams;
    
    // count modules
    int nmodule = 0;

    for (auto & module : _modules){
      
      localWatch.Start();
      if (_verbose) module -> SetVerbose();
      module -> do_params_fill(fParams);
      _moduleTimes[nmodule] += 1000*localWatch.RealTime();

      if (_debug){
        std::cout << "Listing the changes made by " << module -> name() << ":"
                  << " [" << _moduleTimes.back() << " s ]"
                  << std::endl;
        fParams.ReportDiff(localCopy);
        std::cout << std::endl;
        localCopy = fParams;
      }
      nmodule += 1;
    }

    if (_debug){
      std::cout << "Final cluster is :\n";
      fParams.Report();
    }
    
    return;
    
    // return result;
  }
  



  void ClusterParamsAlg::ReportTimes() const
  {
    std::cout.precision(3);
    //std::scientific;

    for (size_t m=0; m < _modules.size(); m++)
      std::cout << "  \033[95m<<" << _modules.at(m)->name() << " Time>>\033[00m  : "
                << _moduleTimes[m]/1000. << " [s] ... or "
                << 1000.*(_moduleTimes[m]/_nClusters) << " [us/cluster]" << std::endl;
  }

  ClusterParamsAlg::~ClusterParamsAlg(){
    // Attempt to safely delete the algs:
    for(unsigned int i = 0; i < _modules.size(); i ++ ){
      if (_modules.at(i)) delete _modules.at(i);
    }
  }

}    

#endif
