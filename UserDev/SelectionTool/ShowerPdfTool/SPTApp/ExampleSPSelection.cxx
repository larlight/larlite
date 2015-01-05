#ifndef LARLITE_EXAMPLESPSELECTION_CXX
#define LARLITE_EXAMPLESPSELECTION_CXX

#include "ExampleSPSelection.h"

namespace larlite {

  ExampleSPSelection::ExampleSPSelection() : SPTAnaBase()
  { 
    _name="ExampleSPSelection"; 
  }

  bool ExampleSPSelection::initialize() {

    //use SPTAnaBase.h _useMC to decide if to load MC or Reco info
    if (!_useMC)
      RecoProducer("","");

    _mgr.Initialize();
    return SPTAnaBase::initialize();

  }
  
  bool ExampleSPSelection::analyze(storage_manager* storage) {

    auto status = SPTAnaBase::analyze(storage);
    if(!status) return status;

    _mgr.Process(_data);
  
    return true;
  }

  bool ExampleSPSelection::finalize() {

    _mgr.Finalize(_fout);
    return SPTAnaBase::finalize();

  }

}
#endif
