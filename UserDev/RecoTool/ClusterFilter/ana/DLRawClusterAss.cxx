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
    auto ev_out_cluster = (event_cluster*) storage->get_data(data::kCluster,"dlraw");
    auto ev_out_hit     = (event_hit*)     storage->get_data(data::kHit,"dlraw");
    auto ev_ass         = (event_ass*)     storage->get_data(data::kAssociation,"dlraw");

    assert(ev_out_cluster->empty());
    assert(ev_out_hit->empty());
    
    event_hit* ev_hit_dl  = nullptr;
    event_hit* ev_hit_raw = nullptr;
    
    auto const& ass_dl_cluster_vv  = storage->find_one_ass(ev_dl_cluster->id(),  ev_hit_dl,  ev_dl_cluster->name());
    auto const& ass_raw_cluster_vv = storage->find_one_ass(ev_raw_cluster->id(), ev_hit_raw, ev_raw_cluster->name());

    if(!ev_hit_dl)  return true;
    if(!ev_hit_raw) return true;

    if(_debug) {
      std::cout << "GOT " << ev_dl_cluster->size() << " DL clusters" << std::endl; 
      std::cout << "GOT " << ev_hit_dl->size() << " DL hits" << std::endl;
      std::cout << "GOT " << ev_raw_cluster->size() << " RAW clusters" << std::endl; 
      std::cout << "GOT " << ev_hit_raw->size() << " RAW hits" << std::endl;
      std::cout << "GOT " << ev_out_cluster->size() << " OUTPUT clusters" << std::endl;
      std::cout << "GOT " << ev_out_hit->size()     << " OUTPUT hits" << std::endl;
    }


    //
    // copy DL hits & raw hits
    //
    std::vector<const larlite::hit* > tmp_out_hit_v;
    tmp_out_hit_v.reserve(ev_hit_dl->size() + ev_hit_raw->size());

    for(const auto& hit : (*ev_hit_dl))  tmp_out_hit_v.emplace_back(&hit);
    for(const auto& hit : (*ev_hit_raw)) tmp_out_hit_v.emplace_back(&hit);

    std::vector<size_t> dup_v;
    dup_v.resize(tmp_out_hit_v.size(),data::kINVALID_SIZE);

    //
    // remove duplicates
    //
    for(size_t hid1=0; hid1<tmp_out_hit_v.size(); ++hid1) {
      for(size_t hid2=hid1+1; hid2<tmp_out_hit_v.size(); ++hid2) {
	if (Equals(*tmp_out_hit_v[hid1],*tmp_out_hit_v[hid2])) {
	  dup_v[hid1] = hid2;
	}
      }
    }
    
    std::vector<bool> ignore_v(tmp_out_hit_v.size(),false);

    std::vector<const larlite::hit* > out_hit_v;
    out_hit_v.reserve(tmp_out_hit_v.size());

    for(size_t hid=0; hid<tmp_out_hit_v.size(); ++hid) {
      if (ignore_v[hid]) continue;
      out_hit_v.emplace_back(tmp_out_hit_v[hid]);
      if (dup_v[hid] == data::kINVALID_SIZE) continue;
      ignore_v[dup_v[hid]] = true;
    }
    
    //
    // copy to output hits
    //
    std::vector<std::pair<size_t,size_t> > out_to_dl_raw_vp;
    std::vector<size_t> dl_to_out_hit_v;
    std::vector<size_t> raw_to_out_hit_v;

    out_to_dl_raw_vp.resize(out_hit_v.size(),
			    std::make_pair(data::kINVALID_SIZE,data::kINVALID_SIZE));

    dl_to_out_hit_v.resize(ev_hit_dl->size(),data::kINVALID_SIZE);
    raw_to_out_hit_v.resize(ev_hit_raw->size(),data::kINVALID_SIZE);

    for(size_t hid=0; hid<out_hit_v.size(); ++hid) {
      const auto hit_ptr = out_hit_v[hid];
      auto& out_to_dl_raw_p = out_to_dl_raw_vp[hid];

      for(size_t dl_hid=0; dl_hid<ev_hit_dl->size(); ++dl_hid){
	if (Equals(*hit_ptr,ev_hit_dl->at(dl_hid))) {
	  out_to_dl_raw_p.first = dl_hid;
	  dl_to_out_hit_v[dl_hid] = hid;
	  break;
	}
      }

      for(size_t raw_hid=0; raw_hid<ev_hit_raw->size(); ++raw_hid){
	if (Equals(*hit_ptr,ev_hit_raw->at(raw_hid))) {
	  out_to_dl_raw_p.second = raw_hid;
	  raw_to_out_hit_v[raw_hid] = hid;
	  break;
	}
      }

      ev_out_hit->emplace_back(*hit_ptr);
    }
    
    
    //
    // identify overlapping DL & raw clusters
    //
    std::vector<std::vector<int> > dl_to_raw_ctr_vv;
    dl_to_raw_ctr_vv.resize(ass_dl_cluster_vv.size());
    for(auto& v : dl_to_raw_ctr_vv) v.resize(ass_raw_cluster_vv.size(),0);

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

    ev_ass->set_association(ev_dl_cluster->id(), ev_raw_cluster->id(), ass_cc_dl_to_raw_vv); 
    
    if(_debug) std::cout << std::endl;

    //
    // make new dl-raw clusters
    //
    AssSet_t ass_ch_dlraw_to_dlraw_vv;
    ass_ch_dlraw_to_dlraw_vv.resize(ass_dl_cluster_vv.size());

    std::vector<bool> raw_ignore_v(ass_raw_cluster_vv.size(),false);

    //
    // copy to dl cluster and hits to output
    //

    for(size_t dl_cid=0; dl_cid < ass_dl_cluster_vv.size(); ++ dl_cid) {
      const auto& ass_dl_cluster_v = ass_dl_cluster_vv[dl_cid];
      const auto& ass_cc_dl_to_raw_v = ass_cc_dl_to_raw_vv[dl_cid];
      
      const auto& dl_cluster = ev_dl_cluster->at(dl_cid);
      ev_out_cluster->emplace_back(dl_cluster);

      for(auto& raw_cid : ass_cc_dl_to_raw_v) {
    	if (_debug) std::cout << "ignore this @raw_cid=" << raw_cid << std::endl;
	raw_ignore_v[raw_cid] = true;
      }

      auto& ass_ch_dlraw_to_dlraw_v = ass_ch_dlraw_to_dlraw_vv[dl_cid];
      ass_ch_dlraw_to_dlraw_v.resize(ass_dl_cluster_v.size());
      
      // get the out hit associated with DL and set association
      for(size_t dl_hid=0; dl_hid < ass_dl_cluster_v.size(); ++dl_hid) {
	auto dl_hassid = ass_dl_cluster_v[dl_hid];
	auto dl_to_out_hid = dl_to_out_hit_v.at(dl_hassid);
	ass_ch_dlraw_to_dlraw_v[dl_hid] = dl_to_out_hid;
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

      std::vector<unsigned> ass_ch_dlraw_to_dlraw_v;
      ass_ch_dlraw_to_dlraw_v.resize(ass_raw_cluster_v.size());
      
      const auto& raw_cluster = ev_raw_cluster->at(raw_cid);
      ev_out_cluster->emplace_back(raw_cluster);

      // get the out hit associated with raw and set association
      for(size_t raw_hid=0; raw_hid < ass_raw_cluster_v.size(); ++raw_hid) {
      	auto raw_hassid = ass_raw_cluster_v[raw_hid];
      	auto raw_to_out_hid = raw_to_out_hit_v.at(raw_hassid);
      	ass_ch_dlraw_to_dlraw_v[raw_hid] = raw_to_out_hid;
      }
      
      ass_ch_dlraw_to_dlraw_vv.emplace_back(std::move(ass_ch_dlraw_to_dlraw_v));
    }
    
    ev_ass->set_association(ev_out_cluster->id(), ev_out_hit->id(), ass_ch_dlraw_to_dlraw_vv);

    if(_debug) {
      std::cout << "RET " << ev_dl_cluster->size() << " DL clusters" << std::endl; 
      std::cout << "RET " << ev_hit_dl->size() << " DL hits" << std::endl;
      std::cout << "RET " << ev_raw_cluster->size() << " RAW clusters" << std::endl; 
      std::cout << "RET " << ev_hit_raw->size() << " RAW hits" << std::endl;
      std::cout << "RET " << ev_out_cluster->size() << " OUTPUT clusters" << std::endl;
      std::cout << "RET " << ev_out_hit->size()     << " OUTPUT hits" << std::endl;
    }

    
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
