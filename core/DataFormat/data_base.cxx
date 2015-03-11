#ifndef LARLITE_DATA_BASE_CXX
#define LARLITE_DATA_BASE_CXX

#include "data_base.h"

namespace larlite {

  //**************************
  void event_base::clear_data()
  //**************************
  {
    data_base::clear_data();
    fRunNumber = kINVALID_RUN;
    fSubRunNumber = kINVALID_SUBRUN;
    fEventID = kINVALID_EVENT;
  }

}

#endif
