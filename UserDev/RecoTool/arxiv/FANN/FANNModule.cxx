#ifndef RECOTOOL_FANNMODULE_CXX
#define RECOTOOL_FANNMODULE_CXX

#include "FANNModule.h"
#include "LArUtil/LArUtilManager.h"

namespace cluster{

  void FANNModule::LoadFromFile(std::string s){
    if (s != "") net.create_from_file(s);
    else
      net.create_from_file(fInputFileName);
    // net.print_parameters();
  }

  void FANNModule::init(){

    // larutil::LArUtilManager::Reconfigure(larlite::geo::kArgoNeuT);
    return;
  }


  void FANNModule::run(float * data){
    float * result = net.run(data);
    for (size_t i = 0; i < net.get_num_output(); i ++){
      std::cout << "result " << i << " is : " << result[i] << std::endl; 
    }
  }

  void FANNModule::run(std::vector<float> & data){
    float * result = net.run(&data[0]);
    for (size_t i = 0; i < net.get_num_output(); i ++){
      std::cout << "result " << i << " is : " << result[i] << std::endl; 
    }
  }

  void FANNModule::run(std::vector<float> & data, std::vector<float> & returnResult){
    float * result = net.run(&data[0]);
    if (returnResult.size() != net.get_num_output()){
      returnResult.resize(net.get_num_output());
    } 
    for (size_t i = 0; i < net.get_num_output(); i ++){
      returnResult[i] = result[i]; 
    }
    return;
  }

}

#endif
