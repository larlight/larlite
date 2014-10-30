#ifndef RECOTOOL_CLUSTERMATCHER_CXX
#define RECOTOOL_CLUSTERMATCHER_CXX

#include "ClusterMatcher.h"

namespace larlite {

  ClusterMatcher::ClusterMatcher() : _mgr(nullptr)
  { 
    _name="ClusterMatcher"; 
    _fout=0; 
    _cluster_producer="mergedfuzzycluster";

    auto geom = ::larutil::Geometry::GetME();
    _mgr = new ::cmtool::CMatchManager(geom->Nplanes());
    
  }

  bool ClusterMatcher::initialize() {

    if(_fout) _mgr->SetAnaFile(_fout);

    return true;
  }
  
  bool ClusterMatcher::analyze(storage_manager* storage) {

    std::vector<std::vector<larutil::PxHit> > local_clusters;

    _cru_helper.GeneratePxHit(storage,_cluster_producer,local_clusters);

    _mgr->Reset();

    _mgr->SetClusters(local_clusters);

    _mgr->Process();

    return true;
  }

  bool ClusterMatcher::finalize() {

    return true;
  }

}
#endif
