#ifndef LARLITE_DLRAWCLUSTERASS_CXX
#define LARLITE_DLRAWCLUSTERASS_CXX

#include "DLRawClusterAss.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"
#include "DataFormat/event_ass.h"

#include <exception>
#include <cassert>

namespace larlite {

  bool DLRawClusterAss::analyze(storage_manager* storage) {
    if(_debug) std::cout << "START" << std::endl;

    if (_threshold<0) {
      std::cout << "negative threshold?" << std::endl;
      throw std::exception();
    }

    auto ev_dl_cluster  = (event_cluster*) storage->get_data(data::kCluster,"dl");    
    auto ev_raw_cluster = (event_cluster*) storage->get_data(data::kCluster,"raw");
    auto ev_out_cluster = (event_cluster*) storage->get_data(data::kCluster,"raw_filtered");
    auto ev_ass         = (event_ass*)     storage->get_data(data::kAssociation,"raw_filtered");

    assert(ev_out_cluster->empty());
    
    event_hit* ev_hit_dl  = nullptr;
    event_hit* ev_hit_raw = nullptr;
    
    auto const& ass_dl_cluster_vv  = storage->find_one_ass(ev_dl_cluster->id(),  ev_hit_dl,  ev_dl_cluster->name());
    auto const& ass_raw_cluster_vv = storage->find_one_ass(ev_raw_cluster->id(), ev_hit_raw, ev_raw_cluster->name());

    if(!ev_hit_dl)  return true;
    if(!ev_hit_raw) return true;

    //
    // identify overlapping DL & raw clusters
    //
    std::vector<std::vector<int> > dl_to_raw_ctr_vv;
    dl_to_raw_ctr_vv.resize(ass_dl_cluster_vv.size());
    for(auto& v : dl_to_raw_ctr_vv) v.resize(ass_raw_cluster_vv.size());

    //dl cluster
    for(size_t dl_cid = 0; dl_cid < ass_dl_cluster_vv.size(); ++dl_cid) {
      auto ass_dl_cluster_v = ass_dl_cluster_vv[dl_cid];
      auto& dl_to_raw_ctr_v = dl_to_raw_ctr_vv[dl_cid];

      if (ass_dl_cluster_v.empty()) continue;

      auto dl_plane = ev_hit_dl->at(ass_dl_cluster_v.front()).View();

      //raw cluster
      for(size_t raw_cid = 0; raw_cid < ass_raw_cluster_vv.size(); ++raw_cid) {
	auto ass_raw_cluster_v = ass_raw_cluster_vv[raw_cid];
	auto& dl_to_raw_ctr = dl_to_raw_ctr_v[raw_cid];

	if (ass_raw_cluster_v.empty()) continue;
	
	auto raw_plane = ev_hit_raw->at(ass_raw_cluster_v.front()).View();
	
	if (dl_plane != raw_plane) continue;

	//dl hit
	for(size_t dl_hid=0; dl_hid < ass_dl_cluster_v.size(); ++dl_hid) {
	  auto dl_hassid = ass_dl_cluster_v[dl_hid];
	  const auto& dl_hit = ev_hit_dl->at(dl_hassid);
	  
	  //raw hit
	  for(size_t raw_hid=0; raw_hid < ass_raw_cluster_v.size(); ++raw_hid) {
	    auto raw_hassid = ass_raw_cluster_v[raw_hid];
	    const auto& raw_hit = ev_hit_raw->at(raw_hassid);
	    
	    if (Equals(dl_hit,raw_hit)) dl_to_raw_ctr++;

	  } //raw hit
	} //dl hit
      } //raw cluster
    } //dl cluster


    //
    // write DL cluster to raw cluster association
    //
    AssSet_t ass_cc_dl_to_raw_vv;

    ass_cc_dl_to_raw_vv.resize(dl_to_raw_ctr_vv.size());

    ev_out_cluster->reserve(ev_dl_cluster->size() + ev_raw_cluster->size());

    for(size_t dl_cid=0; dl_cid < dl_to_raw_ctr_vv.size(); ++dl_cid) {
      auto& ass_cc_dl_to_raw_v = ass_cc_dl_to_raw_vv[dl_cid];
      auto& dl_to_raw_ctr_v = dl_to_raw_ctr_vv[dl_cid];

      ass_cc_dl_to_raw_v.clear();

      for(size_t raw_cid=0; raw_cid < dl_to_raw_ctr_v.size(); ++raw_cid) {
	auto dl_raw_ctr = dl_to_raw_ctr_v[raw_cid];
	if (dl_raw_ctr>_threshold)  {
	  if (_debug) 
	    std::cout << "ASS (dl,raw)=(" << dl_cid << "," << raw_cid << ") !" << std::endl;
	  ass_cc_dl_to_raw_v.push_back(raw_cid);
	}
      }
    }

    if(_debug) std::cout << std::endl;

    //
    // make filtered
    //
    AssSet_t ass_ch_rawfilter_to_rawfilter_vv;
    std::vector<bool> raw_ignore_v(ass_raw_cluster_vv.size(),false);

    //
    // ignore overlaping clusters
    //
    for(size_t dl_cid=0; dl_cid < ass_dl_cluster_vv.size(); ++ dl_cid) {
      const auto& ass_cc_dl_to_raw_v = ass_cc_dl_to_raw_vv[dl_cid];

      for(auto& raw_cid : ass_cc_dl_to_raw_v) {
    	if (_debug) std::cout << "ignore this @raw_cid=" << raw_cid << std::endl;
	raw_ignore_v[raw_cid] = true;
      }

    }
    
    //
    // copy to raw cluster and hits to output (with ignore)
    //
    for(size_t raw_cid=0; raw_cid < ass_raw_cluster_vv.size(); ++raw_cid) {
      const auto& ass_raw_cluster_v = ass_raw_cluster_vv[raw_cid];

      if (raw_ignore_v[raw_cid]) {
    	if (_debug) std::cout << "ignore that @raw_cid=" << raw_cid << std::endl;
    	continue;
      }

      const auto& raw_cluster = ev_raw_cluster->at(raw_cid);
      ev_out_cluster->emplace_back(raw_cluster);

      ass_ch_rawfilter_to_rawfilter_vv.resize(ev_out_cluster->size());
      auto& ass_ch_rawfilter_to_rawfilter_v = ass_ch_rawfilter_to_rawfilter_vv.back();
      ass_ch_rawfilter_to_rawfilter_v.resize(ass_raw_cluster_v.size());

      // get the out hit associated with raw and set association
      for(size_t raw_hid=0; raw_hid < ass_raw_cluster_v.size(); ++raw_hid) {
      	auto raw_hassid = ass_raw_cluster_v[raw_hid];
      	ass_ch_rawfilter_to_rawfilter_v[raw_hid] = raw_hassid;
      }
    }
    
    ev_ass->set_association(ev_out_cluster->id(), ev_hit_raw->id(), ass_ch_rawfilter_to_rawfilter_vv);
    
    if(_debug) std::cout << "END" << std::endl;

    return true;
  }

  bool DLRawClusterAss::Equals(const larlite::hit& h1, const larlite::hit& h2) {
    if (h1.PeakTime() != h2.PeakTime()) return false;
    if (h1.WireID().Wire != h2.WireID().Wire) return false;
    return true;
  }

}
#endif
