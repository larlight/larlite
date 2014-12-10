#ifndef SPANAGAMMASELECTION_CXX
#define SPANAGAMMASELECTION_CXX

#include "SPAnaGammaSelection.h"

namespace larlite {
  
  SPAnaGammaSelection::SPAnaGammaSelection()
    : SPTAnaBase()
    , _algo (nullptr)
    { _name="SPAnaGammaSelection"; _fout=0; }
  
  bool SPAnaGammaSelection::initialize() {
    
    auto status = SPTAnaBase::initialize();
    
    if(!_algo) {
      print(msg::kERROR,__FUNCTION__,"Algorithm not set. Aborting...");
      return false;
    }
    
    _algo->Init();
    
    return status;
  }
  
  bool SPAnaGammaSelection::analyze(storage_manager* storage) {
  
    auto status = SPTAnaBase::analyze(storage);
    if(!status) return status;
    
    ::sptool::SPAData input = _data;
    
    _algo->Fill(input);
    
    return status;
  }

  bool SPAnaGammaSelection::finalize() {
    
    auto status = SPTAnaBase::finalize();
  
    return status;
  }

}
#endif
