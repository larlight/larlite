#include "larlite_dataformat_utils.h"

namespace larlite {
  
  product_id::product_id() : std::pair<unsigned short,std::string>(larlite::data::kDATA_TYPE_MAX,"noname") {};
  
}
