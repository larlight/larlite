#ifndef LARLITE_CALORIMETRY_CXX
#define LARLITE_CALORIMETRY_CXX

#include "calorimetry.h"

namespace larlite {

  calorimetry::calorimetry() : data_base(data::kCalorimetry) 
  {
    clear_data();
  } 

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

  event_calorimetry::event_calorimetry(std::string name) : event_base(data::kCalorimetry,name)
  {
    clear_data();
  }

}

#endif
