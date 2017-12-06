#ifndef RECOTOOL_CPALGOARRAY_CXX
#define RECOTOOL_CPALGOARRAY_CXX

#include "CPAlgoArray.h"

namespace cmtool {

  //-------------------------------------------------------
  CPAlgoArray::CPAlgoArray() : CPriorityAlgoBase()
  //-------------------------------------------------------
  {
    _mode = kPositiveAddition;
    _algo_array.clear();
  }

  //----------------------------------------------------------------------------------------------
  float CPAlgoArray::Priority(const cluster::cluster_params &cluster)
  //----------------------------------------------------------------------------------------------
  {

    std::vector<float> score_array;
    score_array.reserve(_algo_array.size());

    for(auto const& algo : _algo_array) {

      float score = algo->Priority(cluster);

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
