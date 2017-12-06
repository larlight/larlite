#ifndef CPALGO_CLASS_NAME_CXX
#define CPALGO_CLASS_NAME_CXX

#include "CPAlgo_Class_Name.h"

namespace cmtool {

  //----------------------------------------------------------
  CPAlgo_Class_Name::CPAlgo_Class_Name() : CPriorityAlgoBase()
  //----------------------------------------------------------
  {

  }

  //-----------------------------
  void CPAlgo_Class_Name::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------
  float CPAlgo_Class_Name::Priority(const ::cluster::ClusterParamsAlg &cluster)
  //------------------------------------------------------------------------------
  {
    if(cluster.GetNHits()) return 1.;
    else return -1.;
  }
  
  //------------------------------
  void CPAlgo_Class_Name::Report()
  //------------------------------
  {

  }
    
}
#endif
