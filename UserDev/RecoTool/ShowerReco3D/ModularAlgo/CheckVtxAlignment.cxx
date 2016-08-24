#ifndef CHECKVTXALIGNMENT_CXX
#define CHECKVTXALIGNMENT_CXX

#include "CheckVtxAlignment.h"

namespace showerreco {

void CheckVtxAlignment::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                    Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  // get the 3D direction and the start point
  // make sure they project back to the vertex, within some impact parameter
  // if not, throw away the shower

}


} //showerreco

#endif
