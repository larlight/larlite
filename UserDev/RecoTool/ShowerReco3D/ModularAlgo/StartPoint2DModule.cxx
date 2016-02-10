#ifndef STARTPOINT2DMODULE_CXX
#define STARTPOINT2DMODULE_CXX

#include "StartPoint2DModule.h"

namespace showerreco {

void StartPoint2DModule::do_reconstruction(const ProtoShower & proto_shower,
    Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower

}


} //showerreco

#endif