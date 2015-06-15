#ifndef BASICTOOL_FHICLLITE_CONFIGMANAGER_CXX
#define BASICTOOL_FHICLLITE_CONFIGMANAGER_CXX

#include <fstream>
#include <sstream>
#include "ConfigManager.h"
#include "FhiclLiteException.h"

namespace fcllite {
  ConfigManager::ConfigManager(const std::string& name)
    : _data(name)
  {
  }

  void ConfigManager::Reset()
  { _data.clear(); }
  
  void ConfigManager::AddCfgFile(std::string fname)
  {
    std::ifstream filestrm(fname.c_str());
    std::string   contents;
    std::string   line;

    while(std::getline(filestrm, line)) {

      if(line.empty()) continue;
      
      std::stringstream linestrm(line);
      std::string       valid_line;
      
      std::getline(linestrm, valid_line, '#');
      
      if(valid_line.empty()) continue;
      
      contents += " " + valid_line;
    }
    
    _data.add(contents);
    filestrm.close();
    
    //linestream >> val1 >> val2;
  }
}

#endif
