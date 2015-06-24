#ifndef ERTOOL_ERANAGRAPHMAKER_CXX
#define ERTOOL_ERANAGRAPHMAKER_CXX

#include "ERAnaGraphMaker.h"

namespace ertool {

  ERAnaGraphMaker::ERAnaGraphMaker() : AnaBase()
			       , _result_tree(nullptr)

  {
    _name     = "ERAnaGraphMaker";

    if (_result_tree) { delete _result_tree; }
    _result_tree = new TTree("_result_tree","Result Tree");

  }

  void ERAnaGraphMaker::Reset()
  {}
  
  bool ERAnaGraphMaker::Analyze(const EventData &data, const ParticleGraph &graph)
  {
    
    if (_debug)
      std::cout << "******  Begin ERAnaGraphMaker Analysis  ******" << std::endl;
    
    /*
      if (_debug){
      std::cout << "ParticleGraph Diagram: " << std::endl
      << graph.Diagram() << std::endl;
      }
    */
    return true;
  }
  
  void ERAnaGraphMaker::ProcessEnd(TFile* fout)
  {
    
    return;
  }


}

#endif
