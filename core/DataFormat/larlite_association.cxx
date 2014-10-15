
#ifndef LARLITE_ASSOCIATION_CXX
#define LARLITE_ASSOCIATION_CXX

#include "larlite_association.h"

namespace larlite {

  /*
  void association::insert(std::string name, 
			   const size_t index_source,
			   const AssUnit_t& ass) {			   
    auto loc = _data.find(name);
    if(loc == _data.end()) {
      // New entry
      loc = _data.insert(std::make_pair(name,AssSet_t(index_source+1,AssUnit_t())));
      (*loc.first).second.at(index_source) = ass;

    } else {
      
      if( (*loc.second).size() <= index_source ) {
	// New entry
	(*loc.second).resize(index_source+1,AssUnit_t());
	(*loc.second)[index_source]=ass;
      }else if( (*loc.second)[index_source].size() ) {

	std::cerr << "\033[93m" 
		  << "Association: " << name.c_str() << " already has entry " << index_source
		  <<"\033[00m" << std::endl;
	throw std::exception();
	
      }else

	// New entry
	(*loc.second)[index_source] = ass;
    }

  }
  
  void association::insert(std::string name,
			   const AssSet_t& ass)
  {

    auto loc = _data.find(name);
    if(loc == _data.end())

      _data.insert(std::make_pair(name,ass));

    else {

	std::cerr << "\033[93m" 
		  << "Association: " << name.c_str() << " already exists!"
		  <<"\033[00m" << std::endl;
	throw std::exception();
	
    }

  }
  
  size_t association::size(std::string name) const
  {
    auto loc = _data.find(name);
    if(loc == _data.end()) return 0;
    else return (*loc->second).size();
  }

  size_t association::size(std::string name, size_t index_source) const
  {
    auto loc = _data.find(name);
    if(loc == _data.end()) return 0;
    else if( (*loc.second).size()<=index_source) return 0;
    else return (*loc->second)[index_source].size();
  }

  const AssSet_t& association::get(std::string name) const
  {
    auto loc = _data.find(name);
    if(loc == _data.end()) {
      
      std::cerr << "\033[93m"
		<< "Association: " << _name.c_str() << " => " << name.c_str() << " does not exist!"
		<< "\033[00m" << std::endl;
      throw std::exception();
    }
    return (*loc.second);
  }

  const AssUnit_t& association::get(std::string name, size_t index_source) const
  {
    auto loc = _data.find(name);
    if(loc == _data.end()) {
      
      std::cerr << "\033[93m"
		<< "Association: " << _name.c_str() << " => " << name.c_str() << " does not exist!"
		<< "\033[00m" << std::endl;
      throw std::exception();
    }
    if( (*loc.second).size() <= index_source ) {

      std::cerr << "\033[93m"
		<< "Association: " << _name.c_str() << "[" << index_source<<"] => " << name.c_str() << " does not exist!"
		<< "\033[00m" << std::endl;
      throw std::exception();
    }

    return (*loc.second);    
  }
  */
}
#endif
