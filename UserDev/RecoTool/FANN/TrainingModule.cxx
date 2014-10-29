#ifndef RECOTOOL_TRAININGMODULE_CXX
#define RECOTOOL_TRAININGMODULE_CXX

#include "TrainingModule.h"
#include "LArUtil/LArUtilManager.h"

namespace cluster{
  void TrainingModule::configureHiddenLayers(int numLayers, ...){

    fHiddenLayerLength.clear();
    fNumHiddenLayers = numLayers;
    fHiddenLayerLength.reserve(numLayers);

    int i;
    va_list vl;
    va_start(vl,numLayers);
    for (i=1;i<numLayers;i++)
    {
      fHiddenLayerLength.push_back(va_arg(vl,int));
    }
    va_end(vl);
    return;
  }

  void TrainingModule::saveFANNToFile(std::string s){
    if (s != "")
      ann.save(s.c_str());
    else 
      ann.save(fOutputFileName.c_str());

    return;
  }

  void TrainingModule::init(){

    larutil::LArUtilManager::Reconfigure(larlite::geo::kArgoNeuT);

    /*
    if (ann){
      std::cerr << "ERROR: ann is already initialized, aborting.\n";
      return; 
    }
    */

    unsigned int * layers;
    layers = new unsigned int[fNumHiddenLayers + 2];
    layers[0] = fFeatureVectorLength;
    for (int i = 1; i <= fNumHiddenLayers; i++)
      layers[i] = fHiddenLayerLength[i-1];
    layers[fNumHiddenLayers+1] = fOutputVectorLength;
    ann.create_shortcut_array(fNumHiddenLayers+2, layers); 
    ann.print_parameters();
    return;
  }

  void TrainingModule::trainOnData(float * data, float * truth){
    ann.train(data, truth);
  }

  void TrainingModule::trainOnData(std::vector<float> & data, std::vector<float> & truth){

    if (data.size() != 0 && truth.size() != 0){
      ann.train(&data[0], &truth[0]);
    }
  }

  void TrainingModule::run(float * data){
    float * result = ann.run(data);
    for (int i = 0; i < fOutputVectorLength; i ++){
      std::cout << "result " << i << " is : " << result[i] << std::endl; 
    }
  }

  void TrainingModule::run(std::vector<float> & data){
    float * result = ann.run(&data[0]);
    for (int i = 0; i < fOutputVectorLength; i ++){
      std::cout << "result " << i << " is : " << result[i] << std::endl; 
    }
  }

}

#endif
