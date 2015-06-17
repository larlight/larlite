#ifndef BASICTOOL_PSET_CXX
#define BASICTOOL_PSET_CXX

#include "PSet.h"

namespace fcl {

  void PSet::set(const std::string key,
		 const std::string value)
  {
    if( fPSets_m.find(key)  != fPSets_m.end()  ) fPSets_m.erase(key);
    if( fParams_m.find(key) != fParams_m.end() ) fParams_m.erase(key);

    fParams_m[key] = value;

  }

  void PSet::set(const std::string key,
		 const PSet ps)
  {
    if( fPSets_m.find(key)  != fPSets_m.end()  ) fPSets_m.erase(key);
    if( fParams_m.find(key) != fParams_m.end() ) fParams_m.erase(key);

    fPSets_m[key] = ps;
  }

  const std::string& PSet::get_param(std::string key) const
  {
    auto iter = fParams_m.find(key);
    if(iter == fParams_m.end()) throw FhiclLiteException(Form("Parameter \"%s\" not found!",key.c_str()));
    return (*iter).second;
  }
  
  const PSet& PSet::get_pset(std::string key) const
  {
    auto const iter = fPSets_m.find(key);
    if(iter == fPSets_m.end()) throw FhiclLiteException(Form("PSet \"%s\" not found!",key.c_str()));
    return (*iter).second;
  }

  PSet& PSet::get_pset_writeable(std::string key){
    auto iter = fPSets_m.find(key);
    if(iter == fPSets_m.end()) throw FhiclLiteException(Form("PSet \"%s\" not found!",key.c_str()));
    return (*iter).second;
  }

  std::string PSet::dump(int x) const
  {

    //input "x" is used to determine nesting & level of tabbing required for output
    std::string tabbing = "";
    for (int i=0; i < x; i++)
      tabbing += "\t";

    std::string res = fName;
    res += tabbing.c_str();
    res += " {\n";
    for(auto const& key_value : fParams_m) 

      res += Form("%s  %s : %s\n", tabbing.c_str(), key_value.first.c_str(), key_value.second.c_str());
    
    for(auto const& key_value : fPSets_m)

      res += Form("  %s", key_value.second.dump(x+1).c_str());

    res += Form("%s }\n",tabbing.c_str());
    
    return res;
  }


}

#endif
