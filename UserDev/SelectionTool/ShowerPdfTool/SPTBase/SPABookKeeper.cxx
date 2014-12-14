#ifndef SELECTIONTOOL_SPABOOKKEEPER_CXX
#define SELECTIONTOOL_SPABOOKKEEPER_CXX

#include "SPABookKeeper.h"

namespace sptool {

  std::multimap<double,sptool::SPAComb_t> SPABookKeeper::unique_list() const
  {
    std::set<size_t> used;
    std::multimap<double,sptool::SPAComb_t> res;
    
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
