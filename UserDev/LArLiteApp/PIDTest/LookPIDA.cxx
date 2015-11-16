#ifndef LARLITE_LOOKPIDA_CXX
#define LARLITE_LOOKPIDA_CXX

#include "LookPIDA.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/partid.h"
namespace larlite {

  bool LookPIDA::initialize() {

    if(hPIDA) delete hPIDA;
    hPIDA = new TH1D("hPIDA",
		     "PIDA; hPIDA; Counts",
		     200,0.,20.);
    return true;
  }
  
  bool LookPIDA::analyze(storage_manager* storage) {

    auto const ev_calo = storage->get_data<event_calorimetry>("trackkalmanhitcalo");

    auto const ev_pid = storage->get_data<event_partid>("trackkalmanhitpid");

    if(!ev_pid) {
      std::cerr<<"no event_partid!"<<std::endl;
      return false;
    }

    if(ev_pid->empty()) {
      std::cout<<"event_partid empty..."<<std::endl;
      return false;
    }
    
    for(auto const& pid : *ev_pid) 
      {
	hPIDA->Fill(pid.PIDA());
	//std::cout<<"PIDA: "<<pid.PIDA()<<std::endl;
      }
    return true;
  }

  bool LookPIDA::finalize() {
    
    if(_fout) {
      if(hPIDA->GetEntries()) hPIDA->Write();
    }
    return true;
  }

}
#endif
