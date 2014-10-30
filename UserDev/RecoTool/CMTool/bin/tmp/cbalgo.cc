#ifndef CBALGO_CLASS_NAME_CXX
#define CBALGO_CLASS_NAME_CXX

#include "CBAlgo_Class_Name.hh"

namespace cmtool {

  //-------------------------------------------------------
  CBAlgo_Class_Name::CBAlgo_Class_Name() : CBoolAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-----------------------------
  void CBAlgo_Class_Name::Reset()
  //-----------------------------
  {

  }

  //--------------------------------------------------------------------------
  bool CBAlgo_Class_Name::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			       const ::cluster::ClusterParamsAlg &cluster2)
  //--------------------------------------------------------------------------
  {
    if(cluster1.GetNHits() && cluster2.GetNHits()) return false;
    else return false;
  }

  //------------------------------
  void CBAlgo_Class_Name::Report()
  //------------------------------
  {

  }
    
}
#endif
