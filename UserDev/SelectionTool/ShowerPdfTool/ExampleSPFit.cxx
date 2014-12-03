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
    if(_filter) {
      std::vector<size_t> valid_index_v;
      valid_index_v.reserve(_data._showers.size());
      for(size_t i=0; i<_data._showers.size(); ++i) {
	input._showers.resize(1);
	input._showers[0] = _data._showers[i];
	if(_filter && _filter->Select(input))  valid_index_v.push_back(i);
	else if(!_filter) valid_index_v.push_back(i);
	
      }
      
      if(valid_index_v.size() < _algo->_nshowers) return true;

      input._showers.clear();
      input._showers.reserve(valid_index_v.size());
      for(auto const& index : valid_index_v)
	input._showers.push_back(_data._showers[index]);

    }

    // Make a combination and run
    _algo->Fill(input);

    return true;
  }

  bool ExampleSPFit::finalize() {

    _algo->Fit(_fout);

    return true;
  }

}
#endif
