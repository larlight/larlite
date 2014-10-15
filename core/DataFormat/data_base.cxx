#ifndef DATA_BASE_CXX
#define DATA_BASE_CXX

#include "data_base.h"

namespace larlite {

  //**************************
  void event_base::clear_data()
  //**************************
  {
    data_base::clear_data();
    fRunNumber = fSubRunNumber = fEventID = data::kINVALID_UINT;
  }

}

#endif
