
#ifndef EVD_DRAWNEUTRINO_CXX
#define EVD_DRAWNEUTRINO_CXX

#include "DrawNeutrino.h"
#include "DataFormat/event_ass.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace evd {
  
  DrawNeutrino::DrawNeutrino() {
    _name = "DrawNeutrino";
    _fout = 0;
  }
  
  bool DrawNeutrino::initialize() {
    
    // Resize data holder
    if (_dataByPlane.size() != geoService -> Nviews()) {
      _dataByPlane.resize(geoService -> Nviews());
    }
    return true;
  }
  
  bool DrawNeutrino::analyze(larlite::storage_manager* storage) {

    for (unsigned int p = 0; p < geoService -> Nviews(); p ++)
      _dataByPlane.at(p).clear();

    std::cout << "Trying to find the neutrino!" << std::endl;
    
    // get a handle to the association
    auto ev_ass = storage->get_data<larlite::event_ass>(_producer);

    // get the association keys
    auto const& ass_keys = ev_ass->association_keys();

    std::cout << "Found " << ass_keys.size() << " ass keys" << std::endl;


    //std::cout << "key 0 first  int : " << ass_keys[0].first.first << " string : "<< ass_keys[0].first.second << std::endl;
    //std::cout << "key 0 second int : " << ass_keys[0].second.first << " string : "<< ass_keys[0].second.second << std::endl;    
    //std::cout << "key 1 first  int : " << ass_keys[1].first.first << " string : "<< ass_keys[1].first.second << std::endl;
    //std::cout << "key 1 second int : " << ass_keys[1].second.first << " string : "<< ass_keys[1].second.second << std::endl;

    larlite::AssSet_t ass_trk_vtx_v;
    larlite::event_track *ev_trk = nullptr;
    ass_trk_vtx_v = storage->find_one_ass( ass_keys[0].second, ev_trk, ev_ass->name() );

    larlite::AssSet_t ass_vtx_trk_v;
    larlite::event_vertex *ev_vtx = nullptr;
    ass_vtx_trk_v = storage->find_one_ass( ass_keys[0].first, ev_vtx, ev_ass->name() );

    // are there tracks? are there vertices?
    if (!ev_trk or (ev_trk->size() == 0)){
      std::cout << "No track! exit" << std::endl;
      return false;
    }
    if (!ev_vtx or (ev_vtx->size() == 0)){
      std::cout << "No vertex! exit" << std::endl;
      return false;
    }

    // grab PFParticles associated with these tracks
    larlite::AssSet_t ass_trk_pfpart_v;
    larlite::event_pfpart *ev_pfpart = nullptr;
    ass_trk_pfpart_v = storage->find_one_ass( ev_trk->id(), ev_pfpart, ev_trk->name() );


    // draw the full neutrino interaction? only if pfparticles are found
    bool pfpart = true;
    
    if (!ev_pfpart or (ev_pfpart->size() == 0)){
      std::cout << "No PFPart associated to traks! exit" << std::endl;
      pfpart = false;
    }

    // and now grab tracks associated to the same PFParts
    larlite::AssSet_t ass_pfpart_trk_v;
    larlite::event_track *ev_trk_2 = nullptr;
    if (pfpart){
      ass_pfpart_trk_v = storage->find_one_ass( ev_pfpart->id(), ev_trk_2, ev_pfpart->name() );
      
      if (!ev_trk_2 or (ev_trk_2->size() == 0)){
	std::cout << "No track associated to PFPart! exit" << std::endl;
	pfpart = false;
      }
    }

    // get the pfpart -> cluster -> hit association.
    bool showers = true;
    
    larlite::AssSet_t ass_pfpart_clus_v;
    larlite::event_cluster *ev_clus = nullptr;
    if (showers){
      ass_pfpart_clus_v = storage->find_one_ass( ev_pfpart->id(), ev_clus, ev_pfpart->name() );
      if (!ev_clus or (ev_clus->size() == 0)){
	std::cout << "No clusters associated to PFPart! exit" << std::endl;
	showers = false;
      }
    }

    std::cout << "Found cluster " << ev_clus->name() << " associated w/ PFPart " << ev_pfpart->name() << std::endl;

    larlite::AssSet_t ass_clus_hit_v;
    larlite::event_hit *ev_hit = nullptr;
    if (showers){
      ass_clus_hit_v = storage->find_one_ass( ev_clus->id(), ev_hit, ev_clus->name() );
      if (!ev_hit or (ev_hit->size() == 0)){
	std::cout << "No hits associated to PFPart! exit" << std::endl;
	showers = false;
      }
    }
    

    // find the track and vertex associated to the neutrino
    for (size_t i=0; i < ass_vtx_trk_v.size(); i++){

      if (ass_vtx_trk_v[i].size() == 0){
	std::cout << "vtx->trk association is empty..." << std::endl;
	continue;
      }
      
      std::cout << "trk " << i << " associated to vtx " << ass_vtx_trk_v[i][0] << std::endl;
      auto const& nutrk = ev_trk->at(i);
      auto const& nuvtx = ev_vtx->at( ass_vtx_trk_v[i][0] );

      Point2D point;
      double * xyz = new double[3];
      
      // add the track & vertex to the event
      for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
	Neutrino2D nu;

	// first the track
	nu._muon = getTrack2D(nutrk,view);

	// now the vertex
	nuvtx.XYZ(xyz);
	try {
	  point = geoHelper -> Point_3Dto2D(xyz, view);
	}
	catch (const std::exception& e) {
	  std::cerr << e.what() << '\n';
	}
	nu._vtx = point;

	if (pfpart){
	  // now try and find all other tracks/showers associated with the neutrino
	  // grab the PFParticle associated with this muon
	  std::cout << "PFParts associated with muon : " <<  ass_trk_pfpart_v[i].size() << std::endl;
	  auto pfpart_idx = ass_trk_pfpart_v[i][0];
	  auto muon = ev_pfpart->at(pfpart_idx);
	  if (muon.Parent() >= ev_pfpart->size()){
	    std::cout << "Muon parent not here..." << std::endl;
	    return false;
	  }
	  auto neutrino = ev_pfpart->at( muon.Parent() );
	  // grab neutrino children

	  std::cout << "number of daughters : " << neutrino.Daughters().size() << std::endl;
	  for (auto daughter_idx : neutrino.Daughters() ){
	    std::cout << "looking @ daughter..." << daughter_idx << std::endl;
	    // find the track associated with this
	    auto daughter = ev_pfpart->at(daughter_idx);
	    if (daughter.PdgCode() == 13){
	      auto trk_idx = ass_pfpart_trk_v[daughter_idx];
	      if (trk_idx.size() != 1){
		std::cout << "no associated track...skip" << std::endl;
		continue;
	      }
	      auto trk = ev_trk_2->at(trk_idx[0]);
	      std::cout << "Added daughter to neutrino tracks" << std::endl;
	      nu._trk_v.push_back( getTrack2D(trk,view) );
	    }// if tracks
	    // if showers
	    else if (showers){
	      auto clus_v_idx = ass_pfpart_clus_v[daughter_idx];
	      if (clus_v_idx.size() == 0){
		std::cout << "no associated cluster...skip" << std::endl;
		continue;
	      }
	      // loop through clusters, and find the one for this plane
	      for (auto const& clus_idx : clus_v_idx){
		// get hits associated with this cluster
		std::cout << "cluster index : " << clus_idx << std::endl;
		auto hit_idx_v = ass_clus_hit_v[clus_idx];
		std::cout << "There are " << hit_idx_v.size() << " hits associated" << std::endl;
		if (hit_idx_v.size() == 0)
		  continue;
		// first figure out the plane...
		auto plane = ev_hit->at( hit_idx_v[0] ).WireID().Plane;
		if (plane != view)
		  continue;
		// same plane -> add cluster to view information
		Cluster2D clus;
		for (auto const& idx : hit_idx_v){
		  auto hit = ev_hit->at(idx);
		  Hit2D hit2d(hit.WireID().Wire,
			      hit.PeakTime(),
			      hit.Integral(),
			      hit.RMS(),
			      hit.StartTick(),
			      hit.PeakTime(),
			      hit.EndTick(),
			      hit.PeakAmplitude());
		  clus.push_back(hit2d);
		}// for all hits
		std::cout << "adding cluster with " << clus.size() << " hits " << std::endl; 
		nu._clus_v.push_back(clus);
	      }
	    }// if showers
	  }
	}// if pfpart is available
	// save the neutrino information
	_dataByPlane.at(view).push_back( nu );
      }// for all 3 planes
      
      break;
      
    }
    
    // given the tagged muon track, find the PFParticle it is associated to.
    
      
  return true;
  }
  
  bool DrawNeutrino::finalize() {
    
    return true;
  }
  
  
} // larlite

#endif
