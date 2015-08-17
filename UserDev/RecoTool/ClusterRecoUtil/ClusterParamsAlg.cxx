#ifndef CLUSTERPARAMSALG_CXX
#define CLUSTERPARAMSALG_CXX

#include "ClusterParamsAlg.h"
#include "LArUtil/GeometryHelper.h"

namespace cluster{

  void ClusterParamsAlg::FillParams(){

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
      module -> do_params_fill(fParams, _verbose);
      _moduleTimes[nmodule] += 1000*localWatch.RealTime();

      if (_debug){
	std::cout << "Listing the changes made by " << module -> name() << ":" << std::endl;
	fParams.ReportDiff(localCopy);
	localCopy = fParams;
      }
      nmodule += 1;
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

    UChar_t plane = larutil::Geometry::GetME()->ChannelToPlane(inputHits[0].Channel());
    std::vector<Hit2D> tempHits;
    Hit2D temp;

    for (int i = 0; i < inputHits.size(); i++ ){
      temp.w = inputHits[i].WireID().Wire * geoHelper -> WireToCm();
      temp.t = inputHits[i].PeakTime() * geoHelper -> TimeToCm();
      temp.charge = inputHits[i].Integral();
      temp.plane = plane;
      tempHits.push_back(temp);
    }

    fParams.hit_vector = tempHits;

    return tempHits.size();

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


}    

#endif
