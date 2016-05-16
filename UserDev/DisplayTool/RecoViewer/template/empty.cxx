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

  // Clear out the data but reserve some space
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(emptyHandle -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }


  // Populate the empty vector:
  for (auto & empty : *emptyHandle) {
    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      _dataByPlane.at(view).push_back(getEmpty2D(empty, view));
    }
  }


  return true;
}

bool DrawEmpty::finalize() {

  return true;
}

DrawEmpty::~DrawEmpty(){}

Empty2D DrawEmpty::getEmpty2D(larlite::empty empty, unsigned int plane) {
  Empty2D result;
  
  /*
  Fill in the product empty2D here
   */


  return result;
}


} // larlite

#endif
