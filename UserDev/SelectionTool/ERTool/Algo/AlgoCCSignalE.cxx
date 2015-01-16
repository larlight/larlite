#ifndef ERTOOL_ALGOCCSIGNALE_CXX
#define ERTOOL_ALGOCCSIGNALE_CXX

#include "AlgoCCSignalE.h"

namespace ertool {

  AlgoCCSignalE::AlgoCCSignalE() : AlgoBase()
  {
    _name     = "AlgoCCSignalE";
  }

  void AlgoCCSignalE::ProcessEnd(TFile* fout)
  {
  }

  
  void AlgoCCSignalE::ProcessBegin()
  {
    _alg_singleE.ProcessBegin();

    return;
  }

  void AlgoCCSignalE::LoadParams(std::string fname, size_t version){
    
    // Load singleE params
    _alg_singleE.LoadParams(fname,version);
    
    return;
  }

  void AlgoCCSignalE::Reset()
  {}

  ParticleSet AlgoCCSignalE::Reconstruct(const EventData &data)
  { 
    
    ParticleSet res;

    //Get a list of single (start point isolated) electron showers
    //from the AlgoSingleE instance
    ParticleSet single_es = _alg_singleE.Reconstruct(data);

    //Loop over all isolated electrons in the event
    //Do a series of checks to pick out electrons that are signal
    for(auto const& electron : single_es){

    }
    return res;

  }

}

#endif
