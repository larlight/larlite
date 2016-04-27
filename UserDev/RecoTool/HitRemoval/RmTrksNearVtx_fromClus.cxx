#ifndef LARLITE_RMTRKSNEARVTX_FROMCLUS_CXX
#define LARLITE_RMTRKSNEARVTX_FROMCLUS_CXX

#include "RmTrksNearVtx_fromClus.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/vertex.h"
#include "DataFormat/PiZeroROI.h"
#include "LArUtil/GeometryHelper.h"

namespace larlite {

  RmTrksNearVtx_fromClus::RmTrksNearVtx_fromClus() {
    _name="RmTrksNearVtx_fromClus";
    _fout=0;
    _use_mc = false;
    _save_unclustered_hits = true;
  }

  bool RmTrksNearVtx_fromClus::initialize() {

    return true;
  }
  
  bool RmTrksNearVtx_fromClus::analyze(storage_manager* storage) {

    // produce new shower-like hits
    auto ev_shr_hits = storage->get_data<event_hit>("shrlike");

    // produce new shower-like clusters
    auto ev_shr_clus = storage->get_data<event_cluster>("shrlike");

    // grab input clusters
    auto ev_clus = storage->get_data<event_cluster>(_clus_producer);

    // produce cluster -> hit association
    auto *ev_clus_hit_ass = storage->get_data<event_ass>("shrlike");
    std::vector<std::vector<unsigned int> > shr_clus_hit_ass;

    // grab vtx to be used (when using MC)
    auto *ev_vtx = storage->get_data<event_vertex>(_vtx_producer);

    // grab Pi0ROI to be used for vtx (when using RECO)
    auto *ev_roi = storage->get_data<event_PiZeroROI>(_roi_producer);

    storage->set_id(storage->run_id(), storage->subrun_id(), storage->event_id());

    // if no clusters -> exit
    if ( (!ev_clus) or (ev_clus->size() == 0) ){
      std::cout << "No Cluster -> skip event" << std::endl;
      return false;
    }

    if ( ( !ev_vtx or (ev_vtx->size() == 0) ) and _use_mc ){
      std::cout << "No Vertex -> return" << std::endl;
      return false;
    }

    if ( ( !ev_roi or (ev_roi->size() == 0) ) and !_use_mc ){
      std::cout << "No Pi0ROI -> return" << std::endl;
      return false;
    }

    // geometry helper, to be used across the module
    auto const* geom = larutil::GeometryHelper::GetME();
        
    // get w -> cm
    auto const& w2cm = geom->WireToCm();
    // get t -> cm
    auto const& t2cm = geom->TimeToCm();
    
    if (_use_mc){
      // get vertex position on the various planes (in cm)
      std::vector<double> xyz(3,0);
      auto const& vtx = ev_vtx->at(0);
      xyz = {vtx.X() + 960 * t2cm ,vtx.Y(),vtx.Z()};
      vtx_w_cm = {0.,0.,0.};
      vtx_t_cm = {0.,0.,0.};
      for (size_t pl=0; pl < 3; pl++){
	auto const& vtx_pl = geom->Point_3Dto2D(xyz,pl);
	vtx_w_cm[pl] = vtx_pl.w;
	vtx_t_cm[pl] = vtx_pl.t;
      }
    }
    else{
      auto const& roi = ev_roi->at(0);
      vtx_w_cm = {0.,0.,0.};
      vtx_t_cm = {0.,0.,0.};
      for (size_t pl=0; pl < 3; pl++){
	vtx_t_cm[pl] = roi.GetVertex().at(pl).first * t2cm;
	vtx_w_cm[pl] = roi.GetVertex().at(pl).second * w2cm;
      }
    }

    /*
    std::cout << "Vtx U [" << vtx_w_cm[0] << ", " << vtx_t_cm[0] << "].\t"
	      << "Vtx V [" << vtx_w_cm[1] << ", " << vtx_t_cm[1] << "].\t"
	      << "Vtx Y [" << vtx_w_cm[2] << ", " << vtx_t_cm[2] << "].\t" << std::endl;
    */

    // get hits associated with the clusters
    larlite::event_hit *ev_hit = nullptr;
    auto & hits_ass_to_cluster    = storage->find_one_ass(ev_clus->id(), ev_hit, ev_clus->name());

    // if no hits -> return false
    if ( (!ev_hit) or (ev_hit->size() == 0) ){
      std::cout << "No hit associated to cluster " << ev_clus->name() <<  " -> skip event" << std::endl;
      return false;
    }

    // prepare a list of hit indices to be skipped. We will add all hits in the event
    // but not these hits, which come from clusters considered "trk-like"
    std::vector<size_t> hits_tobe_removed;

    // loop through Clusters
    for (size_t i=0; i < ev_clus->size(); i++){
      
      bool track = false;
      
      // and finally the hits associated to that PFPart
      auto ass_hit_idx_v = hits_ass_to_cluster[i];
      // add all these hits to the new producer
      for (auto const& hit_idx : ass_hit_idx_v){
	// figure out if hit is close to the vertex
	
	auto const& hit = ev_hit->at(hit_idx);
	
	// get plane
	auto const& pl = hit.WireID().Plane;
	
	// get the hit position in polar coordinates
	double hit_t_cm = hit.PeakTime() * t2cm;
	double hit_w_cm = hit.WireID().Wire * w2cm;
	
	double r = sqrt( (hit_t_cm - vtx_t_cm[pl]) * (hit_t_cm - vtx_t_cm[pl]) +
			 (hit_w_cm - vtx_w_cm[pl]) * (hit_w_cm - vtx_w_cm[pl]) );
	
	if (r < 2){
	  track = true;
	  //std::cout << "Found cluster to be removed..." << std::endl;
	  break;
	}
      }// for all hits in the cluster

      // if it is track-like, re-loop over hit indices and add them
      // to lise of to-be-removed hits
      if ( (track == true) && _save_unclustered_hits ){
	for (auto const& hit_idx : ass_hit_idx_v)
	  hits_tobe_removed.push_back(hit_idx);
      }

      if ( (track == false) && !_save_unclustered_hits ){
	// save hits to output cluster
	
	// and finally the hits associated to that cluster
	auto ass_hit_idx_v = hits_ass_to_cluster[i];
	// add all these hits to the new producer
	for (auto const& hit_idx : ass_hit_idx_v)
	  ev_shr_hits->emplace_back( ev_hit->at( hit_idx ) );
	
	//add cluster & cluster -> hit association to the output "shrlike" clusters
	ev_shr_clus->emplace_back( ev_clus->at(i) );
	shr_clus_hit_ass.push_back( ass_hit_idx_v );
	
      }// if track is false
      
      
    }// for all clusters associated to this PFParticle

    // now loop back through all hits.
    // those hits not in the "to be removed" list will be saved to the output

    // if we are interested in saving all un-clustered hits
    if (_save_unclustered_hits){
      
      for (size_t idx = 0; idx < ev_hit->size(); idx++){
	
	if ( std::find(hits_tobe_removed.begin(), hits_tobe_removed.end(), idx) == hits_tobe_removed.end() )
	  ev_shr_hits->emplace_back( ev_hit->at( idx ) );
	
      }// for all hits
    }// if we want to save unclustered hits
    
    ev_clus_hit_ass->set_association(ev_shr_clus->id(),product_id(data::kHit,ev_hit->name()), shr_clus_hit_ass);    
  
    return true;
  }

  bool RmTrksNearVtx_fromClus::finalize() {

    return true;
  }

}
#endif
