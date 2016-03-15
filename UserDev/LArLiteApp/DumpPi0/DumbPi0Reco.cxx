#ifndef LARLITE_DUMBPI0RECO_CXX
#define LARLITE_DUMBPI0RECO_CXX

#include "DumbPi0Reco.h"

namespace larlite {

  DumbPi0Reco::DumbPi0Reco() : ERToolAnaBase()
  { 
    _name="DumbPi0Reco"; 
  }

  bool DumbPi0Reco::initialize() {

    return ERToolAnaBase::initialize();

  }
  
  bool DumbPi0Reco::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    return _mgr.Process();
  
    return true;
  }

  bool DumbPi0Reco::finalize() {

    return ERToolAnaBase::finalize();

  }

}
#endif
