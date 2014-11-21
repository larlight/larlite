#ifndef CCQESHOWERSEL_CXX
#define CCQESHOWERSEL_CXX

#include "CCQEShowerSel.h"
#include <sstream>

namespace larlite {

  bool CCQEShowerSel::initialize() {

    return true;
  }
  
  bool CCQEShowerSel::analyze(storage_manager* storage) {

    auto ev_calo = storage->get_data<event_calorimetry>(Form("calo%s",fTrackProducer.c_str()));
    
    auto ev_track = storage->get_data<event_track>(fTrackProducer);

    auto ev_shower = storage->get_data<event_shower>(fShowerProducer);

    auto ev_ctag = storage->get_data<event_cosmictag>(Form("%stag",fTrackProducer.c_str()));

    if(!ev_calo || !ev_track || !ev_shower || ev_ctag) {

      std::ostringstream msg;
      if(!ev_calo)   msg << "Calorimetry object not found!" << std::endl;
      if(!ev_track)  msg << "Track object not found!"       << std::endl;
      if(!ev_shower) msg << "Shower object not found!"      << std::endl;
      if(!ev_ctag)   msg << "CosmicTag object not found!"   << std::endl;
      throw DataFormatException(msg.str());

    }
    
    return true;
  }

  bool CCQEShowerSel::finalize() {

    return true;
  }

}
#endif
