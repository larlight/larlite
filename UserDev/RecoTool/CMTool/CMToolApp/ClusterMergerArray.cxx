#ifndef RECOTOOL_CLUSTERMERGERARRAY_CXX
#define RECOTOOL_CLUSTERMERGERARRAY_CXX

#include "ClusterMergerArray.h"

namespace larlite {

  ClusterMergerArray::ClusterMergerArray()
  {
    _name="ClusterMergerArray"; 
    _fout=0; 
    _input_producer="fuzzycluster"; 
    _output_producer="";
    _write_output=false;
  }

  ::cmtool::CMergeManager& ClusterMergerArray::GetManager(size_t mgr_id)
  {
    if(_mgr_v.size() <= mgr_id) _mgr_v.resize(mgr_id+1);
    return _mgr_v[mgr_id];
  }

  bool ClusterMergerArray::initialize() {

    if(!_mgr_v.size()) {
      print(msg::kERROR,__FUNCTION__,
	    "No manager is set ro run!");
      return false;
    }

    if(_fout) 
      for(auto& mgr : _mgr_v) mgr.SetAnaFile(_fout);

    return true;
  }
  
  bool ClusterMergerArray::analyze(storage_manager* storage) {

    std::vector<std::vector<larutil::PxHit> > local_clusters;

    _cru_helper.GeneratePxHit(storage,_input_producer,local_clusters);

    ::cmtool::CMergeBookKeeper bk;

    for(size_t i=0; i<_mgr_v.size(); ++i) {

      auto& mgr = _mgr_v[i];

      mgr.Reset();
      
      if(!i) mgr.SetClusters(local_clusters);
      else mgr.SetClusters( _mgr_v[i-1].GetClusters() );

      mgr.Process();

      auto const& new_bk = mgr.GetBookKeeper();

      if(!i) bk = new_bk;
      else if(new_bk.GetResult().size() < new_bk.size())
	bk.Combine(new_bk);

      //bk.Report();
    }

    if(!_write_output) return true;

    // To write an output, get input cluster data
    auto ev_cluster = storage->get_data<event_cluster>(_input_producer);

    // Initialize the output cluster data product
    if(_output_producer.empty()) _output_producer = Form("merged%s",_input_producer.c_str());
    auto out_cluster_v = storage->get_data<event_cluster>(_output_producer);
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
    bk.PassResult(merged_indexes);

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

  bool ClusterMergerArray::finalize() {

    return true;
  }

}
#endif
