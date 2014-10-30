#ifndef CFALGO_CLASS_NAME_CXX
#define CFALGO_CLASS_NAME_CXX

#include "CFAlgo_Class_Name.hh"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgo_Class_Name::CFAlgo_Class_Name() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CFAlgo_Class_Name::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgo_Class_Name::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    if(clusters.size()) return 1.;
    else return -1.;
  }

  //------------------------------
  void CFAlgo_Class_Name::Report()
  //------------------------------
  {

  }
    
}
#endif
