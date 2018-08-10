#ifndef MICHELELECTRONFILTER_CXX
#define MICHELELECTRONFILTER_CXX

#include "MichelElectronFilter.h"
#include "DataFormat/mcshower.h"


namespace larlite {
  
  bool MichelElectronFilter::initialize() {
    
    total_evts = 0;
    kept_evts = 0;

    return true;
  }
  
  bool MichelElectronFilter::analyze(storage_manager* storage) {

    total_evts++;

    //Grab the MCShowers
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");    
    if(!ev_mcshower) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mcshower!"));
      return false;
    }  

    //If no MCShowers in event, no michels for sure
    if(!ev_mcshower->size()){
    std::cout<<"michel filter returning false!"<<std::endl;
      return false;
    }
    //Loop over MCShowers, ask if they came from a muon decay
    for(auto const& mcs : *ev_mcshower){

      //mcs.Process seems to always either be muMinusCaptureAtRest or muIoni
      //seems like captureatrest is the michel.
      //captureatrest has endpoint energy at 55 (expected)
      //ioni has endpoint energy at like 35 (too low)      
      //Also make sure the michel has most of its energy contained in detector     
      
      if(mcs.MotherPdgCode() == 13 &&
	 mcs.Process() == "muMinusCaptureAtRest" &&
	 mcs.DetProfile().E()/mcs.Start().E() > 0.95){
	kept_evts++;
	std::cout<<"michel filter returning true!"<<std::endl;
	return true;
      }

    }
    
    std::cout<<"michel filter returning false!"<<std::endl;
    return false;
  }

  bool MichelElectronFilter::finalize() {

    print(larlite::msg::kNORMAL,__FUNCTION__,Form("Total events considered = %zu, kept events = %zu.",total_evts,kept_evts));
  
    return true;
  }

}
#endif
