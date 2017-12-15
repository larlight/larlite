#ifndef LARLITE_COPYDL2DLRAW_CXX
#define LARLITE_COPYDL2DLRAW_CXX

#include "CopyDL2DLRaw.h"
#include <cassert>


#include "DataFormat/pfpart.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/event_ass.h"

namespace larlite {

  
  bool CopyDL2DLRaw::analyze(storage_manager* storage) {

    assert(!_input_producer.empty());
    assert(!_output_producer.empty());
    
    //
    // input
    //
    const auto ev_dl_vertex   = (larlite::event_vertex*)storage->get_data(larlite::data::kVertex,_input_producer);

    //
    // output
    //
    auto ev_out_vertex  = (larlite::event_vertex*) storage->get_data(larlite::data::kVertex,_output_producer);
    auto ev_out_pfpart  = (larlite::event_pfpart*) storage->get_data(larlite::data::kPFParticle,_output_producer);
    auto ev_out_cluster = (larlite::event_cluster*) storage->get_data(larlite::data::kCluster,_output_producer);
    auto ev_out_hit     = (larlite::event_hit*) storage->get_data(larlite::data::kHit,_output_producer);
    auto ev_out_ass     = (larlite::event_ass*) storage->get_data(data::kAssociation,_output_producer);


    if (ev_dl_vertex->empty()) return true;

    //
    // parse input
    //
    larlite::event_pfpart* ev_dl_pfpart = nullptr;
    larlite::event_cluster* ev_dl_cluster = nullptr;
    larlite::event_hit* ev_dl_hit = nullptr;

    const auto& ass_dl_pfpart_vv = storage->find_one_ass(ev_dl_vertex->id(), ev_dl_pfpart, ev_dl_vertex->name());
    const auto& ass_dl_cluster_vv = storage->find_one_ass(ev_dl_pfpart->id(), ev_dl_cluster, ev_dl_pfpart->name());
    const auto& ass_dl_hit_vv = storage->find_one_ass(ev_dl_cluster->id(), ev_dl_hit, ev_dl_cluster->name());

    std::cout << "GOT dl vertex sz=" << ev_dl_vertex->size() << std::endl;
    std::cout << "GOT dl pfpart sz=" << ev_dl_pfpart->size() << std::endl;
    std::cout << "GOT dl cluster sz=" << ev_dl_cluster->size() << std::endl;
    std::cout << "GOT dl hit sz=" << ev_dl_hit->size() << std::endl;

    std::vector<std::vector<unsigned int> > ass_vtx_to_pfpart_vv;
    std::vector<std::vector<unsigned int> > ass_pfpart_to_cluster_vv;
    std::vector<std::vector<unsigned int> > ass_cluster_to_hit_vv;

    for( size_t vtx_id = 0; vtx_id < ass_dl_pfpart_vv.size(); ++vtx_id) {

      const auto& vertex = ev_dl_vertex->at(vtx_id);
      const auto& ass_dl_pfpart_v = ass_dl_pfpart_vv.at(vtx_id);

      std::vector<const larlite::pfpart*> dl_pfpart_v;
      std::vector<std::vector<const larlite::cluster* > > dl_cluster_vv;
      std::vector<std::vector<std::vector<const larlite::hit*> > > dl_hit_vvv;
    
      for( size_t pfp_id=0; pfp_id < ass_dl_pfpart_v.size(); ++ pfp_id) {
	const auto pfpart_id = ass_dl_pfpart_v.at(pfp_id);
	const auto& ass_dl_cluster_v = ass_dl_cluster_vv.at(pfpart_id);

	dl_pfpart_v.push_back(&ev_dl_pfpart->at(pfpart_id));

	dl_cluster_vv.resize(dl_cluster_vv.size()+1);
	auto& dl_cluster_v = dl_cluster_vv.back();
      
	dl_hit_vvv.resize(dl_hit_vvv.size()+1);
	auto& dl_hit_vv = dl_hit_vvv.back();
      
	for(size_t clus_id=0; clus_id < ass_dl_cluster_v.size(); ++ clus_id) {
	  const auto cluster_id = ass_dl_cluster_v.at(clus_id);
	  const auto& ass_dl_hit_v = ass_dl_hit_vv.at(cluster_id);

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
      // copy vertex, pfparticle, hit
      //
      ev_out_vertex->emplace_back(vertex);
      ass_vtx_to_pfpart_vv.resize(ev_out_vertex->size());
      auto& ass_vtx_to_pfpart_v = ass_vtx_to_pfpart_vv.back();
      for(size_t pfp_id=0; pfp_id < dl_hit_vvv.size(); ++pfp_id) {

	const auto& dl_hit_vv = dl_hit_vvv.at(pfp_id);
	
	ass_vtx_to_pfpart_v.push_back(ev_out_pfpart->size());
	ev_out_pfpart->push_back(*(dl_pfpart_v.at(pfp_id)));

	ass_pfpart_to_cluster_vv.resize(ev_out_pfpart->size());
	auto& ass_pfpart_to_cluster_v = ass_pfpart_to_cluster_vv.back();

	for(size_t clus_id=0; clus_id < dl_hit_vv.size(); ++clus_id) {

	  const auto& dl_hit_v = dl_hit_vv[clus_id];

	  ass_pfpart_to_cluster_v.push_back(ev_out_cluster->size());
	  ev_out_cluster->resize(ev_out_cluster->size()+1);
	  auto& out_cluster = ev_out_cluster->back();
	  out_cluster.set_id(ev_out_cluster->size()-1);

	  ass_cluster_to_hit_vv.resize(ev_out_cluster->size());
	  auto& ass_cluster_to_hit_v = ass_cluster_to_hit_vv.back();

	  out_cluster.set_view(dl_cluster_vv[pfp_id][clus_id]->View());
	  out_cluster.set_planeID(dl_cluster_vv[pfp_id][clus_id]->Plane());

	  for(auto hit_ptr : dl_hit_v) {
	    ass_cluster_to_hit_v.push_back(ev_out_hit->size());
	    ev_out_hit->emplace_back(*hit_ptr);
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
