#ifndef ERTOOL_UTILFUNC_H
#define ERTOOL_UTILFUNC_H

#include <vector>
#include "ERToolTypes.h"

namespace ertool {

  /// Utility to get a particle mass.
  double ParticleMass(const int pdgcode);
  
  /// Utility to make a combination of m-elements from the full set of n-elements
  std::vector<ertool::Combination_t> Combination(const size_t n, 
						 const size_t m);

  namespace msg {

    /// ertool::msg::Message::send function direct call
    void send(Level_t level, const std::string& msg);
    
    /// ertool::msg::Message::send function direct call
    void send(Level_t level, const std::string& where, const std::string& msg);
    
  }
}
#endif
