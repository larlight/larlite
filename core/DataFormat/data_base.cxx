#ifndef LARLITE_DATA_BASE_CXX
#define LARLITE_DATA_BASE_CXX

#include "data_base.h"

namespace larlite {

  //**************************
  void event_base::clear_data()
  //**************************
  {
    data_base::clear_data();
    fRunNumber = fSubRunNumber = fEventID = data::kINVALID_UINT;
    fAssociation.clear();
  }

  void event_base::set_association(const product_id type,
				   const size_t index_source,
				   const AssUnit_t& ass) {			   
    auto loc = fAssociation.find(type);
    if(loc == fAssociation.end()) {
      // New entry
      auto new_loc = fAssociation.insert(std::make_pair(type,AssSet_t(index_source+1,AssUnit_t())));
      (*new_loc.first).second.at(index_source) = ass;
    } else {
      
      if( (*loc).second.size() <= index_source ) {
	// New entry
	(*loc).second.resize(index_source+1,AssUnit_t());
	(*loc).second[index_source]=ass;
      }else if( (*loc).second[index_source].size() ) {

	std::cerr << "\033[93m" 
		  << " Association to " << data::kDATA_TREE_NAME[type.first].c_str() 
		  << " by " << type.second.c_str() 
		  << " already has entry " << index_source
		  <<"\033[00m" << std::endl;
	throw std::exception();
	
      }else

	// New entry
	(*loc).second[index_source] = ass;
    }

  }
  
  void event_base::set_association(const product_id type,
				   const AssSet_t& ass)
  {

    auto loc = fAssociation.find(type);
    if(loc == fAssociation.end())

      fAssociation.insert(std::make_pair(type,ass));

    else {

	std::cerr << "\033[93m" 
		  << " Association to " << data::kDATA_TREE_NAME[type.first].c_str()
		  << " by " << type.second.c_str() 
		  << " already exists!"
		  <<"\033[00m" << std::endl;
	throw std::exception();
	
    }

  }
  
  size_t event_base::size_association(const product_id type) const
  {
    auto loc = fAssociation.find(type);
    if(loc == fAssociation.end()) return 0;
    else return (*loc).second.size();
  }

  size_t event_base::size_association(const product_id type, const size_t index_source) const
  {
    auto loc = fAssociation.find(type);
    if(loc == fAssociation.end()) return 0;
    else if( (*loc).second.size()<=index_source) return 0;
    else return (*loc).second[index_source].size();
  }

  const AssSet_t& event_base::association(const product_id type) const
  {
    auto loc = fAssociation.find(type);
    if(loc == fAssociation.end()) {
      
      std::cerr << "\033[93m"
		<< " Association to " << data::kDATA_TREE_NAME[type.first].c_str() 
		<< " by " << type.second.c_str()
		<< " does not exist!"
		<< "\033[00m" << std::endl;
      throw std::exception();
    }
    return (*loc).second;
  }

  const AssUnit_t& event_base::association(const product_id type, const size_t index_source) const
  {
    auto loc = fAssociation.find(type);
    if(loc == fAssociation.end()) {
      
      std::cerr << "\033[93m"
		<< " Association to " << data::kDATA_TREE_NAME[type.first].c_str()
		<< " by " << type.second.c_str() 
		<< " does not exist!"
		<< "\033[00m" << std::endl;
      throw std::exception();
    }
    if( (*loc).second.size() <= index_source ) {

      std::cerr << "\033[93m"
		<< " Association to " << data::kDATA_TREE_NAME[type.first].c_str() 
		<< " by " << type.second.c_str()
		<< " does not exist for element [" << index_source<<"]"
		<< "\033[00m" << std::endl;
      throw std::exception();
    }

    return (*loc).second[index_source];
  }


}

#endif
