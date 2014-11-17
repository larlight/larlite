#ifndef RECOTOOL_CLUSTERMERGER_CXX
#define RECOTOOL_CLUSTERMERGER_CXX

#include "ClusterMerger.h"

namespace larlite {

  bool ClusterMerger::initialize() {

    if(_fout) _mgr.SetAnaFile(_fout);

    return true;
  }
  
  bool ClusterMerger::analyze(storage_manager* storage) {

    std::vector<std::vector<larutil::PxHit> > local_clusters;

    _cru_helper.GeneratePxHit(storage,_cluster_producer,local_clusters);

    _mgr.Reset();

    _mgr.SetClusters(local_clusters);

    _mgr.Process();

    if(!_write_output) return true;

    // To write an output, get input cluster data
    auto ev_cluster = storage->get_data<event_cluster>(_cluster_producer);

    // Initialize the output cluster data product
    auto out_cluster_v = storage->get_data<event_cluster>(Form("merged%s",_cluster_producer.c_str()));
    out_cluster_v->clear();
    out_cluster_v->set_event_id(ev_cluster->event_id());
    out_cluster_v->set_run(ev_cluster->run());
    out_cluster_v->set_subrun(ev_cluster->subrun());

    // Proceed to write an output data product if any cluster data product exists in the input
    if(!ev_cluster->size()) {
      print(msg::kWARNING,__FUNCTION__,
	    Form("No input clusters! Saving empty event to output ttree..."));
      return true;
    }

    // Get hit producer name
    auto hit_producer_v = ev_cluster->association_keys(data::kHit);

    if(!hit_producer_v.size()) {

      print(msg::kERROR,__FUNCTION__,
	    Form("Non empty cluster has no association to hits!"));
      return false;
    }
    auto hit_producer = hit_producer_v[0];

    std::vector<std::vector<unsigned short> > merged_indexes;
    _mgr.GetBookKeeper().PassResult(merged_indexes);

    // Reserve output vector size for faster push_back
    out_cluster_v->reserve(merged_indexes.size());
    
    //tmp_index to know what plane to use
    unsigned int tmp_index = 0;
    AssSet_t hit_ass;
    auto original_hit_ass = ev_cluster->association(data::kHit,hit_producer);

    for(auto const& indexes : merged_indexes) {

      AssUnit_t merged_association;

      for(auto const& cluster_index : indexes) {

	tmp_index = cluster_index;

	merged_association.reserve(merged_association.size() + original_hit_ass[cluster_index].size());

	for(auto const& hit_index : original_hit_ass[cluster_index])

	  merged_association.push_back(hit_index);
	
      }

      cluster out_cluster;
      out_cluster.set_id(out_cluster_v->size());
      out_cluster.set_view(ev_cluster->at(tmp_index).View());
      out_cluster_v->push_back(out_cluster);
      hit_ass.push_back(merged_association);
    }

    out_cluster_v->set_association(data::kHit,hit_producer,hit_ass);
    return true;
  }

  bool ClusterMerger::finalize() {

    return true;
  }

}
#endif
