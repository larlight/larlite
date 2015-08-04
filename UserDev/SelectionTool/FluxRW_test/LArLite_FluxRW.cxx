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


    // One can just feed fluxRW the storage container and it will do all the 
    // hard work behind the curtain. This simple cout's the weight that you will 
    // want to apply to your event 
    std::cout << "Event Weight : " << _fluxRW.event_weight(storage) << std::endl; 
  
    // If you want to make the sausage yourself then you will need to pull out 
    // the "mctruth" and "mcflux" data-products
    // I was fast and loose with how I defined them, but you will want to be careful 
    // to extract the information that relates to the event you are studying
    // though if you are using something like a "cosmic overlaid neutrino interaction" 
    // you will want to use the **product of the weights**! 
    auto mcn = storage->get_data<event_mctruth>("generator");
    auto flux = storage->get_data<event_mcflux>("generator")->at(0); 
    
    // You will need to extract the neutrino 
    auto const& mc = mcn->at(0).GetNeutrino();
    
    // and then cast the neutrino PDG code into an "ntype" 
        Int_t ntype = 0;	
	if(mc.Nu().PdgCode() == 12) ntype = 1;	
	else if(mc.Nu().PdgCode() == -12) ntype = 2;	
	else if(mc.Nu().PdgCode() == 14) ntype = 3;	
	else if(mc.Nu().PdgCode() == -14) ntype = 4;
	
	// Now you need to know the parent of the neutrino: muon, charged pion, K_L^0, or charged kaon
	// This is cast as a "ptype", the "mcflux" data product isn't well commented but
	// "fndecay" returns
	//
	// 1  K0L -> nue pi- e+
	// 2  K0L -> nuebar pi+ e-
	// 3  K0L -> numu pi- mu+
	// 4  K0L -> numubar pi+ mu-
	// 5  K+  -> numu mu+
	// 6  K+  -> nue pi0 e+
	// 7  K+  -> numu pi0 mu+
	// 8  K-  -> numubar mu-
	// 9  K-  -> nuebar pi0 e-
	// 10  K-  -> numubar pi0 mu-
	// 11  mu+ -> numubar nue e+
	// 12  mu- -> numu nuebar e-
	// 13  pi+ -> numu mu+
	// 14  pi- -> numubar mu- 
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
	


	// Finally the last thing you need is the neutrinos true energy
	// this then gets feed in with the neutrino flavor and the parent ID to produce an event weight
	// this event weight should then be applied to any frame that contains such a neutrino interaction
	// if you feed a ntpye or ptype which doesn't correspond to a known value this function will return
	// a weight of 1. 
	  std::cout << "Energy : " << mc.Nu().Trajectory().at(0).E() << 
	         " , NType : " << ntype << 
	         " , Ptype : " << ptype << 
	         " thus Weight " << _fluxRW.get_weight(mc.Nu().Trajectory().at(0).E(), ntype, ptype) << std::endl;
	
	  // Congrats! You know are properly weighting your neutrino flux! Woohoo! 
	  //                            ヘ(◕。◕ヘ)
	  //           
	  //
	  //           ♪┏(・o･)┛♪┗ ( ･o･) ┓♪┏ ( ) ┛♪┗ (･o･ ) ┓♪┏(･o･)┛♪
	  //
	  //

    return true;
  }

  bool LArLite_FluxRW::finalize() {

    std::cout << "DONE!" << std::endl; 
    
    return true;
  }

}
#endif
