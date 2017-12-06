#ifndef RECOTOOL_CLUSTERMERGER_CXX
#define RECOTOOL_CLUSTERMERGER_CXX

#include "ClusterMerger.h"
#include <algorithm>

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

namespace larlite {

ClusterMerger::ClusterMerger()
{
  _name = "ClusterMerger";
  _fout = 0;
  _clus_producer = "";
  _vtx_producer  = "";
  _output_producer = "";
  _write_output = false;

  _wire2cm  = larutil::GeometryHelper::GetME()->WireToCm();
  _time2cm  = larutil::GeometryHelper::GetME()->TimeToCm();
  
}

bool ClusterMerger::initialize() {

  _merge_helper.SetAnaFile(_fout);

  return true;
}

bool ClusterMerger::analyze(storage_manager* storage) {

  std::vector< ::cluster::Cluster > local_clusters;

  _cluster_maker.MakeClusters(storage, _clus_producer, _vtx_producer, local_clusters);

  _merge_helper.Process(local_clusters);

  auto const& bk = _merge_helper.GetResult();

  if (!_write_output) return true;

  // To write an output, get input cluster data
  auto ev_cluster = storage->get_data<event_cluster>(_clus_producer);

  // Initialize the output cluster data product
  if (_output_producer.empty()) _output_producer = Form("merged%s", _clus_producer.c_str());
  auto out_cluster_v = storage->get_data<event_cluster>(_output_producer);
  auto out_event_ass = storage->get_data<event_ass>(out_cluster_v->name());
  if (!out_cluster_v) {
    print(msg::kERROR, __FUNCTION__,
          Form("Failed initializing the output cluster data product with producer name %s!", _output_producer.c_str()));
    return false;
  }
  // without the above (!out_cluster_v) check, code crashes here
  out_cluster_v->clear();

  // set event ID through storage manager
  storage->set_id(ev_cluster->run(), ev_cluster->subrun(), ev_cluster->event_id());

  // Proceed to write an output data product if any cluster data product exists in the input
  if (!ev_cluster->size()) {
    print(msg::kWARNING, __FUNCTION__,
          Form("No input clusters! Saving empty event to output ttree..."));
    return true;
  }

  std::vector<std::vector<unsigned short> > merged_indexes;
  bk.PassResult(merged_indexes);

  event_hit* ev_hits = nullptr;
  auto const& original_hit_ass = storage->find_one_ass(ev_cluster->id(), ev_hits, ev_cluster->name());

  // prepare clusters from merged indices in order to fill additional cluster attributes
  std::vector< ::cluster::Cluster > output_clusters;
  std::vector< std::vector<unsigned int> > output_cluster_hit_indices_v;
  for (auto const& clus_idx_v : merged_indexes) {
    std::vector<unsigned int> clus_hit_idx_v;
    for (auto const& clus_idx : clus_idx_v) {
      auto const& hit_idx_v = original_hit_ass[clus_idx];
      for (auto const& hit_idx : hit_idx_v) {
	clus_hit_idx_v.push_back( hit_idx );
      }
    }
    output_cluster_hit_indices_v.push_back(clus_hit_idx_v);
  }
  _cluster_maker.MakeClusters(storage, output_cluster_hit_indices_v, _clus_producer, _vtx_producer, output_clusters);
  
  // Reserve output vector size for faster push_back
  out_cluster_v->reserve(merged_indexes.size());

  //tmp_index to know what plane to use
  unsigned int tmp_index = 0;
  AssSet_t hit_ass;

  for (size_t mi=0; mi < merged_indexes.size(); mi++) {

    auto const& indexes = merged_indexes[mi];

    AssUnit_t merged_association;

    for (auto const& cluster_index : indexes) {

      tmp_index = cluster_index;

      merged_association.reserve(merged_association.size() + original_hit_ass[cluster_index].size());

      for (auto const& hit_index : original_hit_ass[cluster_index])

        merged_association.push_back(hit_index);

    }

    cluster out_cluster;
    out_cluster.set_id(out_cluster_v->size());
    auto const& view = ev_cluster->at(tmp_index).View();
    out_cluster.set_view( view );


    // Deal with keeping track of the mergin information
    out_cluster.set_is_merged(true);

    // fill cluster information based on ::cluster::Cluster parameters
    FillClusterProperties(out_cluster,output_clusters[mi]);

    // Check to see if the input was merged or not:
    if (ev_cluster -> front().IsMergedCluster()) {

      // In this case, merge the original indexes and copy the producer
      std::string original_producer = ev_cluster -> front().OriginalClusterProducer();

      out_cluster.set_original_producer(original_producer);

      // Combine all the indexes of the original clusters:
      std::vector<unsigned short> original_indexes;
      for (auto const& cluster_index : indexes) {
        original_indexes.insert(original_indexes.end(),
                                ev_cluster -> at(cluster_index).OriginalClusterIndexes().begin(),
                                ev_cluster -> at(cluster_index).OriginalClusterIndexes().end());
      }

      // clean up the indexes:
      std::sort(original_indexes.begin(), original_indexes.end());
      auto last = std::unique(original_indexes.begin(), original_indexes.end());
      original_indexes.erase(last, original_indexes.end());

      out_cluster.set_original_indexes(original_indexes);
    }
    else {

      // Here, set the original producer and the original indexes:
      out_cluster.set_original_producer(_clus_producer);
      out_cluster.set_original_indexes(indexes);
    }

    out_cluster_v->push_back(out_cluster);
    hit_ass.push_back(merged_association);
  }

  //auto out_event_ass = storage->get_data<event_ass>(out_cluster_v->name());
  out_event_ass->set_association(out_cluster_v->id(), ev_hits->id(), hit_ass);
  //out_cluster_v->set_association(data::kHit,hit_producer,hit_ass);
  return true;
}

  void ClusterMerger::FillClusterProperties(::larlite::cluster& cluster,
					    const ::cluster::Cluster& CMCluster) {

    float startW = CMCluster._start_pt._w;
    float startT = CMCluster._start_pt._t;

    float endW   = CMCluster._end_pt._w;
    float endT   = CMCluster._end_pt._t;

    cluster.set_planeID(::larlite::geo::PlaneID(0,0,CMCluster._plane));
    
    cluster.set_start_wire( int(startW / _wire2cm), 0.5);
    cluster.set_start_tick( int(startT / _time2cm), 0.5);

    cluster.set_end_wire( int(endW / _wire2cm), 0.5);
    cluster.set_end_tick( int(endT / _time2cm), 0.5);

    cluster.set_start_angle( CMCluster._angle );
    cluster.set_summedADC( CMCluster._sum_charge, 0., 0.);
    cluster.set_n_hits( CMCluster.size() );
  }

bool ClusterMerger::finalize() {

  return true;
}

}
#endif
