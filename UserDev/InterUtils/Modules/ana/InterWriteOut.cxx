#ifndef LARLITE_INTERWRITEOUT_CXX
#define LARLITE_INTERWRITEOUT_CXX

#include "InterWriteOut.h"
#include "DataFormat/vertex.h"
#include "DataFormat/track.h"
#include "DataFormat/shower.h"

namespace larlite {

  InterWriteOut::InterWriteOut() {
    _name = "InterWriteOut";
  }
  
  bool InterWriteOut::analyze(storage_manager* storage) {

    larlite::event_vertex* ev_inter_vertex = nullptr;
    ev_inter_vertex = (larlite::event_vertex*)storage->get_data(larlite::data::kVertex,"inter_vertex");
    (void)ev_inter_vertex;

    larlite::event_shower* ev_inter_shower = nullptr;
    ev_inter_shower = (larlite::event_shower*)storage->get_data(larlite::data::kShower,"inter_shower");
    (void)ev_inter_shower;

    larlite::event_track* ev_inter_track = nullptr;
    ev_inter_track = (larlite::event_track*)storage->get_data(larlite::data::kTrack,"inter_track");
    (void)ev_inter_track;

    larlite::event_ass* ev_inter_ass = nullptr;
    ev_inter_ass = (larlite::event_ass*) storage->get_data(larlite::data::kAssociation,"inter_ass");
    (void)ev_inter_ass;

    return true;
  }


}
#endif
