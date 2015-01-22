#ifndef LARLITE_EXAMPLEERSELECTION_CXX
#define LARLITE_EXAMPLEERSELECTION_CXX

#include "ExampleERSelection.h"

namespace larlite {

  ExampleERSelection::ExampleERSelection() : ERToolAnaBase()
  { 
    _name="ExampleERSelection"; 
  }

  bool ExampleERSelection::initialize() {

    _mgr.Initialize();
    return ERToolAnaBase::initialize();

  }
  
  bool ExampleERSelection::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    _particles = _mgr.Process(_data);
  
    return true;
  }

  bool ExampleERSelection::finalize() {

    _mgr.Finalize(_fout);
    return ERToolAnaBase::finalize();

  }

}
#endif
