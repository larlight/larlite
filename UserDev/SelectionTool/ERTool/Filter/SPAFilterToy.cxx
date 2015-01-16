#ifndef SELECTIONTOOL_SPAFILTERTOY_CXX
#define SELECTIONTOOL_SPAFILTERTOY_CXX

#include "SPAFilterToy.h"

namespace sptool {

  SPAFilterToy::SPAFilterToy() : SPAFilterBase()
  {
    _name = "SPAFilterToy";
  }

  SPAOrder SPAFilterToy::Select(const SPAData &data)
  { return SPAOrder(); }

}

#endif
