#ifndef LARLITE_BASE_UTILFUNC_CXX
#define LARLITE_BASE_UTILFUNC_CXX

#include "UtilFunc.h"
#include "DataFormatConstants.h"

namespace larlite {
  const std::string& GetProductName(const larlite::data::DataType_t t)
  {
    return larlite::data::kDATA_TREE_NAME[t];
  }
}

#endif
