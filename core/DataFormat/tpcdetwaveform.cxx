#ifndef LARLITE_TPCDETWAVEFORM_CXX
#define LARLITE_TPCDETWAVEFORM_CXX

#include "tpcdetwaveform.h"

namespace larlite {
  tpcdetwaveform::tpcdetwaveform()
    : data_base(data::kTPCDetWaveform)
  { clear_data(); }
}//namespace larlite

#endif
