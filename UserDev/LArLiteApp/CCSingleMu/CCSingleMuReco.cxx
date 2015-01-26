#ifndef CCSINGLEMURECO_CXX
#define CCSINGLEMURECO_CXX

#include "CCSingleMuReco.h"

namespace larlite {

  CCSingleMuReco::CCSingleMuReco() : ERToolAnaBase()
  {
    _name = "CCSingleMuReco";
  }

  bool CCSingleMuReco::initialize() {

    _mgr.Initialize();
    return ERToolAnaBase::initialize();
  }
  
  bool CCSingleMuReco::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    _RecoParticles = _mgr.Process(_data);

    return true;
  }

  bool CCSingleMuReco::finalize() {

    _mgr.Finalize(_fout);
    return ERToolAnaBase::finalize();

  }

}
#endif
