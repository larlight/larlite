#ifndef CCSINGLEMURECO_CXX
#define CCSINGLEMURECO_CXX

#include "CCSingleMuReco.h"

namespace larlite {

  CCSingleMuReco::CCSingleMuReco() : ERToolAnaBase()
  {
    _name = "CCSingleMuReco";
  }

  bool CCSingleMuReco::initialize() {

    return ERToolAnaBase::initialize();
  }
  
  bool CCSingleMuReco::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    return _mgr.Process();
  }

  bool CCSingleMuReco::finalize() {

    return ERToolAnaBase::finalize();

  }

}
#endif
