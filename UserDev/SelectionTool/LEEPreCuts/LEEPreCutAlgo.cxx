
//PreCu stuff
#include "LEEPreCutAlgo.h"

// C++
#include <algorithm>
#include <utility>

namespace leeprecuts {

  bool LEEPreCutAlgo::CoordsContained(float x, float y, float z, float edge_x, float edge_y, float edge_z) {

    float xmax =  256.25;
    float xmin =  0.;
    float ymax =  116.5;
    float ymin = -116.5;
    float zmax =  1036.8;
    float zmin =  0;

    if (x<(xmax-edge_x) && x>(xmin+edge_x) && y<(ymax-edge_y) && y>(ymin+edge_y) && z<(zmax-edge_z) && z>(zmin+edge_z)){
      return true;
    } 
    else {
	return false;
    }

  }


  bool LEEPreCutAlgo::EventContained(std::vector<float> nuEndCoords,std::vector<float> lepStartCoords,std::vector<float> lepEndCoords) {

    float fidCut = 5.0;

    bool NuContained = CoordsContained(nuEndCoords[0],nuEndCoords[1],nuEndCoords[2],fidCut,fidCut,fidCut);
    bool LepStartContained = CoordsContained(lepStartCoords[0],lepStartCoords[1],lepStartCoords[2],fidCut,fidCut,fidCut);
    bool LepEndContained = CoordsContained(lepEndCoords[0],lepEndCoords[1],lepEndCoords[2],fidCut,fidCut,fidCut);

    if (NuContained && LepStartContained && LepEndContained) {
      return true;
    }
    else {
      return false;
    }

  }

  bool LEEPreCutAlgo::IsCCQE(int interactionCode) {
    if (interactionCode == 1001) {
      return true;
    }
    else {
      return false;
    }
  }


  /*
  std::vector<float> LEEPreCutAlgo::MakeTimeBin(const larlite::event_ophit& ophitList, int timeBinning,int beamWinStart,int beamWinEnd) {

    int numBins    = (beamWinEnd - beamWinStart)/timeBinning + 1;
    float tickSize = 0.015625;

    std::vector<float> PEbyTimeBin(numBins,0.);

    for (unsigned int i=0; i<ophitList.size(); i++) {
      
      if (ophitList[i].PeakTime()/tickSize > beamWinEnd || ophitList[i].PeakTime()/tickSize < beamWinStart) {
	continue;
      }
      
      PEbyTimeBin[(int)((ophitList[i].PeakTime()/tickSize - beamWinStart)/timeBinning)]+=ophitList[i].PE();

    }

    return PEbyTimeBin;

  }
  */

  std::vector<float> LEEPreCutAlgo::MakeTimeBin(const std::vector<float>& ophit_peaktime_v, const std::vector<float>& ophit_pe_v, int timeBinning,int beamWinStart,int beamWinEnd) {

    int numBins    = (beamWinEnd - beamWinStart)/timeBinning + 1;
    float tickSize = 0.015625;

    std::vector<float> PEbyTimeBin(numBins,0.);

    for (unsigned int i=0; i<ophit_peaktime_v.size(); i++) {
      
      if (ophit_peaktime_v[i]/tickSize > beamWinEnd || ophit_peaktime_v[i]/tickSize < beamWinStart) {
	continue;
      }

      int time_bin = ((ophit_peaktime_v[i]/tickSize - beamWinStart)/timeBinning);
      PEbyTimeBin[time_bin]+=ophit_pe_v[i];
    }

    return PEbyTimeBin;

  }
  

  std::vector<float> LEEPreCutAlgo::GetTotalPE(float coincThresh, std::vector<float> flashes) {

    float totPE = 0;
    bool flashFound = false;
    std::vector<float> PEinfo;
    std::vector<float> flashBins;

    for (unsigned int i=0; i<flashes.size(); i++) {

      if (flashes[i] > coincThresh && flashFound == true) {
	flashBins.push_back(i);
	totPE+=flashes[i];
      }

      if (flashes[i] < coincThresh && flashFound == true) {
	break;
      }

      if (flashes[i] > coincThresh && flashFound == false) {
	totPE+=flashes[i];
	flashBins.push_back(i);
	flashFound = true;
      }

    }

    PEinfo.push_back(totPE);
    for (unsigned int i=0; i < flashBins.size(); i++) {
      PEinfo.push_back(flashBins[i]);
    }

    return PEinfo;

  }

  /*
  float LEEPreCutAlgo::PMTMaxFrac(const larlite::event_ophit& ophitList,std::vector<float> flashBins,float totPE,int timeBinning, float Winstart) {

    std::vector<float> PEFracbyPMT(32,0.);
    float maxFrac;

    for (unsigned int i=0; i<ophitList.size(); i++) {

      int ophitBin = (ophitList[i].PeakTime() - Winstart)/timeBinning;

      if (std::find(flashBins.begin(),flashBins.end(),ophitBin)!=flashBins.end()) {
	PEFracbyPMT[ophitList[i].OpChannel()]+=ophitList[i].PE()/totPE;
      }

    }
    
    maxFrac = *max_element(PEFracbyPMT.begin(),PEFracbyPMT.end());

    return maxFrac;

  }
  */

  float LEEPreCutAlgo::PMTMaxFrac(const std::vector<float>& ophit_peaktime_v, const std::vector<float>& ophit_pe_v, const std::vector<int>& ophit_femch_v,
				  std::vector<float> flashBins,int timeBinning, float Winstart) {

    std::vector<float> PEFracbyPMT(32,0.);
    std::vector<float> binsAboveThresh;

    for ( unsigned int j = 1; j < flashBins.size(); j++) {

      binsAboveThresh.push_back(flashBins.at(j));
    }

    float maxFrac;
    float totalEventPE = (float)flashBins.at(0);
    float tickSize = 0.015625;
    
    for (unsigned int i=0; i<ophit_peaktime_v.size(); i++) {

      int ophitBin = (ophit_peaktime_v[i]/tickSize - Winstart)/timeBinning;

      if (std::find(binsAboveThresh.begin(),binsAboveThresh.end(),ophitBin)!=binsAboveThresh.end()) {
	PEFracbyPMT[ophit_femch_v[i]]+=ophit_pe_v[i]/totalEventPE;
      }

    }
    
    maxFrac = *max_element(PEFracbyPMT.begin(),PEFracbyPMT.end());

    return maxFrac;

  }
  
    


}
