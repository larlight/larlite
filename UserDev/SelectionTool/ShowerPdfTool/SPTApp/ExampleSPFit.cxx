#ifndef EXAMPLESPFIT_CXX
#define EXAMPLESPFIT_CXX

#include "ExampleSPFit.h"

namespace larlite {

  ExampleSPFit::ExampleSPFit()
    : SPTAnaBase()
    , _filter (nullptr)
    , _algo   (nullptr)
  { _name="ExampleSPFit"; }

  bool ExampleSPFit::initialize() {
    
    auto status = SPTAnaBase::initialize();

    if(!_algo) {
      print(msg::kERROR,__FUNCTION__,
	    "Algorithm not set. Aborting...");
      return false;
    }

    return status;
  }
  
  bool ExampleSPFit::analyze(storage_manager* storage) {

    auto status = SPTAnaBase::analyze(storage);

    if(!status) return status;

    ::sptool::SPAData input = _data;

    // Filter out some showers
    _data.ApplyOrder(_filter->Select(_data));

    // Make a combination and run
    _algo->Fill(_data);

    return true;
  }

  bool ExampleSPFit::finalize() {

    _algo->Fit(_fout);

    return true;
  }

}
#endif
