
#ifndef EVD_DRAWNEUTRINO_CXX
#define EVD_DRAWNEUTRINO_CXX

#include "DrawNeutrino.h"
#include "DataFormat/event_ass.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"

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

    std::cout << "Trying to find the neutrino!" << std::endl;
    
    // get a handle to the association
    auto ev_ass = storage->get_data<larlite::event_ass>(_producer);

    // get the association keys
    auto const& ass_keys = ev_ass->association_keys();

    std::cout << "Found " << ass_keys.size() << " ass keys" << std::endl;


    std::cout << "key 0 first  int : " << ass_keys[0].first.first << " string : "<< ass_keys[0].first.second << std::endl;
    std::cout << "key 0 second int : " << ass_keys[0].second.first << " string : "<< ass_keys[0].second.second << std::endl;    
    std::cout << "key 1 first  int : " << ass_keys[1].first.first << " string : "<< ass_keys[1].first.second << std::endl;
    std::cout << "key 1 second int : " << ass_keys[1].second.first << " string : "<< ass_keys[1].second.second << std::endl;

    larlite::AssSet_t ass_trk_vtx_v;
    larlite::event_track *ev_trk = nullptr;
    ass_trk_vtx_v = storage->find_one_ass( ass_keys[0].second, ev_trk, ev_ass->name() );

    larlite::AssSet_t ass_vtx_trk_v;
    larlite::event_track *ev_vtx = nullptr;
    ass_vtx_trk_v = storage->find_one_ass( ass_keys[0].second, ev_vtx, ev_ass->name() );

    // are there tracks? are there vertices?
    if (!ev_trk or (ev_trk->size() == 0)){
      std::cout << "No track! exit" << std::endl;
      return false;
    }
    if (!ev_vtx or (ev_vtx->size() == 0)){
      std::cout << "No vertex! exit" << std::endl;
      return false;
    }

    std::cout << "Got some data-products..." << std::endl;

    std::cout << "ev trk producer " << ev_trk->name() << std::endl;
    std::cout << "ev vtx producer " << ev_vtx->name() << std::endl;

    std::cout << "ass_trk_vtx_v size : " << ass_trk_vtx_v.size() << std::endl;
    std::cout << "ass_vtx_trk_v size : " << ass_vtx_trk_v.size() << std::endl;

    // find the track and vertex associated to the neutrino
    for (size_t i=0; i < ass_vtx_trk_v.size(); i++){
      if (ass_vtx_trk_v[i].size() > 0){
	std::cout << "trk " << i << " associated to vtx " << ass_vtx_trk_v[i][0] << std::endl;
	auto const& nutrk = ev_trk->at(i);
	auto const& nuvtx = ev_vtx->at( ass_vtx_trk_v[i][0] );

	for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
	  Neutrino2D nu;
	  nu._trk = getTrack2D(nutrk,view);
	  _dataByPlane.at(view).push_back( nu );
	}
	
	break;
      }
    }


      
  return true;
  }
  
  bool DrawNeutrino::finalize() {
    
    return true;
  }
  
  
} // larlite

#endif
