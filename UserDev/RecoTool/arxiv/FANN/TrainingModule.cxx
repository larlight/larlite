#ifndef RECOTOOL_TRAININGMODULE_CXX
#define RECOTOOL_TRAININGMODULE_CXX

#include "TrainingModule.h"
#include "LArUtil/LArUtilManager.h"

namespace cluster{


  void TrainingModule::addLayer(int nodes){
    if (nodes > 0)
      fHiddenLayerLength.push_back(nodes);
  }

  void TrainingModule::saveFANNToFile(std::string s){
    if (s != "")
      ann.save(s.c_str());
    else 
      ann.save(fOutputFileName.c_str());

    return;
  }

  void TrainingModule::init(std::string file){

    // initialize the training data object:
    std::cout << "Initializing data from file " << file << "\n";
    trainingData.read_train_from_file(file);

    fNumHiddenLayers = fHiddenLayerLength.size();

    if( _use_cascade){
      ann.create_shortcut(2, trainingData.num_input_train_data(), trainingData.num_output_train_data()); 
    }
    else{
      unsigned int * layers;
      layers = new unsigned int[fNumHiddenLayers + 2];
      layers[0] = fFeatureVectorLength;
      for (int i = 1; i <= fNumHiddenLayers; i++)
        layers[i] = fHiddenLayerLength[i-1];
      layers[fNumHiddenLayers+1] = fOutputVectorLength;
      ann.create_standard_array(fNumHiddenLayers + 2, layers);
    }

    std::cout << ann.get_num_layers() << std::endl;
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


  void TrainingModule::train(){

    if (_use_cascade){
      const float desired_error = 0.1;
      const unsigned int max_neurons = 30;
      const unsigned int neurons_between_reports = 2;

      ann.cascadetrain_on_data(trainingData, max_neurons,
          neurons_between_reports, desired_error);
    }
    else{
      const float desired_error = 0.001;
      const unsigned int max_epochs = 10000;
      const unsigned int epochs_between_reports = 500;

      ann.train_on_data(trainingData, max_epochs,
          epochs_between_reports, desired_error);
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
