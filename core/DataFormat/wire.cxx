#ifndef LARLITE_WIRE_CXX
#define LARLITE_WIRE_CXX

#include "wire.h"

namespace larlite {
  
  //*********************
  void wire::clear_data()
  //*********************
  {
    fSignal.clear();
    fView       = geo::kUnknown;
    fSignalType = geo::kMysteryType;
    fChannel    = 0xffffffff;
  }

}
#endif
