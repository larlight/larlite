#ifndef LARLITE_ERANAPI0RECO_CXX
#define LARLITE_ERANAPI0RECO_CXX

#include "ERAnaPi0Reco.h"

namespace larlite {

  ERAnaPi0Reco::ERAnaPi0Reco() : ERToolAnaBase()
  { 
    _name="ERAnaPi0Reco"; 
  }

  bool ERAnaPi0Reco::initialize() {

    return ERToolAnaBase::initialize();

  }
  
  bool ERAnaPi0Reco::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    return _mgr.Process();
  
    return true;
  }

  bool ERAnaPi0Reco::finalize() {

    return ERToolAnaBase::finalize();

  }

}
#endif
