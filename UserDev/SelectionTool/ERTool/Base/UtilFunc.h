#ifndef ERTOOL_UTILFUNC_H
#define ERTOOL_UTILFUNC_H

#include <vector>
#include "ERToolTypes.h"

namespace ertool {

  /// Utility to make a combination of m-elements from the full set of n-elements
  std::vector<ertool::Combination_t> Combination(const size_t n, 
						 const size_t m);

}
#endif
