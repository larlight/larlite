#ifndef CLUSTER3DPARAMSALG_CXX
#define CLUSTER3DPARAMSALG_CXX

#include "Cluster3DParamsAlg.h"

namespace cluster3D{

  void Cluster3DParamsAlg::FillParams(cluster3D_params & fParams){

    // Figure out if it's worth running on this cluster params, basically if there are enough hits:
    if (fParams.point_vector.size() < _min_hits) return;

    // increase cluster counter by one
    _nClusters += 1;

    // prepare stopwatch
    TStopwatch localWatch;

    // static int i = 0;
    // i ++;
    // if (i % 10000 == 0)
    //     std::cin.get();

    
    cluster3D_params localCopy;
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
  



  void Cluster3DParamsAlg::ReportTimes() const
  {
    std::cout.precision(3);
    //std::scientific;

    for (size_t m=0; m < _modules.size(); m++)
      std::cout << "  \033[95m<<" << _modules.at(m)->name() << " Time>>\033[00m  : "
                << _moduleTimes[m]/1000. << " [s] ... or "
                << 1000.*(_moduleTimes[m]/_nClusters) << " [us/cluster]" << std::endl;
  }

  Cluster3DParamsAlg::~Cluster3DParamsAlg(){
    // Attempt to safely delete the algs:
    for(unsigned int i = 0; i < _modules.size(); i ++ ){
      if (_modules.at(i)) delete _modules.at(i);
    }
  }


}    

#endif
