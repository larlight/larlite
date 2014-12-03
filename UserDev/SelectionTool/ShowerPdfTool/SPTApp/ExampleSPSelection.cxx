#ifndef LARLITE_EXAMPLESPSELECTION_CXX
#define LARLITE_EXAMPLESPSELECTION_CXX

#include "ExampleSPSelection.h"

namespace larlite {

  ExampleSPSelection::ExampleSPSelection() : SPTAnaBase()
  { _name="ExampleSPSelection"; }

  bool ExampleSPSelection::initialize() {

    return SPTAnaBase::initialize();

  }
  
  bool ExampleSPSelection::analyze(storage_manager* storage) {

    auto status = SPTAnaBase::analyze(storage);
    if(!status) return status;

    _mgr.Process(_data);
  
    return true;
  }

  bool ExampleSPSelection::finalize() {

    return SPTAnaBase::finalize();

  }

}
#endif
