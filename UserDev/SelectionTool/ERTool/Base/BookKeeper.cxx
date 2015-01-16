#ifndef ERTOOL_BOOKKEEPER_CXX
#define ERTOOL_BOOKKEEPER_CXX

#include "BookKeeper.h"

namespace ertool {

  std::multimap<double,ertool::Combination_t> BookKeeper::unique_list() const
  {
    std::set<size_t> used;
    std::multimap<double,ertool::Combination_t> res;
    
    for(auto iter = _list.rbegin();
	iter != _list.rend();
	++iter) {

      bool next=false;
      for(auto const& id : (*iter).second) {
	
	if(used.find(id) != used.end()) {
	  next = true;
	  break;
	}
	used.insert(id);
      }
      if(next) continue;
      res.insert(std::make_pair((*iter).first,(*iter).second));
    }
    return res;
  }

}

#endif
