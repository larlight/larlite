#ifndef ERTOOL_ERANATREEMAKER_CXX
#define ERTOOL_ERANATREEMAKER_CXX

#include "ERAnaTreeMaker.h"

namespace ertool {

  ERAnaTreeMaker::ERAnaTreeMaker() : AnaBase()
			       , _result_tree(nullptr)

  {
    _name     = "ERAnaTreeMaker";

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");

  }

  void ERAnaTreeMaker::Reset()
  {}
  
  bool ERAnaTreeMaker::Analyze(const EventData &data, const ParticleGraph &graph)
  {
    
    if (_debug)
      std::cout << "******  Begin ERAnaTreeMaker Analysis  ******" << std::endl;
    
    /*
      if (_debug){
      std::cout << "ParticleGraph Diagram: " << std::endl
      << graph.Diagram() << std::endl;
      }
    */
    return true;
  }
  
  void ERAnaTreeMaker::ProcessEnd(TFile* fout)
  {
    
    return;
  }


}

#endif
