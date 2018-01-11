#ifndef RECOTOOL_MATCHBOOKKEEPER_CXX
#define RECOTOOL_MATCHBOOKKEEPER_CXX

#include "CMatchBookKeeper.h"

namespace cmtool {

  CMatchBookKeeper::CMatchBookKeeper()
  { 
    Reset();
  }

  void CMatchBookKeeper::Reset()
  {
    _register.clear();
  }

  void CMatchBookKeeper::Match(const std::vector<unsigned int>& matched_indexes,
			      const float& score)
  {
    _register.insert(std::make_pair(score,matched_indexes));
  }

  
  std::vector<std::vector<unsigned int> > CMatchBookKeeper::GetResult() const
  {
    std::vector<std::vector<unsigned int> > res;

    PassResult(res);

    return res;

  }
  
  /// Method to pass result
  void CMatchBookKeeper::PassResult(std::vector<std::vector<unsigned int> >& result) const
  {
    result.clear();

    // Rough guess: assume half of registered pairs are good
    result.reserve((unsigned int)(_register.size()/2));

    std::vector<bool> used_index;
    
    for( auto riter = _register.rbegin();
	 riter != _register.rend();
	 ++riter) {

      bool valid_set = true;

      for(auto const& index : (*riter).second) {

	if(index >= used_index.size())

	  used_index.resize(index+1,false);

	else if(used_index.at(index)) valid_set = false;

      }

      if(valid_set) {

	result.push_back((*riter).second);

	for(auto& index : (*riter).second)

	  used_index.at(index) = true;

      }

    }
    
  }
  
}


#endif
