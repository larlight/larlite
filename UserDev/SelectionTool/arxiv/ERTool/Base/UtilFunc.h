#ifndef ERTOOL_UTILFUNC_H
#define ERTOOL_UTILFUNC_H

#include <vector>
#include "ERToolTypes.h"
#include "ERToolConstants.h"

namespace ertool {

  /// Utility to get a default configuration file
  const std::string GetDefaultConfigFileName();
  
  /// Utility to get a particle mass.
  double ParticleMass(const int pdgcode);
  
  /// Utility to make a combination of m-elements from the full set of n-elements
  std::vector<ertool::Combination_t> Combination(const size_t n, 
						 const size_t m);

}
#endif
