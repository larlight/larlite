#ifndef ERTOOL_ENV_H
#define ERTOOL_ENV_H

#include <string>
#include <climits>
#include <limits>
#include <vector>
namespace ertool {

  /// Defines a type for a combinatory index expression
  typedef std::vector<size_t> Combination_t;

  /// Default parameter set storage file name
  const std::string kDataFileName = "larlite_spt_data.root";

  /// Utility: maximum value for double 
  const double kDOUBLE_MAX = std::numeric_limits<double>::max();

  /// Utility: minimum value for double
  const double kDOUBLE_MIN = std::numeric_limits<double>::min();

  /// Utility: invalid double value
  const double kINVALID_DOUBLE = kDOUBLE_MAX;

  /// Utility: invalid int value
  const double kINVALID_INT = std::numeric_limits<int>::max();

  /// Utility: invalid size_t value
  const double kINVALID_SIZE = std::numeric_limits<size_t>::max();

} 

#endif
