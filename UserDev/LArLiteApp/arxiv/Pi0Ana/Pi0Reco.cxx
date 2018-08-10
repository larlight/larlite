#ifndef LARLITE_PI0RECO_CXX
#define LARLITE_PI0RECO_CXX

#include "Pi0Reco.h"

namespace larlite {

  Pi0Reco::Pi0Reco() : ERToolAnaBase()
  { 
    _name="Pi0Reco"; 
  }

  bool Pi0Reco::initialize() {

    return ERToolAnaBase::initialize();

  }
  
  bool Pi0Reco::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    return _mgr.Process();
  
    return true;
  }

  bool Pi0Reco::finalize() {

    return ERToolAnaBase::finalize();

  }

}
#endif
