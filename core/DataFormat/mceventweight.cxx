#ifndef LARLITE_MCEVENTWEIGHT_CXX
#define LARLITE_MCEVENTWEIGHT_CXX

#include "mceventweight.h"

namespace larlite {

  void mceventweight::clear_data(){
    _weights.clear();
  }

  mceventweight::mceventweight(const std::map<std::string, std::vector<double> > weights) {
    _weights = weights;
  }

  // Setter Function Definitions
  void mceventweight::SetWeights(const std::map<std::string, std::vector<double> > weights) {
    _weights = weights;
  }

  // Getter Function Definitions
  std::map<std::string, std::vector<double> > mceventweight::GetWeights() const {
    return _weights;
  }

}

#endif  // LARLITE_MCEVENTWEIGHT_CXX

