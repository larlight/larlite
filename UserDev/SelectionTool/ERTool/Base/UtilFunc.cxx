#ifndef ERTOOL_UTILFUNC_CXX
#define ERTOOL_UTILFUNC_CXX

#include "UtilFunc.h"

namespace ertool {

  std::vector<Combination_t> Combination(const size_t n, 
					 const size_t m)
  {
    
    if(n<m){
      std::ostringstream msg;
      msg << "<<" << __FUNCTION__ << ">>"
	  << " Cannot make a combination of " << m 
	  << " elements from the pool of " << n << std::endl;
      throw ERException(msg.str());      
    }

    std::vector<Combination_t> result;

    std::vector<bool> comb_index_v(n);
    std::fill(comb_index_v.begin() + m, comb_index_v.end(), true);

    do {

      Combination_t index_v;
      index_v.reserve(m);

      for (size_t i = 0; i < comb_index_v.size(); ++i)

	if (!comb_index_v[i]) index_v.push_back(i);
      
      result.push_back(index_v);

    } while (std::next_permutation(comb_index_v.begin(), comb_index_v.end()));

    return result;
  }
  
}

#endif
