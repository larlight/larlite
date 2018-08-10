#ifndef __LEE_PRECUT_ALGO_h__
#define __LEE_PRECUT_ALGO_h__

//C++ stuff
#include <vector>

// config/storage from LArCV
//#include "Base/LArCVBaseUtilFunc.h"
//#include "Base/DataCoordinator.h"

//larlite stuff
//#include "Base/DataFormatConstants.h"
//#include "DataFormat/ophit.h"


namespace leeprecuts {

  class LEEPreCutAlgo {
    
  public:
    LEEPreCutAlgo() {};
    virtual ~LEEPreCutAlgo() {};

    bool CoordsContained(float x, float y, float z, float edge_x, float edge_y, float edge_z);

    bool EventContained(std::vector<float> nuEndCoords,std::vector<float> leptStartCoords,std::vector<float> lepEndCoords);

    bool IsCCQE(int interactionCode);

    //std::vector<float> MakeTimeBin( const larlite::event_ophit& ophitList,int timeBinning,int beamWinStart,int beamWinEnd);
    std::vector<float> MakeTimeBin(const std::vector<float>& ophit_peaktime_v, const std::vector<float>& ophit_pe_v, int timeBinning,int beamWinStart,int beamWinEnd);
    
    std::vector<float> GetTotalPE(float coincThresh, std::vector<float> flashes);
    
    float PMTMaxFrac(const std::vector<float>& ophit_peaktime_v, const std::vector<float>& ophit_pe_v, const std::vector<int>& ophit_femch_v,
		     std::vector<float> flashBins, int timeBinning, float Winstart);

    //float PMTMaxFrac( const larlite::event_ophit& ophitList, std::vector<float> flashBins,float totPE,int timeBinning,float Winstart);

  };

}

#endif

