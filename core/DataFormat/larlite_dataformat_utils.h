/**
 * \file larlite_dataformat_utils.h
 *
 * \ingroup DataFormat
 * 
 * \brief DataFormat utility typedefs
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef LARLITE_DATAFORMAT_UTILS_H
#define LARLITE_DATAFORMAT_UTILS_H

#include <vector>
#include <map>

#include "Base/DataFormatConstants.h"

namespace larlite{

  /// Represents one => X (1 or many) association in terms of vector index number
  typedef std::vector<unsigned int> AssUnit_t;
  /// Represents a set of AssUnit_t ... association @ event_X data product level
  typedef std::vector<AssUnit_t > AssSet_t;
  /// Represents a (weak) product ID (type + producer's name)
  typedef std::pair<larlite::data::DataType_t,std::string> ProductID_t;
  /// Association data type meant to be stored within event_X data product
  typedef std::map<ProductID_t,AssSet_t> AssMap_t;

}
#endif
/** @} */ // end of doxygen group 
