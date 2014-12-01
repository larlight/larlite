#ifndef LARLITE_EXAMPLESPSELECTION_CXX
#define LARLITE_EXAMPLESPSELECTION_CXX

#include "ExampleSPSelection.h"

namespace larlite {

  bool ExampleSPSelection::initialize() {

    if(_shower_producer.empty()) {
      print(msg::kERROR,__FUNCTION__,
	    "Shower producer name is not set...");
      return false;
    }

    return true;
  }
  
  bool ExampleSPSelection::analyze(storage_manager* storage) {

    auto ev_shower = storage->get_data<event_shower>(_shower_producer);

    _mgr.Process(*ev_shower);
  
    return true;
  }

  bool ExampleSPSelection::finalize() {

    return true;
  }

}
#endif
