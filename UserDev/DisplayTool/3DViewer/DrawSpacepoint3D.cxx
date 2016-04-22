#ifndef DRAWSPACEPOINT3D_CXX
#define DRAWSPACEPOINT3D_CXX

#include "DrawSpacepoint3D.h"

namespace evd {


DrawSpacepoint3D::DrawSpacepoint3D() {
  _name = "DrawSpacepoint3D";
  _fout = 0;
}

bool DrawSpacepoint3D::initialize() {

  return true;
}

bool DrawSpacepoint3D::analyze(larlite::storage_manager* storage) {

  // get a handle to the tracks
  auto spacepointHandle = storage->get_data<larlite::event_spacepoint>(_producer);

  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(spacepointHandle -> size());


  // Populate the shower vector:
  for (auto & spt : *spacepointHandle) {
    _data.push_back(cluster3D::Point3D(spt.XYZ()[0],
                                       spt.XYZ()[1],
                                       spt.XYZ()[2]
                                      ));
  }


  return true;
}

bool DrawSpacepoint3D::finalize() {

  return true;
}

DrawSpacepoint3D::~DrawSpacepoint3D() {}


} // evd

#endif
