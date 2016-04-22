#ifndef STARTPOINTPANDORAMODULE_CXX
#define STARTPOINTPANDORAMODULE_CXX

#include "StartPointPandora.h"

namespace showerreco {

void StartPointPandora::do_reconstruction(
  const ::protoshower::ProtoShower & proto_shower,
  Shower_t& resultShower) {


  // This is really simple.  Read in the 3D params and use that to
  // set the 3D start point of the shower from a vertex.
  // Right now, no checks against the
  // 2D projection but should be added somewhere.

  // Also, just taking the first vertex but could make a selection for
  // the best one.

  // If there is no 3D params, throw an exception.
  //
  if (proto_shower.hasVertex()) {
    resultShower.fXYZStart = proto_shower.vertexes().front();
  }
  else {
    throw ShowerRecoException("StartPointPandora requires vertexes but has none.");
  }
}


} //showerreco

#endif