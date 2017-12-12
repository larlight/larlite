#ifndef RECOTOOL_VERTEXCLUSTERMERGER_CXX
#define RECOTOOL_VERTEXCLUSTERMERGER_CXX

#include "VertexClusterMerger.h"
#include <algorithm>

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

#include "DataFormat/pfpart.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {

  VertexClusterMerger::VertexClusterMerger()
  {
    _name = "VertexClusterMerger";
    _fout = 0;
    _clus_producer = "";
    _vtx_producer  = "";
    _output_producer = "";

    _wire2cm  = larutil::GeometryHelper::GetME()->WireToCm();
    _time2cm  = larutil::GeometryHelper::GetME()->TimeToCm();
  
  }

  bool VertexClusterMerger::initialize() {

    _merge_helper.SetAnaFile(_fout);

    return true;
  }

  bool VertexClusterMerger::analyze(storage_manager* storage) {

    std::vector< ::cluster::Cluster > local_clusters;
    std::vector<std::vector<const larlite::hit*> > local_hit_vv;


    std::cout << "requested vertex producer= " << _vtx_producer << std::endl;
    std::cout << "requested cluster producer= " << _clus_producer << std::endl;

    //
    // input
    //
    const auto ev_dl_vertex   = (larlite::event_vertex*)storage->get_data(larlite::data::kVertex,_vtx_producer);
    const auto ev_raw_filter_cluster = (larlite::event_cluster*)storage->get_data(larlite::data::kCluster,_clus_producer);

    //
    // output
    //
    auto ev_out_vertex = (larlite::event_vertex*) storage->get_data(larlite::data::kVertex,"dlraw");
    auto ev_out_pfpart = (larlite::event_pfpart*) storage->get_data(larlite::data::kPFParticle,"dlraw");
    auto ev_out_cluster = (larlite::event_cluster*) storage->get_data(larlite::data::kCluster,"dlraw");
    auto ev_out_hit = (larlite::event_hit*) storage->get_data(larlite::data::kHit,"dlraw");
    auto ev_out_ass = (larlite::event_ass*) storage->get_data(data::kAssociation,"dlraw");

    if (ev_dl_vertex->empty()) {
      std::cout << "no vertex, next!" << std::endl;
      return true;
    }

    //
    // parse input
    //
    larlite::event_pfpart* ev_dl_pfpart = nullptr;
    larlite::event_cluster* ev_dl_cluster = nullptr;
    larlite::event_hit* ev_dl_hit = nullptr;

    larlite::event_hit* ev_raw_filter_hit = nullptr;

    const auto& ass_dl_pfpart_vv = storage->find_one_ass(ev_dl_vertex->id(), ev_dl_pfpart, ev_dl_vertex->name());
    const auto& ass_dl_cluster_vv = storage->find_one_ass(ev_dl_pfpart->id(), ev_dl_cluster, ev_dl_pfpart->name());
    const auto& ass_dl_hit_vv = storage->find_one_ass(ev_dl_cluster->id(), ev_dl_hit, ev_dl_cluster->name());
  
    const auto& ass_raw_filter_hit_vv = storage->find_one_ass(ev_raw_filter_cluster->id(), 
							      ev_raw_filter_hit, 
							      ev_raw_filter_cluster->name());

    std::cout << "GOT dl vertex sz=" << ev_dl_vertex->size() << std::endl;
    std::cout << "GOT dl pfpart sz=" << ev_dl_pfpart->size() << std::endl;
    std::cout << "GOT dl cluster sz=" << ev_dl_cluster->size() << std::endl;
    std::cout << "GOT dl hit sz=" << ev_dl_hit->size() << std::endl;
    std::cout << "GOT raw hits sz=" << ev_raw_filter_hit->size() << std::endl;

    //
    // pull out the hits associated with each raw filter cluster
    //
    std::vector<std::vector<const larlite::hit*> > raw_filter_hit_vv;

    std::cout << std::endl;
    std::cout << "--> dump raw" << std::endl;
    std::cout << std::endl;
    for(size_t clus_id=0; clus_id < ass_raw_filter_hit_vv.size(); ++ clus_id) {
      std::cout << "@clus_id=" << clus_id << std::endl;
      const auto& ass_raw_filter_hit_v = ass_raw_filter_hit_vv.at(clus_id);
    
      raw_filter_hit_vv.resize(raw_filter_hit_vv.size()+1);
      auto& raw_filter_hit_v = raw_filter_hit_vv.back();
    
      std::cout << "GOT: " << ass_raw_filter_hit_v.size() << " associated hits" << std::endl;
      
      for(size_t h_id=0; h_id < ass_raw_filter_hit_v.size(); ++ h_id) {
	const auto hit_id = ass_raw_filter_hit_v.at(h_id);
	const auto& raw_filter_hit = ev_raw_filter_hit->at(hit_id);
	raw_filter_hit_v.push_back(&raw_filter_hit);
      } // end raw_filter hit
    } // end raw_filter cluster
  
    std::vector<std::vector<unsigned int> > ass_vtx_to_pfpart_vv;
    std::vector<std::vector<unsigned int> > ass_pfpart_to_cluster_vv;
    std::vector<std::vector<unsigned int> > ass_cluster_to_hit_vv;

    std::cout << std::endl;
    std::cout << "--> dump dl" << std::endl;
    std::cout << std::endl;

    for( size_t vtx_id = 0; vtx_id < ass_dl_pfpart_vv.size(); ++vtx_id) {
      std::cout << "@vtx_id=" << vtx_id << std::endl;
      //
      // get the dl vertex, cluster, hit
      //
      const auto& vertex = ev_dl_vertex->at(vtx_id);
      const auto& ass_dl_pfpart_v = ass_dl_pfpart_vv.at(vtx_id);

      std::vector<const larlite::pfpart*> dl_pfpart_v;
      std::vector<std::vector<const larlite::cluster* > > dl_cluster_vv;
      std::vector<std::vector<std::vector<const larlite::hit*> > > dl_hit_vvv;
    
      for( size_t pfp_id=0; pfp_id < ass_dl_pfpart_v.size(); ++ pfp_id) {
	std::cout << "@pfp_id=" << pfp_id << std::endl;
	const auto pfpart_id = ass_dl_pfpart_v.at(pfp_id);
	const auto& ass_dl_cluster_v = ass_dl_cluster_vv.at(pfpart_id);
	std::cout << "..." << ass_dl_cluster_v.size() << " ass clusters" << std::endl;


	dl_pfpart_v.push_back(&ev_dl_pfpart->at(pfpart_id));

	dl_cluster_vv.resize(dl_cluster_vv.size()+1);
	auto& dl_cluster_v = dl_cluster_vv.back();
      
	dl_hit_vvv.resize(dl_hit_vvv.size()+1);
	auto& dl_hit_vv = dl_hit_vvv.back();
      
	for(size_t clus_id=0; clus_id < ass_dl_cluster_v.size(); ++ clus_id) {
	  std::cout << "@clus_id=" << clus_id << std::endl;

	  const auto cluster_id = ass_dl_cluster_v.at(clus_id);
	  const auto& ass_dl_hit_v = ass_dl_hit_vv.at(cluster_id);
	  std::cout << "..." << ass_dl_hit_v.size() << " ass hits" << std::endl;	  

	  dl_cluster_v.push_back(&ev_dl_cluster->at(cluster_id));
	  
	  dl_hit_vv.resize(dl_hit_vv.size()+1);
	  auto& dl_hit_v = dl_hit_vv.back();
	  
	  for(size_t h_id=0; h_id < ass_dl_hit_v.size(); ++ h_id) {
	    const auto hit_id = ass_dl_hit_v.at(h_id);
	    dl_hit_v.push_back(&(ev_dl_hit->at(hit_id)));
	  } // end dl hit
	} // end dl cluster
      } // end pfp

      //
      // for each pf particle, try and recluster
      //
      ev_out_vertex->emplace_back(vertex);
      ass_vtx_to_pfpart_vv.resize(ev_out_vertex->size());
      auto& ass_vtx_to_pfpart_v = ass_vtx_to_pfpart_vv.back();
      std::cout << std::endl;
      std::cout << "--> attempt to merge" << std::endl;
      std::cout << std::endl;
      for(size_t pfp_id=0; pfp_id < dl_hit_vvv.size(); ++pfp_id) {
	
	std::cout << "@pfp_id=" << pfp_id << std::endl;
	ass_vtx_to_pfpart_v.push_back(ev_out_pfpart->size());
	ev_out_pfpart->push_back(*(dl_pfpart_v.at(pfp_id)));

	ass_pfpart_to_cluster_vv.resize(ev_out_pfpart->size());
	auto& ass_pfpart_to_cluster_v = ass_pfpart_to_cluster_vv.back();

	local_clusters.clear();
	local_hit_vv.clear();

	const auto dl_hit_vv = dl_hit_vvv.at(pfp_id);
	std::cout << "dl_hit_vv sz=" << dl_hit_vv.size() << std::endl;
	if (dl_hit_vv.empty()) continue;

	local_hit_vv.resize(local_hit_vv.size() + raw_filter_hit_vv.size());

	std::vector<bool> isdl_v(local_hit_vv.size(),false);
	
	//set the dl hits
	for(size_t dl_hit_id=0; dl_hit_id < dl_hit_vv.size(); ++dl_hit_id) {
	  const auto& dl_hit_v = dl_hit_vv[dl_hit_id];
	  local_hit_vv[dl_hit_id] = dl_hit_v;
	  isdl_v[dl_hit_id] = true;
	}

	// set the raw hits
	for(size_t raw_filter_hit_id=dl_hit_vv.size(); raw_filter_hit_id < local_hit_vv.size(); ++raw_filter_hit_id) {
	  const auto& raw_filter_hit_v = raw_filter_hit_vv[raw_filter_hit_id];
	  local_hit_vv[raw_filter_hit_id] = raw_filter_hit_v;
	}

	std::cout << "local_hit_vv sz=" << local_hit_vv.size() << std::endl;

	_cluster_maker.MakeClusters(local_hit_vv, vertex, local_clusters);

	std::cout << "local clusters sz=" << local_clusters.size() << std::endl;
    
	_merge_helper.Process(local_clusters);
    
	const auto& bk = _merge_helper.GetResult();
    
	std::vector<std::vector<unsigned short> > merged_indexes;
	bk.PassResult(merged_indexes);

	std::cout << "merged_indexes sz=" << merged_indexes.size() << std::endl;

	// only merge with dl hits
	for(size_t mi=0; mi < merged_indexes.size(); ++mi) {

	  bool dlseed = false;
	  for(auto idx : merged_indexes.at(mi))
	    if(isdl_v[idx]) dlseed=true;
	  
	  if(!dlseed) continue;

	  std::cout << "@mi=" << mi << std::endl;
	  ass_pfpart_to_cluster_v.push_back(ev_out_cluster->size());
	  ev_out_cluster->resize(ev_out_cluster->size()+1);
	  auto& out_cluster = ev_out_cluster->back();
	  out_cluster.set_id(ev_out_cluster->size()-1);

	  ass_cluster_to_hit_vv.resize(ev_out_cluster->size());
	  auto& ass_cluster_to_hit_v = ass_cluster_to_hit_vv.back();


	  for(auto idx : merged_indexes.at(mi)) {
	    std::cout << "\tidx=" << idx << std::endl;
	    auto& local_hit_v = local_hit_vv.at(idx);
	    if (local_hit_v.empty()) continue;
	    
	    out_cluster.set_view(local_hit_v.front()->View());
	    larlite::geo::PlaneID c_plane_id(larlite::data::kINVALID_UINT,
					     larlite::data::kINVALID_UINT,
					     local_hit_v.front()->WireID().Plane);
	    out_cluster.set_planeID(c_plane_id);

	    for(auto hit_ptr : local_hit_v) {
	      ass_cluster_to_hit_v.push_back(ev_out_hit->size());
	      ev_out_hit->emplace_back(*hit_ptr);
	    }
	  }


	}
      } // end pf particle
    } // end vertex

    ev_out_ass->set_association(ev_out_vertex->id(), ev_out_pfpart->id(), ass_vtx_to_pfpart_vv);
    ev_out_ass->set_association(ev_out_pfpart->id(), ev_out_cluster->id(), ass_pfpart_to_cluster_vv);
    ev_out_ass->set_association(ev_out_cluster->id(), ev_out_hit->id(), ass_cluster_to_hit_vv);

    return true;
  }

}
#endif
