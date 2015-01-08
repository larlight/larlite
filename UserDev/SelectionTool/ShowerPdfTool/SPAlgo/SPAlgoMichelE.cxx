#ifndef SELECTIONTOOL_SPALGOMICHELE_CXX
#define SELECTIONTOOL_SPALGOMICHELE_CXX

#include "SPAlgoMichelE.h"

namespace sptool {

  size_t n_michel_e = 0;

  SPAlgoMichelE::SPAlgoMichelE() : SPAlgoBase()
  {
    _name     = "SPAlgoMichelE";
    michel_energy = 0;
  }

  void SPAlgoMichelE::ProcessEnd(TFile* fout){
 
    if(fout){
      fout->cd();
      if(michel_energy)
	michel_energy->Write();

    }

  }

  void SPAlgoMichelE::Finalize()
  {
    std::cout<<"Number of michel E's found is "<<n_michel_e<<std::endl;
  }

  void SPAlgoMichelE::ProcessBegin()
  {
    _alg_singleE.ProcessBegin();

    InitializeHistos();

    return;
  }

  void SPAlgoMichelE::InitializeHistos(){
    
    if(!michel_energy)
      michel_energy = new TH1F("michel_energy","michel_energy",100,0,100);
    
  }
  

  void SPAlgoMichelE::LoadParams(std::string fname, size_t version){
    
    // Load singleE params
    _alg_singleE.LoadParams(fname,version);
    
    return;
  }

  SPArticleSet SPAlgoMichelE::Reconstruct(const SPAData &data)
  { 
    
    SPArticleSet res;

    //Get a list of single (start point isolated) electron showers
    //from the SPAlgoSingleE instance
    SPArticleSet single_es = _alg_singleE.Reconstruct(data);
    
    //Loop over all isolated electrons in the event
    //Are any of the electrons located at the endpoint (1cm) of a long track?
    for(auto const& electron : single_es){

      for(auto const& track : data._tracks){

	if(electron.pos().Dist(*(track.end()-1)) < 1.){
	  n_michel_e++;
	  
	  if(michel_energy)
	    michel_energy->Fill(electron.energy());
	  
	}
      }

    }

    return res;

  }

}

#endif
