#ifndef CLUSTERPARAMSALG_CXX
#define CLUSTERPARAMSALG_CXX

#include "ClusterParamsAlg.h"
#include "LArUtil/GeometryHelper.h"

namespace cluster{

  void ClusterParamsAlg::FillParams(){



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

      for (auto & module : _modules){
        module -> do_params_fill(fParams, _verbose);
        if (_debug){
          fParams.ReportDiff(localCopy);
          localCopy = fParams;
        }

      }

      return;

      // return result;
    }

  int ClusterParamsAlg::SetHits(const std::vector<Hit2D> &){
    return 0;
  }

  int ClusterParamsAlg::SetHits(const std::vector< ::larlite::hit> & inputHits){

    fParams.Clear();

    auto geoHelper = larutil::GeometryHelper::GetME();

    std::vector<Hit2D> tempHits;
    Hit2D temp;

    for (int i = 0; i < inputHits.size(); i++ ){
      temp.w = inputHits[i].WireID().Wire * geoHelper -> WireToCm();
      temp.t = inputHits[i].PeakTime() * geoHelper -> TimeToCm();
      temp.charge = inputHits[i].Integral();
      tempHits.push_back(temp);
    }

    fParams.hit_vector = tempHits;

    return tempHits.size();

  }


}    

#endif
