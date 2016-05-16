#ifndef DRAWEMPTY_CXX
#define DRAWEMPTY_CXX

#include "DrawEmpty.h"
#include "DataFormat/empty.h"

namespace evd {


DrawEmpty::DrawEmpty() {
  _name = "DrawEmpty";
  _fout = 0;
}

bool DrawEmpty::initialize() {

  // Resize data holder
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;
}

bool DrawEmpty::analyze(larlite::storage_manager* storage) {



  // get a handle to the tracks
  auto emptyHandle = storage->get_data<larlite::event_empty>(_producer);

  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(trackHandle -> size());
  }


  // Populate the track vector:
  for (auto & track : *trackHandle) {
    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      _dataByPlane.at(view).push_back(getEmpty2D(track, view));
    }
  }


  return true;
}

bool DrawEmpty::finalize() {

  return true;
}

DrawEmpty::~DrawEmpty(){}

Empty2D DrawEmpty::getEmpty2D(larlite::track track, unsigned int plane) {
  Empty2D result;
  
  /*
  Fill in the product empty2D here
   */


  return result;
}


} // larlite

#endif
