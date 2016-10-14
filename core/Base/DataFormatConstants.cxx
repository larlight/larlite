#include "DataFormatConstants.h"

/// Namespace of everything in this framework
namespace larlite{
  
  /// Defines constants for data structure definition (system utility)
  namespace data{
    
    const std::string& GET_DATA_TREE_NAME( larlite::data::DataType_t t ) { return kDATA_TREE_NAME[t]; };

  }
}
