#ifndef SELECTIONTOOL_SPALGO_CLASS_NAME_CXX
#define SELECTIONTOOL_SPALGO_CLASS_NAME_CXX

#include "SPAlgo_Class_Name.h"

namespace sptool {

  SPAlgo_Class_Name::SPAlgo_Class_Name() : SPAlgoBase()
  {
    _name     = "SPAlgo_Class_Name";
  }

  void SPAlgo_Class_Name::Reset()
  {}

  SPArticleSet SPAlgo_Class_Name::Reconstruct(const SPAData &data)
  { return SPArticleSet(); }

}

#endif
