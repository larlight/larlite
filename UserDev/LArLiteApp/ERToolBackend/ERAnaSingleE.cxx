#ifndef LARLITE_ERANASINGLEE_CXX
#define LARLITE_ERANASINGLEE_CXX

#include "ERAnaSingleE.h"

namespace larlite {

  ERAnaSingleE::ERAnaSingleE() : ERToolAnaBase()
  { 
    _name="ERAnaSingleE"; 
  }

  bool ERAnaSingleE::initialize() {
    
    _numEvts = 0;
    _singleE = 0;
    
    return ERToolAnaBase::initialize();
  }
  
  bool ERAnaSingleE::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;
    _mgr.Process();

    _numEvts += 1;

    auto const& particles = _mgr.ParticleSet();
    // Search for an unknown particle in particle Set
    int singles = 0;
    for (auto& p : particles)
      if (p.PdgCode() == 14) { singles += 1; }
    if (singles == 1)
      _singleE += 1;
    
  
    return true;
  }

  bool ERAnaSingleE::finalize() {

    std::cout << "RESULTS: " << std::endl
	      << "Tot Events    : " << _numEvts << std::endl
	      << "SingleE Events: " << _singleE << std::endl;

    return ERToolAnaBase::finalize();
  }

}
#endif
