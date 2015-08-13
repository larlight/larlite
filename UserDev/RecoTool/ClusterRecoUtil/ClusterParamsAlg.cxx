#ifndef CLUSTERPARAMSALG_CXX
#define CLUSTERPARAMSALG_CXX

#include "ClusterParamsAlg.h"

namespace cluster{

  void ClusterParamsAlg::FillParams(cluster_params & cluster){



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
       localCopy = cluster;

      for (auto & module : _modules){
        module -> do_params_fill(cluster, _verbose);
        if (_debug){
          cluster.ReportDiff(localCopy);
          localCopy = cluster;
        }

      }

      return;

      // return result;
    }

}    

#endif
