#ifndef LARLITE_MERGEALIGNEDCLUSTERS_CXX
#define LARLITE_MERGEALIGNEDCLUSTERS_CXX

#include "MergeAlignedClusters.h"

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/vertex.h"

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

namespace larlite {

  MergeAlignedClusters::MergeAlignedClusters() 
  {
    
    _pfp_producer = "";
    _vtx_producer = "";
    _name="MergeAlignedClusters";
    _fout=0;

    auto geomH = larutil::GeometryHelper::GetME();
    
    _w2cm = geomH->WireToCm();
    _t2cm = geomH->TimeToCm();
    
  }

  bool MergeAlignedClusters::initialize() {

    if ( (_pfp_producer == "") || (_vtx_producer == "") ) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("Vtx and/or PFPart producers not specified. Quit"));
      return false;
    }
    
    return true;
  }
  
  bool MergeAlignedClusters::analyze(storage_manager* storage) {

    _pfp_clus_map.clear();
    _clus_pos.clear();
    _clus_angle.clear();

    auto ev_vtx    = storage->get_data<event_vertex>(_vtx_producer);

    if (ev_vtx->size() != 1) {
      print(larlite::msg::kERROR, __FUNCTION__, Form("!= 1 vertices! Error."));
      return false;
    }

    auto geom = larutil::Geometry::GetME();
    auto geoH = larutil::GeometryHelper::GetME();
    double *origin;
    origin = new double[3];
    geom->PlaneOriginVtx(2,origin);
    auto const& vtx = ev_vtx->at(0);
    std::vector<double> xyz = {vtx.X(), vtx.Y(), vtx.Z()};
    auto const& pt = geoH->Point_3Dto2D(xyz,2);
    _vtxW = pt.w;
    _vtxT = pt.t + 800 * _t2cm - origin[0];
    
    
    auto ev_pfpart = storage->get_data<event_pfpart>(_pfp_producer);

    larlite::event_cluster* ev_cluster = nullptr;
    larlite::event_hit*     ev_hit     = nullptr;

    auto ev_pfpart_new  = storage->get_data<event_pfpart>(_out_pfp_producer);
    auto ev_cluster_new = storage->get_data<event_cluster>(_out_cluster_producer);
    // create a new pfpart -> cluster association
    larlite::event_ass* ev_pfpart_cluster_ass_v_new = storage->get_data<event_ass>( ev_pfpart_new->name() );
    // create a new cluster -> hit association
    larlite::event_ass* ev_cluster_hit_ass_v_new    = storage->get_data<event_ass>( ev_cluster_new->name() );

    //set event ID through storage manager
    storage->set_id(storage->run_id(),storage->subrun_id(),storage->event_id());

    // if no imput showers -> quit
    if ( !ev_pfpart || (ev_pfpart->size() == 0) )
      return true;
    
    auto const& ass_pfpart_clus_v = storage->find_one_ass(ev_pfpart->id(), ev_cluster, ev_pfpart->name());
    auto const& ass_clus_hit_v = storage->find_one_ass(ev_cluster->id(), ev_hit, ev_cluster->name());
    // vector to hold associated hits to cluster
    std::vector< std::vector< unsigned int> > clus_hit_ass_holder_new;
    // vector to hold pfpart -> cluster ass
    std::vector< std::vector< unsigned int> > pfpart_clus_ass_holder_new;
    

    // loop through PFParticles, for each grab the collection-plane showers
    for (size_t p=0; p < ev_pfpart->size(); p++) {

      // grab associated clusters
      auto clus_idx_v = ass_pfpart_clus_v.at(p);

      // how many?
      if (clus_idx_v.size() < 2) {
	print(larlite::msg::kERROR, __FUNCTION__, Form("This PFPart has less than 2 clusters associated to it!"));
	continue;
      }

      // grab collection-plane cluster
      for (auto const& clus_idx : clus_idx_v) {

	auto const& clus = ev_cluster->at(clus_idx);

	// what is the plane?
	if (clus.Plane().Plane == 2) {

	  auto const& sW = clus.StartWire() * _w2cm - _vtxW;
	  auto const& sT = clus.StartTick() * _t2cm - _vtxT;
	  double rmin = sqrt(sW*sW + sT*sT);
	  auto const& eW = clus.EndWire()   * _w2cm - _vtxW;
	  auto const& eT = clus.EndTick()   * _t2cm - _vtxT;
	  double rmax = sqrt(eW*eW + eT*eT);

	  if (_debug){
	    std::cout << "PFPart index " << p << std::endl;
	    std::cout << "sW : " << sW << ", sT " << sT << std::endl;
	    std::cout << "eW : " << eW << ", eT " << eT << std::endl;
	    std::cout << std::endl;
	  }
	  
	  _clus_pos  [ p ]   = std::make_pair( rmin, rmax );
	  _clus_angle[ p ]   = 180. * atan2( sT, sW ) / 3.1415;
	  _pfp_clus_map[ p ] = clus_idx;
	  
	}// if plane == 2

      }// for all clusters

    }// for all PFParticles

    // vector of pairs to be merged
    std::vector< std::pair<size_t, size_t> > pairs_to_merge;

    // are there 2 or more PFParticles?
    if (_clus_pos.size() > 1) {

      // loop through PFParticles and their info
      for (auto const& entry1 : _clus_pos) {
	
	auto const& pfpidx1 = entry1.first;
	auto const& pos1    = entry1.second;
	auto const& Rmin1   = pos1.first;
	auto const& Rmax1   = pos1.second;
	auto const& angle1  = _clus_angle[ pfpidx1 ];

	if (_debug){
	  std::cout << "PFPindex1 : " << pfpidx1 << std::endl;
	  std::cout << "Angle1 : " << angle1 << std::endl;
	  std::cout << "Rmin1 = " << Rmin1 << "\t Rmax1 = " << Rmax1 << std::endl << std::endl;
	}
	
	for (auto const& entry2 : _clus_pos) {

	  auto const& pfpidx2 = entry2.first;

	  if (pfpidx2 == pfpidx1) continue;
	  
	  auto const& pos2    = entry2.second;
	  auto const& Rmin2   = pos2.first;
	  auto const& Rmax2   = pos2.second;
	  auto const& angle2  = _clus_angle[ pfpidx2 ];

	  if (_debug) {
	    std::cout << "\t PFPindex2 : " << pfpidx2 << std::endl;
	    std::cout << "\t Angle2 : " << angle2 << std::endl;
	    std::cout << "\t Rmin2 = " << Rmin2 << "\t Rmax2 = " << Rmax2 << std::endl << std::endl;
	  }

	  // decide if to merge
	  
	  if ( (Rmin2 > Rmax1) && (fabs(angle1-angle2) < _anglediff) ) {
	    
	    // merge!
	    if (_debug) std::cout << "**** Merge! *******" << std::endl << std::endl;

	    // is the 1st cluster already being merged with someone? if so add 2nd to that someone
	    size_t parent = pfpidx1;
	    for (auto const& pair : pairs_to_merge) 
	      if (pfpidx1 == pair.second) { parent = pair.first; }
	    
	    // 2 is downstream of 1. Merge 2 into 1.
	    pairs_to_merge.push_back( std::make_pair(parent,pfpidx2) );
	    
	  }// if clusters should be merged
	  
	}// for all PFParticles
      }// for all PFParticles
      
    }// if at least 2 PFParticles

    // loop through all PFParticles, if to be merged -> merge
    for (size_t p=0; p < ev_pfpart->size(); p++) {

      // is this one of the PFParticles t be merged?
      bool tomerge = false;
      for (auto const& pfpair : pairs_to_merge) {
	if ( (p == pfpair.first) || (p == pfpair.second) ){
	  tomerge = true;
	  break;
	}
      }
      if (tomerge == true) continue;
      
      // grab associated cluster indices
      auto clus_idx_v = ass_pfpart_clus_v.at(p);

      // add all the information to the new producers
      ev_pfpart_new->emplace_back( ev_pfpart->at( p ) );

      for (auto const& clus_idx : clus_idx_v) {
	ev_cluster_new->emplace_back( ev_cluster->at( clus_idx ) );
	clus_hit_ass_holder_new.push_back( ass_clus_hit_v.at( clus_idx ) );
      }

      // vector of newly created cluster indices
      std::vector<unsigned int> new_clus_indices;
      for (size_t i=0; i < clus_idx_v.size(); i++) 
	new_clus_indices.push_back( ev_cluster_new->size()-1-i);

      pfpart_clus_ass_holder_new.push_back( new_clus_indices );

    }// for all PFParticles

    // loop through PFParticles to be merged

    std::vector< size_t > added_parents_v;
    
    for (auto const& pair : pairs_to_merge) {
      
      auto const& parent = pair.first;
      // if we have already added this parent -> skip
      if (std::find(added_parents_v.begin(),added_parents_v.end(),parent) != added_parents_v.end() ) continue;

      std::vector<size_t> children_v;

      for (auto const& pair2 : pairs_to_merge) 
	if (pair2.first == parent) { children_v.push_back( pair2.second ); }

      // create new cluster
      larlite::cluster new_clus;

      // grab hits from old clusters
      std::vector<unsigned int> new_clus_hit_idx_v;

      auto const& parent_clus_idx = _pfp_clus_map[parent];
      auto const& parent_clus_hit_idx_v = ass_clus_hit_v.at( parent_clus_idx );
      for (auto const& hit_idx : parent_clus_hit_idx_v) 
	new_clus_hit_idx_v.push_back( hit_idx );
      
      for (auto const& child : children_v){
	auto const& child_clus_idx = _pfp_clus_map[child];
	auto const& child_clus_hit_idx_v = ass_clus_hit_v.at( child_clus_idx );
	for (auto const& hit_idx : child_clus_hit_idx_v) 
	  new_clus_hit_idx_v.push_back( hit_idx );
      }// for all children

      new_clus.set_n_hits ( new_clus_hit_idx_v.size() );
      new_clus.set_view( ev_hit->at( parent_clus_hit_idx_v.at(0) ).View() );
      auto const& parent_clus = ev_cluster->at( parent_clus_idx );
      new_clus.set_start_wire ( parent_clus.StartWire(), 1);
      new_clus.set_start_tick ( parent_clus.StartTick(), 1);
      new_clus.set_end_wire   ( parent_clus.EndWire()  , 1);
      new_clus.set_end_tick   ( parent_clus.EndTick()  , 1);
      new_clus.set_start_angle( parent_clus.StartAngle()  );
      new_clus.set_planeID    ( parent_clus.Plane()       );

      ev_pfpart_new->emplace_back( ev_pfpart->at( parent ) );
      ev_cluster_new->emplace_back( new_clus );
      std::vector<unsigned int> new_cluster_idx_v;
      new_cluster_idx_v.push_back( ev_cluster_new->size() - 1 );

      clus_hit_ass_holder_new.push_back( new_clus_hit_idx_v );
      // find non-collection-plane cluster to associate to this pfparticle
      auto clus_idx_v = ass_pfpart_clus_v.at(parent);

      for (auto const& clus_idx : clus_idx_v) {
	ev_cluster_new->emplace_back( ev_cluster->at( clus_idx ) );
	clus_hit_ass_holder_new.push_back( ass_clus_hit_v.at( clus_idx ) );
	new_cluster_idx_v.push_back( ev_cluster_new->size() - 1 );
      }

      pfpart_clus_ass_holder_new.push_back( new_cluster_idx_v );
    }

    ev_cluster_hit_ass_v_new   ->set_association( ev_cluster_new->id(), product_id(data::kHit,ev_hit->name()),              clus_hit_ass_holder_new    );
    ev_pfpart_cluster_ass_v_new->set_association( ev_pfpart_new->id(),  product_id(data::kCluster,ev_cluster_new->name()),  pfpart_clus_ass_holder_new );    
      
    return true;
  }

  bool MergeAlignedClusters::finalize() {

    return true;
  }

}
#endif
