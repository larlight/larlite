#ifndef ERTOOL_ALGOMICHELE_CXX
#define ERTOOL_ALGOMICHELE_CXX

#include "AlgoMichelE.h"

namespace ertool {

  size_t n_michel_e = 0;

  AlgoMichelE::AlgoMichelE() : AlgoBase()
  {
    _name     = "AlgoMichelE";
    michel_energy = 0;
  }

  void AlgoMichelE::ProcessEnd(TFile* fout){
 
    if(fout){
      fout->cd();
      if(michel_energy)
	michel_energy->Write();

    }

  }

  void AlgoMichelE::Finalize()
  {
    std::cout<<"Number of michel E's found is "<<n_michel_e<<std::endl;
  }

  void AlgoMichelE::ProcessBegin()
  {
    _alg_singleE.ProcessBegin();

    InitializeHistos();

    return;
  }

  void AlgoMichelE::InitializeHistos(){
    
    if(!michel_energy)
      michel_energy = new TH1F("michel_energy","michel_energy",100,0,100);
    
  }
  

  void AlgoMichelE::LoadParams(std::string fname, size_t version){
    
    // Load singleE params
    _alg_singleE.LoadParams(fname,version);
    
    return;
  }

  ParticleSet AlgoMichelE::Reconstruct(const EventData &data)
  { 
    
    ParticleSet res;

    //Get a list of single (start point isolated) electron showers
    //from the AlgoSingleE instance
    ParticleSet single_es = _alg_singleE.Reconstruct(data);
    
    //Loop over all isolated electrons in the event
    //Are any of the electrons located at the endpoint (1cm) of a long track?
    for(auto const& electron : single_es){

      for(auto const& track : data.Track()){

	if(electron.Vertex().Dist(track->back()) < 1.){
	  n_michel_e++;
	  
	  if(michel_energy)
	    michel_energy->Fill(electron.Energy());
	  
	}
      }

    }

    return res;

  }

}

#endif
