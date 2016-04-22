#ifndef DRAWSIMCH_CXX
#define DRAWSIMCH_CXX

#include "DrawSimChannel3D.h"

namespace evd {


DrawSimChannel3D::DrawSimChannel3D() {
  _name = "DrawSimChannel3D";
  _fout = 0;
}

bool DrawSimChannel3D::initialize() {

  return true;
}

bool DrawSimChannel3D::analyze(larlite::storage_manager* storage) {

  // get a handle to the tracks
  auto simchHandle = storage->get_data<larlite::event_simch>(_producer);

  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(simchHandle -> size());


  // std::cout << "will load simch " << std::endl;

  // Populate the shower vector:
  for (auto & simch : *simchHandle) {
    // get all the IDEs associated with this simch
    auto const& all_ide = simch.TrackIDsAndEnergies(0,19600);
    for (auto const& ide : all_ide){
      // std::cout << "3dpoint : [" << ide.x << ", " << ide.y << ", " << ide.z << "]" << std::endl;
      _data.push_back( SimChannel3D(ide.x, ide.y, ide.z) );
    }
  }

  return true;
}

bool DrawSimChannel3D::finalize() {

  return true;
}

DrawSimChannel3D::~DrawSimChannel3D() {}


} // evd

#endif
