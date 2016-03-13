//
// Example C++ routine to run instantiate your sample class
//

#include "FhiclLite/PSet.h"
#include "FhiclLite/ConfigManager.h"
#include <iostream>

int main(int argc, char** argv){

  ::fcllite::ConfigManager mgr;
  mgr.AddCfgFile(argv[1]);
  //std::cout<<mgr.Config().dump()<<std::endl;
  auto const a = mgr.Config().get<fcllite::PSet>("EmuDriver").get<fcllite::PSet>("ShowerEmuSmearingParametrized");
  //auto const a = mgr.Config();
  auto const keys = a.value_keys();
  std::cout<<std::endl<<std::endl;
  for(auto const& k : keys)
    std::cout<<"Key: "<<k<<std::endl<<"  Values: "<<a.get<std::string>(k)<<std::endl;
  //std::cout<<a.dump()<<std::endl;
  //std::cout<< a.get<std::string>("EResFunc") << std::endl;
  /*
  auto const a = mgr.Config().get<fcllite::PSet>("a");
  auto val = a.get<std::vector<int> >("j");
  std::cout<<"size: "<<val.size()<<" ... first: "<<val.front()<<std::endl;
  std::cout<<"x: "<<a.get<std::string>("x")<<std::endl;
  */
  return 0;
}
