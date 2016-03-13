#ifndef LARLITE_CHECKHITALIGNMENT_CXX
#define LARLITE_CHECKHITALIGNMENT_CXX

#include "CheckHitAlignment.h"

namespace larlite {

  bool CheckHitAlignment::initialize() {

    return true;
  }
  
  bool CheckHitAlignment::analyze(storage_manager* storage) {

    // clear maps
    _chan_to_hit_map.clear();
    _chan_to_ide_map.clear();

    // load hits
    auto ev_hits = storage->get_data<event_hit>(_hit_producer);

    // load simchannel info
    auto ev_simch = storage->get_data<event_simch>("largeant");

    // loop through hits and fill a map (channel number -> hit)
    for (size_t i=0; i < ev_hits->size(); i++){
      auto const& hit = ev_hits->at(i);
      // has this channel been added yet?
      if (_chan_to_hit_map.find( hit.Channel() ) == _chan_to_hit_map.end()){
	std::vector<larlite::hit> hit_v = {hit};
	_chan_to_hit_map[ hit.Channel() ] = hit_v;
      }
      else
	_chan_to_hit_map[ ev_hits->at(i).Channel() ].push_back( hit );
    }// for all hits

    // loop through simchannels and get energy deposited at various TDCs
    for (size_t i=0; i < ev_simch->size(); i++){
      auto const& simch = ev_simch->at(i);
      unsigned short ch = simch.Channel();
      // get map connecting TDC -> ide
      auto const TDC_IDE_map = simch.TDCIDEMap();
      // map connecting TDC -> charge
      std::map< unsigned short, double > tdcQ;
      typedef std::map<unsigned short, std::vector<larlite::ide> >::const_iterator ittype;
      for (ittype it = TDC_IDE_map.begin(); it != TDC_IDE_map.end(); it++){
	// get tick
	unsigned short tick = it->first;
	// get charge
	double Q = 0;
	auto const& ides = it->second;
	// loop throgh IDEs
	for (auto const& ide : ides)
	  Q += ide.numElectrons;
	tdcQ[ tick ] = Q;
      }// for all ides for this tick
      // add TDC -> Q map to a simchanel map
      _chan_to_ide_map [ ch ] = tdcQ;
    }// for all simchannels
      
  
    return true;
  }

  bool CheckHitAlignment::finalize() {

    return true;
  }

}
#endif
