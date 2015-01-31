#ifndef LARLITE_EXAMPLEERSELECTION_CXX
#define LARLITE_EXAMPLEERSELECTION_CXX

#include "ExampleERSelection.h"

namespace larlite {

  ExampleERSelection::ExampleERSelection() : ERToolAnaBase()
  { 
    _name="ExampleERSelection"; 
  }

  bool ExampleERSelection::initialize() {

    return ERToolAnaBase::initialize();

  }
  
  bool ExampleERSelection::analyze(storage_manager* storage) {

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    return _mgr.Process();
  }

  bool ExampleERSelection::finalize() {

    return ERToolAnaBase::finalize();

  }

}
#endif
