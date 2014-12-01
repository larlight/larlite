#ifndef EXAMPLESPFIT_CXX
#define EXAMPLESPFIT_CXX

#include "ExampleSPFit.h"

namespace larlite {

  bool ExampleSPFit::initialize() {

    if(!_algo) {
      print(msg::kERROR,__FUNCTION__,
	    "Algorithm not set. Aborting...");
      return false;
    }

    if(_shower_producer.empty()) {
      print(msg::kERROR,__FUNCTION__,
	    "Shower producer name is not set...");
      return false;
    }
    
    return true;
  }
  
  bool ExampleSPFit::analyze(storage_manager* storage) {

    if(_mc) {

      auto ev_mcs = storage->get_data<event_mcshower>(_shower_producer);
      std::vector<const mcshower*> data;
      data.reserve(ev_mcs->size());
      for(size_t i=0; i<ev_mcs->size(); ++i)

	data.push_back(&((*ev_mcs)[i]));

      if(data.size() == _algo->_nshowers)
	_algo->Fill(data);

    }else{

      auto ev_shower = storage->get_data<event_shower>(_shower_producer);

      std::vector<size_t> valid_index_v;
      valid_index_v.reserve(ev_shower->size());
      for(size_t i=0; i<ev_shower->size(); ++i) {
	
	if(_filter && _filter->Select((*ev_shower)[i]))  valid_index_v.push_back(i);
	else if(!_filter) valid_index_v.push_back(i);
	
      }

      std::vector<const shower*> data;
      data.reserve(valid_index_v.size());
      for(auto const& index : valid_index_v) 
	data.push_back(&((*ev_shower)[index]));

      if(data.size() == _algo->_nshowers)

	_algo->Fill(data);

    }      
  
    return true;
  }

  bool ExampleSPFit::finalize() {

    _algo->Fit();

    _algo->StoreParams();

    return true;
  }

}
#endif
