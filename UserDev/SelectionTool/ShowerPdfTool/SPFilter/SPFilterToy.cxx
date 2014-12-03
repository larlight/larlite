#ifndef SELECTIONTOOL_SPFILTERTOY_CXX
#define SELECTIONTOOL_SPFILTERTOY_CXX

#include "SPFilterToy.h"

namespace sptool {

  SPFilterToy::SPFilterToy() : SPFilterBase()
  {
    _name = "SPFilterToy";
  }

  bool SPFilterToy::Select(const SPAData &data)
  { return true; }

}

#endif
