#ifndef PSETMANAGER_CXX
#define PSETMANAGER_CXX

#include "PSetManager.h"

namespace fcl{


  void PSetManager::dump(){

    
    std::cout << "Number of Primary Sets in File: " << _set.size() << std::endl;
    for (size_t i=0; i < _set.size(); i++)
      std::cout << _set.at(i).dump(0).c_str();
    
    return;
  }

}

#endif
