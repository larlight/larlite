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

  void event_base::list_association() const
  {
    std::cout << "  Listing associations stored..." << std::endl;
    for(auto const& id_ass : fAssociation) {

      size_t ass_count=0;

      for(auto const& ass_set : id_ass.second)

	ass_count = ass_set.size();

      std::cout << "    Type: " << data::kDATA_TREE_NAME[id_ass.first.first].c_str()
		<< " (enum = "  << id_ass.first.first << ") "
		<< " produced by \"" << id_ass.first.second.c_str() << "\""
		<< " ... length is "  << id_ass.second.size()
		<< " => " << ass_count << std::endl;

    }
    std::cout << "  ... done!" << std::endl;
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
	throw DataFormatException();
	
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
	throw DataFormatException();
	
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
      throw DataFormatException();
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
      throw DataFormatException();
    }
    if( (*loc).second.size() <= index_source ) {

      std::cerr << "\033[93m"
		<< " Association to " << data::kDATA_TREE_NAME[type.first].c_str() 
		<< " by " << type.second.c_str()
		<< " does not exist for element [" << index_source<<"]"
		<< "\033[00m" << std::endl;
      throw DataFormatException();
    }

    return (*loc).second[index_source];
  }

  const std::vector< ::larlite::product_id> event_base::association_keys() const
  {
    std::vector< ::larlite::product_id> results;
    results.reserve(fAssociation.size());

    for(auto const& key_value : fAssociation)

      results.push_back(key_value.first);

    return results;
  }

  const std::vector<std::string> event_base::association_keys(const data::DataType_t type) const
  {
    std::vector<std::string> results;

    for(auto const& key_value : fAssociation) 

      if(key_value.first.first == type) results.push_back(key_value.first.second);

    return results;

  }

  const std::vector<std::string> event_base::association_keys(const data::DataType_t type,
							      const size_t index_source) const
  {
    std::vector<std::string> results;
    
    for(auto const& key_value : fAssociation) {

      if(key_value.first.first != type) continue;

      if(key_value.second.size() <= index_source) continue;

      if(key_value.second.at(index_source).size())

	results.push_back(key_value.first.second);
    }

    return results;
  }



}

#endif
