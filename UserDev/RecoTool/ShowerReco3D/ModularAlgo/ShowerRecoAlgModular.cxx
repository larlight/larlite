#ifndef SHOWERRECOALGMODULAR_CXX
#define SHOWERRECOALGMODULAR_CXX

#include "ShowerRecoAlgModular.h"

namespace showerreco {

    Shower_t ShowerRecoAlgModular::RecoOneShower(const ShowerClusterSet_t& clusters){
      // Run over the shower reco modules:
      Shower_t result;

      for (auto & module : _modules){
        module -> do_reconstruction(clusters, result);
      }

      return result;
    }
    
    void ShowerRecoAlgModular::AddShowerRecoModule( ShowerRecoModuleBase * module){
      _modules.push_back(module);
    }

    void ShowerRecoAlgModular::InsertShowerRecoModule(ShowerRecoModuleBase * module, unsigned int index){
      if (_modules.size() > index){
        _modules.insert(_modules.begin() + index, module);
      }
      else{
        std::cerr << "WARNING: Request to insert module at index " << index << " which is beyond the last index." << std::endl;
      }
    }

    void ShowerRecoAlgModular::InsertShowerRecoModule(ShowerRecoModuleBase * module, std::string name){
      for (unsigned int index = 0; index < _modules.size(); index ++){
        if (_modules.at(index)->name() == name){
          _modules.insert(_modules.begin() + index, module);
          return;
        }
      }
      std::cerr << "WARNING: Request to insert module after non existing module \"" << name << "\"" << std::endl;

    }
    

    void ShowerRecoAlgModular::ReplaceShowerRecoModule( ShowerRecoModuleBase * module, unsigned int index){
      if (_modules.size() > index){
        _modules.at(index) = module;
      }
      else{
        std::cerr << "WARNING: Request to remove non existing module at index " << index << std::endl;
      }
    }

    
    void ShowerRecoAlgModular::ReplaceShowerRecoModule( ShowerRecoModuleBase * module, std::string name){

      for (unsigned int index = 0; index < _modules.size(); index ++){
        if (_modules.at(index)->name() == name){
          _modules.at(index) = module;
          return;
        }
      }
      std::cerr << "WARNING: Request to remove non existing module \"" << name << "\"" << std::endl;

    }

    void ShowerRecoAlgModular::PrintModuleList(){

      std::cout << "Print the list of modules to run in Shower Reco Alg Modular:\n";
      int i = 0;
      for (auto & module : _modules){
        std::cout << "\t" << i << ") " << module -> name() << "\n";
        i++;
      }

    }

} // showerreco

#endif
