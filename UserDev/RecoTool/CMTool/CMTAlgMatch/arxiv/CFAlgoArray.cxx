#ifndef RECOTOOL_CFALGOARRAY_CXX
#define RECOTOOL_CFALGOARRAY_CXX

#include "CFAlgoArray.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoArray::CFAlgoArray() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _mode = kPositiveAddition;
    _algo_array.clear();
  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoArray::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

    std::vector<float> score_array;
    score_array.reserve(_algo_array.size());

    for(auto const& algo : _algo_array) {

      float score = algo->Float(clusters);

      if(_mode != kSimpleAddition && score < 0)
        return score;

      score_array.push_back(score);
      
    }

    float score_result = 0;

    switch(_mode) {
      
    case kLastAlgo:
      
      score_result = (*score_array.rbegin());
      break;

    case kSimpleAddition:
    case kPositiveAddition:

      for(auto const& score : score_array) score_result += score;
      break;

    case kMultiplication:

      for(auto const& score : score_array) score_result *= score;
      break;

    }
    
    return score_result;
  }

}
#endif
