#ifndef RECOTOOL_CLUSTERMATCHER_CXX
#define RECOTOOL_CLUSTERMATCHER_CXX

#include "ClusterMatcher.h"
#include "DataFormat/pfpart.h"

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

    std::vector<std::vector<Hit2D> > local_clusters;
    
    // grab the clusters
    auto ev_cluster = storage->get_data<event_cluster>(_cluster_producer);

    _cru_helper.GenerateHit2D(storage,_cluster_producer,local_clusters);

    _mgr->Reset();

    _mgr->SetClusters(local_clusters);

    _mgr->Process();

    // if we do not wish to store the output -> quit here
    if (!_write_output) return true;

    // create a new pfpart data-product
    auto ev_pfpart = storage->get_data<event_pfpart>(_output_producer);
    // create association object for pfpart -> cluster association
    auto pfpart_ass_v = storage->get_data<event_ass>(ev_pfpart->name());

    // set event ID through storage manager
    storage->set_id(ev_cluster->run(), ev_cluster->subrun(), ev_cluster->event_id());

    // grab results of matching
    auto res_ass = _mgr->GetBookKeeper().GetResult();
    std::cout << "matches : " << res_ass.size() << std::endl;

    // store vector for the association of pfpart -> cluster
    std::vector<std::vector<unsigned int> > pfpart_cluster_ass_v;
    
    // loop through all output matched pairs produced
    for (auto const& pair : res_ass){
      std::cout << "new PFPart!" << std::endl;
      pfpart match(13,0,0,std::vector<size_t>());
      pfpart_cluster_ass_v.push_back(pair);
      ev_pfpart->push_back(match);

    }

    pfpart_ass_v->set_association(ev_pfpart->id(), product_id(data::kCluster,_cluster_producer),
				  pfpart_cluster_ass_v);


    return true;


    return true;
  }

  bool ClusterMatcher::finalize() {

    return true;
  }

}
#endif
