#ifndef LARLITE_BASE_UTILFUNC_H
#define LARLITE_BASE_UTILFUNC_H

namespace larlite {

  const std::string& GetProductName(const larlite::data::DataType_t t)
  {
    return larlite::data::kDATA_TREE_NAME[t];
  }

}

#endif
