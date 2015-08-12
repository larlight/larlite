#ifndef LARLITE_OPDETWAVEFORM_CXX
#define LARLITE_OPDETWAVEFORM_CXX

//#include "RecoBase/OpHit.h"
//#include "messagefacility/MessageLogger/MessageLogger.h"

#include "opdetwaveform.h"

namespace larlite {
  opdetwaveform::opdetwaveform()
    : data_base(data::kOpDetWaveform)
  {clear_data();}
}//namespace larlite
#endif
