#ifndef SELECTIONTOOL_SPALGOCCSIGNALE_CXX
#define SELECTIONTOOL_SPALGOCCSIGNALE_CXX

#include "SPAlgoCCSignalE.h"

namespace sptool {

  SPAlgoCCSignalE::SPAlgoCCSignalE() : SPAlgoBase()
  {
    _name     = "SPAlgoCCSignalE";
  }

  void SPAlgoCCSignalE::ProcessEnd(TFile* fout)
  {
  }

  
  void SPAlgoCCSignalE::ProcessBegin()
  {
    _alg_singleE.ProcessBegin();

    return;
  }

  void SPAlgoCCSignalE::LoadParams(std::string fname, size_t version){
    
    // Load singleE params
    _alg_singleE.LoadParams(fname,version);
    
    return;
  }

  void SPAlgoCCSignalE::Reset()
  {}

  SPArticleSet SPAlgoCCSignalE::Reconstruct(const SPAData &data)
  { 
    
    SPArticleSet res;

    //Get a list of single (start point isolated) electron showers
    //from the SPAlgoSingleE instance
    SPArticleSet single_es = _alg_singleE.Reconstruct(data);

    //Loop over all isolated electrons in the event
    //Do a series of checks to pick out electrons that are signal
    for(auto const& electron : single_es){



    }


    return res;

  }

}

#endif
