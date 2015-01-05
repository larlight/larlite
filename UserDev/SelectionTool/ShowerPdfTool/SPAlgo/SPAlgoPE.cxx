#ifndef SELECTIONTOOL_SPALGOPE_CXX
#define SELECTIONTOOL_SPALGOPE_CXX

#include "SPAlgoPE.h"

namespace sptool {

  SPAlgoPE::SPAlgoPE() : SPAlgoBase()
  {
    _name     = "SPAlgoPE";
  }

  void SPAlgoPE::Reset()
  {}

  SPArticleSet SPAlgoPE::Reconstruct(const SPAData &data)
  { 
    
    return SPArticleSet(); 
  }

}

#endif
