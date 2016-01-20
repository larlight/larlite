#ifndef LARLITE_DEDX_WALKER_CXX
#define LARLITE_DEDX_WALKER_CXX

#include "dEdx_Walker.h"

namespace larlite {

  bool dEdx_Walker::initialize() {

    dedx = new TH1D(Form("hist_%d", w), "", 500, 0, 500);

    g = new TGraph();

    fTPC.Min(-10., -126., -10. );
    fTPC.Max( 292., 136., 1150. );

    return true;

  }
  
  bool dEdx_Walker::analyze(storage_manager* storage) {

    auto tracks_v = storage->get_data<event_mctrack>("mcreco");

    std::cout << "N tracks : " << tracks_v->size() << std::endl; 

    for(auto track :  *tracks_v){

      if(track.size() == 0) { 
	for(int i = 0; i < track.dEdx().size(); i++){
	  std::cout << "balls" << std::endl; 
	}
	  continue;} 

      geoalgo::Point_t start(track.Start().Position());
      geoalgo::Point_t end(track.End().Position());

      if(!fTPC.Contain(start) || !fTPC.Contain(end)) { continue;}

      w++;
    
      dedx->SetName(Form("hist_%d", w));
      g->SetName(Form("proton_graph_%d", w));

      dQdx = track.dQdx();
      dEdx = track.dEdx();
      
      std::cout << "N dQdx's : "  << dQdx.size() << std::endl;
      std::cout << "N dQdx's Size : "  << dQdx.at(0).size() << std::endl;
          
      double length = 0;

      for(int i = 0; i < dEdx.size(); i++){
	
	length += sqrt(pow(track.at(i).Position().X()-track.at(i+1).Position().X(),2) + 
		       pow(track.at(i).Position().Y()-track.at(i+1).Position().Y(),2) + 
		       pow(track.at(i).Position().Z()-track.at(i+1).Position().Z(),2)) ;


      }

      //      std::cout << "Length " << length << std::endl;
 
      int i = 0;
      
      double len = 0;

      int l = 0;
      double MIP = 0;

      for(auto e : dEdx){
		
	dedx->SetBinContent(i, e);

	len += sqrt(pow(track.at(i).Position().X()-track.at(i+1).Position().X(),2) +
		    pow(track.at(i).Position().Y()-track.at(i+1).Position().Y(),2) +
		    pow(track.at(i).Position().Z()-track.at(i+1).Position().Z(),2)) ;
	
	//	std::cout << "Res Range  : " << length - len << " dedx : " << e << std::endl; 

	if(length - len > 20 && e > 0){
	  l++;
	  MIP += e;
	}

	i++;

	if(e == 0) continue;

	if(length - len > 100){
	  
	  g->SetPoint(i, (length - len), e);

	}
	

	
      }
      
      if(l != 0){
	std::cout << " MIP " << MIP/l << std::endl; 
	  AVG += MIP/l;
	  N++;
      }
      std::cout << "DONE!" << std::endl;

      dedx->Write();

      g->Write();

      dedx->Reset();
      
    }
    

    return true;
  }

  bool dEdx_Walker::finalize() {

    if(N != 0){
      std::cout << " MIPS be " << AVG/N << std::endl; 
    }
    else{ std::cout << " MIPS be 0 dummy" << std::endl; }
      
    dedx->Write();


    return true;
  }

}
#endif
