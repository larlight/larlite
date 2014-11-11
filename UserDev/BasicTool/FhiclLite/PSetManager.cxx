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

  void PSetManager::addParameter(std::string key, std::string param){

    if (!_currentSet){
      std::cout << "should through exeption here!" << std::endl;
      return;
    }
    else
      _currentSet->set(key, param);
      
    return;
  }


  void PSetManager::closeCurrentSet(){

    //need to make the top = to the curren't parent...

    _currentSet = _topSet;
    _topSet = 0;

    return;
  }

  void PSetManager::addSubSet(std::string key){

    //create a new subset of the current set

    if (!_currentSet){
      std::cout << "should through exception..." << std::endl;
      return;
    }
    else{
      // a set is open...create the new set as part of it
      _topSet = _currentSet;
      PSet newset(key.c_str());
      _topSet->set(key.c_str(), newset);
      _currentSet = &(_topSet->get_pset_writeable(key.c_str()));
    }

    return;
  }


}

#endif
