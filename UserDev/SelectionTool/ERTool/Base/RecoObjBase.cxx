#ifndef ERTOOL_RECOOBJBASE_CXX
#define ERTOOL_RECOOBJBASE_CXX

#include "RecoObjBase.h"

namespace ertool {

  void RecoObjBase::Reset() {
    _exclude    = false;
    _cosmogenic = kINVALID_DOUBLE;
  }

}

#endif
