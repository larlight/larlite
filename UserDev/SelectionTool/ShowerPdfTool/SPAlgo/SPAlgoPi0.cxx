#ifndef SELECTIONTOOL_SPALGOPI0_CXX
#define SELECTIONTOOL_SPALGOPI0_CXX

#include "SPAlgoPi0.h"

namespace sptool {

  SPAlgoPi0::SPAlgoPi0() : SPAlgoBase()
  {
    _name     = "SPAlgoPi0";
  }

  SPArticleSet SPAlgoPi0::Select(const SPAData &data)
  { return SPArticleSet(); }

  void SPAlgoPi0::Fill(const SPAData &data)
  {}

}

#endif
