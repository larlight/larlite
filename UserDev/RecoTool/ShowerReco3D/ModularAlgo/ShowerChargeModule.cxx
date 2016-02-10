#ifndef SHOWERCHARGEMODULE_CXX
#define SHOWERCHARGEMODULE_CXX

#include "ShowerChargeModule.h"

namespace showerreco {

void ShowerChargeModule::do_reconstruction(const ProtoShower & proto_shower,
    Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower

}


} //showerreco

#endif