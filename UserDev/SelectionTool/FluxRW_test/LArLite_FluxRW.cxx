#ifndef LARLITE_LARLITE_FLUXRW_CXX
#define LARLITE_LARLITE_FLUXRW_CXX

#include "LArLite_FluxRW.h"

namespace larlite {

  bool LArLite_FluxRW::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    std::cout << "Let's go! " << std::endl;

    return true;
  }
  
  bool LArLite_FluxRW::analyze(storage_manager* storage) {



    std::cout << "Event Weight : " << fluxRW.event_weight(storage) << std::endl; 
  
    auto mcn = storage->get_data<event_mctruth>("generator");
    auto flux = storage->get_data<event_mcflux>("generator")->at(0); 
    
    
    auto const& mc = mcn->at(0).GetNeutrino();
     
        Int_t ntype = 0;	
	if(mc.Nu().PdgCode() == 12) ntype = 1;	
	else if(mc.Nu().PdgCode() == -12) ntype = 2;	
	else if(mc.Nu().PdgCode() == 14) ntype = 3;	
	else if(mc.Nu().PdgCode() == -14) ntype = 4;
	
	  //Determine parent type (muon, charged pion, K_L^0 or charged kaon)
	Int_t ptype = 0;
	
	Int_t decay_type = flux.fndecay;
	switch(decay_type) {
	  
	case 1:
	case 2:
	case 3:
	case 4:
	  ptype = 3;
	  break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	  ptype = 4;
	  break;
	case 11:
	case 12:
	  ptype = 1;
	  break;
	case 13:
	case 14:
	  ptype = 2;
	  
	}
	


	/*
	  std::cout << "Energy : " << mc.Nu().Trajectory().at(0).E() << 
	         " , NType : " << ntype << 
	         " , Ptype : " << ptype << 
	         " thus Weight " << fluxRW.get_weight(mc.Nu().Trajectory().at(0).E(), ntype, ptype) << std::endl;
	*/

	std::cout << " TEST : " << fluxRW.get_weight(mc.Nu().Trajectory().at(0).E(), 0, 0) << std::endl;
  

    return true;
  }

  bool LArLite_FluxRW::finalize() {

    std::cout << "DONE!" << std::endl; 
    
    return true;
  }

}
#endif
