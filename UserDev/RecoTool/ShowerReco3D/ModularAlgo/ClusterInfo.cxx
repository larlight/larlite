#ifndef CLUSTERINFO_CXX
#define CLUSTERINFO_CXX

#include "ClusterInfo.h"

namespace showerreco{

  void ClusterInfo::initialize() {
    
    if (_tree) delete _tree;
    _tree = new TTree(_name.c_str(),"Cluster Info output tree");
    _tree->Branch("_")
    
  }
  
  void ClusterInfo::do_reconstruction(const ShowerClusterSet_t &, Shower_t &){

  }


} //showerreco

#endif