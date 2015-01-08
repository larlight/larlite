#ifndef SELECTIONTOOL_SPALGOMICHELE_CXX
#define SELECTIONTOOL_SPALGOMICHELE_CXX

#include "SPAlgoMichelE.h"

namespace sptool {

  size_t n_michel_e = 0;

  SPAlgoMichelE::SPAlgoMichelE() : SPAlgoBase()
  {
    _name     = "SPAlgoMichelE";
    michel_energy = 0;
    dist_shower_to_trackend = 0;
  }

  void SPAlgoMichelE::ProcessEnd(TFile* fout){
 
    if(fout){
      fout->cd();
      if(michel_energy)
	michel_energy->Write();

      if(dist_shower_to_trackend)
	dist_shower_to_trackend->Write();
    }

  }

  void SPAlgoMichelE::Finalize()
  {
    std::cout<<"Number of michel E's found is "<<n_michel_e<<std::endl;
  }

  void SPAlgoMichelE::ProcessBegin()
  {
    _alg_emp.ProcessBegin();

    InitializeHistos();

    return;
  }

  void SPAlgoMichelE::InitializeHistos(){
    
    if(!michel_energy)
      michel_energy = new TH1F("michel_energy","michel_energy",100,0,100);
    
    if(!dist_shower_to_trackend)
      dist_shower_to_trackend = new TH1F("dist_shower_to_trackend","dist_shower_to_trackend",500,0,100);

  }
  

  void SPAlgoMichelE::LoadParams(std::string fname, size_t version){
    
    // Load EMPart params
    _alg_emp.LoadParams(fname,version);
    
    return;
  }

  SPArticleSet SPAlgoMichelE::Reconstruct(const SPAData &data)
  { 
    
    SPArticleSet res;
    
    std::vector<sptool::SPAShower> e_showers;
    
    //Loop over all showers in the event, store ones that are electron-like
    for(auto const& shower : data._showers) 
      if(IsShowerElectron(shower)) e_showers.push_back(shower);
    
    //Are any of the showers located at the endpoint of a long track?
    for(auto const& e_shower : e_showers){

      for(auto const& track : data._tracks){

	if(dist_shower_to_trackend)
	  dist_shower_to_trackend->Fill(e_shower.Start().Dist(*(track.end()-1)));
	if(e_shower.Start().Dist(*(track.end()-1)) < 1){
	  n_michel_e++;
	  //	  std::cout<<"michel found. track length is "<<track.Length()<<std::endl;
	  if(michel_energy)
	    michel_energy->Fill(e_shower._energy);
	}
      }

    }

    return res;

  }

  bool SPAlgoMichelE::IsShowerElectron(const sptool::SPAShower &shower){
    
    //Make sure the shower is likely an electron
    //by using LL function from an SPAlgoEMPart instance
    //(LL uses only dEdX data if "dist" is plugged in negative)
    //If likelihood its gamma is more than electron, skip
    
    bool e_like = (
      _alg_emp.LL(true, shower._dedx, -1.) >
      _alg_emp.LL(false,shower._dedx, -1.)) ?
      true : false;
    
    return e_like;
    
  }

}

#endif
