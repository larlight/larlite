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

    if (_threshold<0) {
      std::cout << "negative threshold?" << std::endl;
      throw std::exception();
    }

    auto ev_dl_cluster  = (event_cluster*) storage->get_data(larlite::data::kCluster,"dl");    
    auto ev_raw_cluster = (event_cluster*) storage->get_data(larlite::data::kCluster,"raw");
 
    auto ev_ass = (event_ass*) storage->get_data(larlite::data::kAssociation,"dlraw");
    
    event_hit* ev_hit_dl  = nullptr;
    event_hit* ev_hit_raw = nullptr;
    
    auto const& ass_dl_cluster_vv  = storage->find_one_ass(ev_dl_cluster->id(),  ev_hit_dl,  ev_dl_cluster->name());
    auto const& ass_raw_cluster_vv = storage->find_one_ass(ev_raw_cluster->id(), ev_hit_raw, ev_raw_cluster->name());
    
    std::vector<std::vector<int> > dl_to_raw_ctr_vv;
    dl_to_raw_ctr_vv.resize(ass_dl_cluster_vv.size());
    for(auto& v : dl_to_raw_ctr_vv) v.resize(ass_raw_cluster_vv.size(),0);

    //dl cluster
    for(size_t dl_cid = 0; dl_cid < ass_dl_cluster_vv.size(); ++dl_cid) {
      auto ass_dl_cluster_v = ass_dl_cluster_vv[dl_cid];
      const auto& dl_cluster = ev_dl_cluster->at(dl_cid);
      auto& dl_to_raw_ctr_v = dl_to_raw_ctr_vv[dl_cid];

      //raw cluster
      for(size_t raw_cid = 0; raw_cid < ass_raw_cluster_vv.size(); ++raw_cid) {
	auto ass_raw_cluster_v = ass_raw_cluster_vv[raw_cid];
	const auto& raw_cluster = ev_raw_cluster->at(raw_cid);
	
	auto& dl_to_raw_ctr = dl_to_raw_ctr_v[raw_cid];

	if (dl_cluster.Plane() != raw_cluster.Plane()) continue;

	//dl hit
	for(size_t dl_hid=0; dl_hid < ass_dl_cluster_v.size(); ++dl_hid) {
	  auto dl_hassid = ass_dl_cluster_v[dl_hid];
	  const auto& dl_hit = ev_hit_dl->at(dl_hassid);
	  
	  //raw hit
	  for(size_t raw_hid=0; raw_hid < ass_raw_cluster_v.size(); ++raw_hid) {
	    auto raw_hassid = ass_raw_cluster_v[raw_hid];
	    const auto& raw_hit = ev_hit_raw->at(raw_hassid);
	    
	    assert(dl_hit.View() == raw_hit.View());
	    
	    if (Equals(dl_hit,raw_hit)) dl_to_raw_ctr++;

	  } //raw hit
	} //dl hit
      } //raw cluster
    } //dl cluster

    larlite::AssSet_t ass_dl_to_raw_vv;
    ass_dl_to_raw_vv.resize(dl_to_raw_ctr_vv.size());

    for(size_t dl_cid=0; dl_cid < dl_to_raw_ctr_vv.size(); ++dl_cid) {
      auto& ass_dl_to_raw_v = ass_dl_to_raw_vv[dl_cid];
      auto& dl_to_raw_ctr_v = dl_to_raw_ctr_vv[dl_cid];

      ass_dl_to_raw_v.clear();

      for(size_t raw_cid=0; raw_cid < dl_to_raw_ctr_v.size(); ++raw_cid) {
	auto dl_raw_ctr = dl_to_raw_ctr_v[raw_cid];
	if (dl_raw_ctr>_threshold)  {
	  std::cout << "ASS (dl,raw)=(" << dl_cid << "," << raw_cid << ") !" << std::endl;
	  ass_dl_to_raw_v.push_back(raw_cid);
	}
      }
    }
    
    ev_ass->set_association(ev_dl_cluster->id(), ev_raw_cluster->id(), ass_dl_to_raw_vv); 
    
    return true;
  }

  bool Equals(const larlite::hit& h1, const larlite::hit& h2) {
    if (h1.PeakTime() != h2.PeakTime()) return false;
    if (h1.WireID().Wire != h2.WireID().Wire) return false;
    return true;
  }

}
#endif
