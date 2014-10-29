/**
 * \file TrainingModule.h
 *
 * \ingroup TrainingModule
 * 
 * \brief Class def header for a class TrainingModule
 *
 * @author cadams
 */

/** \addtogroup TrainingModule

    @{*/
#ifndef RECOTOOL_TRAININGMODULE_H
#define RECOTOOL_TRAININGMODULE_H


#include <iostream>
#include <stdarg.h>

#include <vector>
#include <string>
#include "floatfann.h"
#include "fann_cpp.h"

/**
   \class TrainingModule
   User defined class TrainingModule ... these comments are used to generate
   doxygen documentation!
 */
namespace cluster{
  class TrainingModule{

  public:

    /// Default constructor
    TrainingModule(){
      fFeatureVectorLength = 2;
      fOutputVectorLength = 1;
      fOutputFileName = "default.net";
      fNumHiddenLayers = 0;
      fHiddenLayerLength.clear();
    }

    /// Default destructor
    //~TrainingModule(){ if (ann) fann_destroy(ann);}
    ~TrainingModule(){}

    // some functions for controlling the input and output to ANN:
    
    /**
     * Sets the length of the feature vector input to the fann
     * @param l desired length of the feature vector
     */
    void setFeatureVectorLength(int l){fFeatureVectorLength = l;}

    /**
     * Sets the length of the output vector input to the fann
     * @param l desired length of the output vector
     */
    void setOutputVectorLength(int l){fOutputVectorLength = l;}

    /**
     * Sets the output file name
     * @param s output file name
     */
    void setOutputFileName(std::string s){fOutputFileName = s;}

    /**
     * call this function to actually initialize the network.
     */
    void init();

    /**
     * Runs the fann training alg on 1 iteration of input data
     * @param data  array of double with input data, must be length set by setFeatureVectorLength
     * @param truth array of double with truth data, must be length set by setOutputVectorLength
     */
    void trainOnData(float * data, float * truth);
    void trainOnData(std::vector<float>& data,
                     std::vector<float>& truth);

    void run(float * data);
    void run(std::vector<float> & data);

    void print_error(){std::cout << "Current Error: "<< ann.get_MSE() << std::endl;}
    /**
     * saves the current state of the fann to a file
     * @param s can override OutputFileName with s
     */
    void saveFANNToFile(std::string s = "");

    void configureHiddenLayers(int numLayers, ... );


  private:
    double fFeatureVectorLength;
    double fOutputVectorLength;
    std::string fOutputFileName;

    int    fNumHiddenLayers;
    std::vector<int> fHiddenLayerLength;

  public:
    // this is the actual neural network.  Right now only one per class:
    FANN::neural_net ann;

  };
}

#endif
/** @} */ // end of doxygen group 

