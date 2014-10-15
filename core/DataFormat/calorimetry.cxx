#ifndef CALORIMETRY_CXX
#define CALORIMETRY_CXX

#include "calorimetry.h"

namespace larlite {

  //*************************************
  void calorimetry::clear_data() 
  //*************************************
  {
    data_base::clear_data();
    fdEdx.clear();
    fdQdx.clear();
    fResidualRange.clear();
    fDeadWireResR.clear();
    fTrkPitch.clear();
    fRange = -1;
  }

}

#endif
