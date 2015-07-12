#ifndef LARLITE_EVENT_ASS_CXX
#define LARLITE_EVENT_ASS_CXX

#include "event_ass.h"
#include <sstream>
#include <set>
namespace larlite{
  
  event_ass::event_ass(const std::string name)
    : event_base(data::kAssociation,name)
  { clear_data(); }

  void event_ass::clear_data()
  {
    event_base::clear_data();
    //for(auto& ass : _ass_data) ass.clear();
    _ass_map_key.clear();
    _ass_data.clear();
    _ass_ids.clear();
  }

  size_t event_ass::size() const
  { return _ass_data.size(); }

  void event_ass::set_association(const product_id& id_a,
				  const product_id& id_b,
				  const AssSet_t& ass_a2b,
				  const bool overwrite)
  {
    auto& map_key_a = _ass_map_key[id_a];
    auto iter_a = map_key_a.find(id_b);
    if(iter_a == map_key_a.end()) {
      iter_a = map_key_a.insert(std::make_pair(id_b,_ass_data.size())).first;
      _ass_data.push_back(ass_a2b);
      _ass_ids.push_back(std::make_pair(id_a,id_b));
      /*
      std::cout<<"Creating a new association by "<<this->name().c_str()
	       <<" ID="
	       << (*iter_a).second
	       <<" ... ("
	       <<data::kDATA_TREE_NAME[id_a.first].c_str()<<","<<id_a.second.c_str()<<")"
	       <<" => ("
	       <<data::kDATA_TREE_NAME[id_b.first].c_str()<<","<<id_b.second.c_str()<<")"
	       <<" ... current size: " << _ass_data.size()<<std::endl;
      */
    }else if(_ass_data.at((*iter_a).second).size()){
      if(!overwrite)
	throw DataFormatException("Overwriting the association not allowed!");
      
      std::cerr<<"\033[93m[WARNING]\033[00m Overwriting the association ... "
	       << "Type: " << id_a.first << " by " << id_a.second.c_str()
	       << " => "
	       << "Type: " << id_b.first << " by " << id_b.second.c_str()
	       << std::endl;
      _ass_data.at((*iter_a).second) = ass_a2b;
    }else{
      /*
      std::cout<<"Refilling an association by "<<this->name().c_str()
	       <<" ID="
	       << (*iter_a).second
	       <<" ... ("
	       <<data::kDATA_TREE_NAME[id_a.first].c_str()<<","<<id_a.second.c_str()<<")"
	       <<" => ("
	       <<data::kDATA_TREE_NAME[id_b.first].c_str()<<","<<id_b.second.c_str()<<")"
	       <<" ... current size: " << _ass_data.size()<<std::endl;
      */
      _ass_data.at((*iter_a).second) = ass_a2b;
    }

    // Do a bi-directional insert
    auto& map_key_b = _ass_map_key[id_b];
    auto iter_b = map_key_b.find(id_a);
    if(iter_b == map_key_b.end()) {
      iter_b = map_key_b.insert(std::make_pair(id_a,_ass_data.size())).first;
      _ass_data.push_back(larlite::AssSet_t());
      _ass_ids.push_back(std::make_pair(id_b,id_a));
      /*
      std::cout<<"Creating a reverse association by "<<this->name().c_str()
	       <<" ID="
	       << (*iter_b).second
	       <<" ... ("
	       <<data::kDATA_TREE_NAME[id_a.first].c_str()<<","<<id_a.second.c_str()<<")"
	       <<" => ("
	       <<data::kDATA_TREE_NAME[id_b.first].c_str()<<","<<id_b.second.c_str()<<")"
	       <<" ... current size: " << _ass_data.size()<<std::endl;
      */
    }
    /*
    else{
      std::cout<<"Refilling a reverse association by "<<this->name().c_str()
	       <<" ID="
	       << (*iter_b).second
	       <<" ... ("
	       <<data::kDATA_TREE_NAME[id_a.first].c_str()<<","<<id_a.second.c_str()<<")"
	       <<" => ("
	       <<data::kDATA_TREE_NAME[id_b.first].c_str()<<","<<id_b.second.c_str()<<")"
	       <<" ... current size: " << _ass_data.size()<<std::endl;
    }
    */

    auto& ass_b = _ass_data[(*iter_b).second];
    
    int max_val = -1;
    for(auto const& ass_unit : ass_a2b) {
      for(auto const& ass : ass_unit)
	if((int)ass > max_val) max_val = ass;
    }
    
    if(max_val >= 0) {
      ass_b.resize(max_val+1);
      for(size_t a_index=0; a_index<ass_a2b.size(); ++a_index) {
	for(auto const& b_index : ass_a2b[a_index])
	  ass_b[b_index].push_back(a_index);
      }
    }

  }

  const AssSet_t& event_ass::association(const AssID_t id) const 
  { 
    if(id>=_ass_data.size()) {
      std::ostringstream msg;
      msg << "Invalid association ID: " << id;
      throw DataFormatException(msg.str());
    }
    return _ass_data[id];
  }

  const AssSet_t& event_ass::association(const product_id& id_a,
					 const product_id& id_b) const
  {
    auto id = assid(id_a,id_b);
    if(id == kINVALID_ASS) {
      std::ostringstream msg;
      msg << "Associtaion (" << id_a.first << "," << id_a.second.c_str()
	  << " => "
	  << "(" << id_b.first << "," << id_b.second.c_str()
	  << " not found...";
      throw DataFormatException(msg.str());
    }
    return association(id);
  }

  const std::vector<std::pair<larlite::product_id,larlite::product_id> > event_ass::association_keys() const
  {
    std::vector<std::pair<larlite::product_id,larlite::product_id> > result;
    for(auto const& id_a_pair : _ass_map_key) {
      auto const& id_a = id_a_pair.first;
      result.reserve(result.size() + id_a_pair.second.size());
      for(auto const& id_b_pair : id_a_pair.second)
	result.emplace_back(id_a,id_b_pair.first);
    }
    return result;
  }
  
  const std::vector<larlite::product_id> event_ass::association_keys(const larlite::product_id& id) const
  {
    std::vector<larlite::product_id> result;
    auto iter = _ass_map_key.find(id);
    if(iter == _ass_map_key.end()) return result;
    for(auto const& id_b_pair : (*iter).second)
      result.push_back(id_b_pair.first);
    return result;
  }

  const std::pair<larlite::product_id,larlite::product_id>& event_ass::association_keys(const AssID_t id) const
  {
    if(id >= _ass_ids.size()) {
      std::ostringstream msg;
      msg << "Invalid association ID: " << id;
      throw DataFormatException(msg.str());
    }
    return _ass_ids[id];
  }

  void event_ass::list_association() const
  {
    std::cout << "  Listing associations stored..." << std::endl;
    auto const& ass_keys = association_keys();
    for(auto const& keys : ass_keys) {
      
      auto const& index = assid(keys.first,keys.second);
      
      size_t a_ctr = 0;
      std::set<size_t> unique_partner;
      for(auto const& ass : _ass_data[index]){
	if(!ass.size()) continue;
	a_ctr += 1;
	for(auto const& partner_id : ass)
	  unique_partner.insert((size_t)partner_id);
      }      
      std::cout << "    Association ID: " << index << "/" << _ass_data.size() << " ... ("
		<< data::kDATA_TREE_NAME[keys.first.first].c_str() << "," << keys.first.second.c_str()
		<< ") => ("
		<< data::kDATA_TREE_NAME[keys.second.first].c_str() << "," << keys.second.second.c_str()
		<< ") ... "
		<< a_ctr
		<< " objects associated with "
		<< unique_partner.size()
		<< " objects."
		<< std::endl;
    }
    std::cout << "  ... done!" << std::endl;
  }

  AssID_t event_ass::assid(const product_id& id_a,
			   const product_id& id_b) const 
  {
    AssID_t id = kINVALID_ASS;
    auto iter_a = _ass_map_key.find(id_a);
    if(iter_a==_ass_map_key.end()) return id;
    auto iter_b = (*iter_a).second.find(id_b);
    if(iter_b == (*iter_a).second.end()) return id;
    id = (*iter_b).second;
    return id;
  }
  
  AssID_t event_ass::find_one_assid(const data::DataType_t type_a,
				    const data::DataType_t type_b) const
  {
    AssID_t id = kINVALID_ASS;
    for(auto const& first_pair : _ass_map_key) {
      if(first_pair.first.first != type_a) continue;
      for(auto const& second_pair : first_pair.second) {
	if(second_pair.first.first != type_b) continue;
	id = second_pair.second;
      }
      if(id != kINVALID_ASS) break;
    }
    return id;
  }
  
  AssID_t event_ass::find_one_assid(const product_id& id_a,
				    const data::DataType_t type_b) const
  {
    AssID_t id = kINVALID_ASS;
    auto iter = _ass_map_key.find(id_a);
    if(iter == _ass_map_key.end()) return id;
    for(auto const& second_pair : (*iter).second) {
      if(second_pair.first.first != type_b) continue;
      id = second_pair.second;
      break;
    }
    return id;
  }
  
  AssID_t event_ass::find_one_assid(const data::DataType_t type_a,
				    const product_id& id_b) const
  {
    AssID_t id = kINVALID_ASS;
    auto iter = _ass_map_key.find(id_b);
    if(iter == _ass_map_key.end()) return id;
    for(auto const& second_pair : (*iter).second) {
      if(second_pair.first.first != type_a) continue;
      id = second_pair.second;
      break;
    }
    return id;
  }
  
  AssID_t event_ass::find_unique_assid(const data::DataType_t type_a,
				       const data::DataType_t type_b) const
  {
    AssID_t id = kINVALID_ASS;
    for(auto const& first_pair : _ass_map_key) {
      if(first_pair.first.first != type_a) continue;
      for(auto const& second_pair : first_pair.second) {
	if(second_pair.first.first != type_b) continue;
	if(id != kINVALID_ASS) {
	  std::ostringstream msg;
	  msg << "Association type: "
	      << data::kDATA_TREE_NAME[type_a].c_str() << " (" << type_a <<")"
	      << " => "
	      << data::kDATA_TREE_NAME[type_b].c_str() << " (" << type_b <<")"
	      << " is not unique!";
	  throw DataFormatException(msg.str());
	}
	id = second_pair.second;
      }
    }
    return id;
  }
  
  AssID_t event_ass::find_unique_assid(const product_id& id_a,
				       const data::DataType_t type_b) const
  {
    AssID_t id = kINVALID_ASS;
    auto iter = _ass_map_key.find(id_a);
    if(iter == _ass_map_key.end()) return id;
    for(auto const& second_pair : (*iter).second) {
      if(second_pair.first.first != type_b) continue;
      if(id != kINVALID_ASS) {
	std::ostringstream msg;
	msg << "Association type: "
	    << data::kDATA_TREE_NAME[id_a.first].c_str() << " (" << id_a.first <<")"
	    << " by " << id_a.second.c_str()
	    << " => "
	    << data::kDATA_TREE_NAME[type_b].c_str() << " (" << type_b <<")"
	    << " is not unique!";
	throw DataFormatException(msg.str());
      }
      id = second_pair.second;
    }
    return id;
  }
  
  AssID_t event_ass::find_unique_assid(const data::DataType_t type_a,
				       const product_id& id_b) const
  {
    AssID_t id = kINVALID_ASS;
    auto iter = _ass_map_key.find(id_b);
    if(iter == _ass_map_key.end()) return id;
    for(auto const& second_pair : (*iter).second) {
      if(second_pair.first.first != type_a) continue;
      if(id != kINVALID_ASS) {
	std::ostringstream msg;
	msg << "Association type: "
	    << data::kDATA_TREE_NAME[type_a].c_str() << " (" << type_a <<")"
	    << " => "
	    << data::kDATA_TREE_NAME[id_b.first].c_str() << " (" << id_b.first <<")"
	    << " by " << id_b.second.c_str()
	    << " is not unique!";
	throw DataFormatException(msg.str());
      }
      id = second_pair.second;
    }
    return id;
  }

  std::vector<larlite::AssID_t>
  event_ass::find_all_assid(const data::DataType_t type_a,
			    const data::DataType_t type_b) const
  {
    std::vector<larlite::AssID_t> id_v;
    for(auto const& first_pair : _ass_map_key) {
      if(first_pair.first.first != type_a) continue;
      for(auto const& second_pair : first_pair.second) {
	if(second_pair.first.first != type_b) continue;
	id_v.push_back(second_pair.second);
      }
    }
    return id_v;
  }
  
  std::vector<larlite::AssID_t>
  event_ass::find_all_assid(const product_id& id_a,
			    const data::DataType_t type_b) const
  {
    std::vector<larlite::AssID_t> id_v;
    auto iter = _ass_map_key.find(id_a);
    if(iter == _ass_map_key.end()) return id_v;
    for(auto const& second_pair : (*iter).second) {
      if(second_pair.first.first != type_b) continue;
      id_v.push_back(second_pair.second);
    }
    return id_v;
  }
  
  std::vector<larlite::AssID_t>
  event_ass::find_all_assid(const data::DataType_t type_a,
			    const product_id& id_b) const
  {
    std::vector<larlite::AssID_t> id_v;
    auto iter = _ass_map_key.find(id_b);
    if(iter == _ass_map_key.end()) return id_v;
    for(auto const& second_pair : (*iter).second) {
      if(second_pair.first.first != type_a) continue;
      id_v.push_back(second_pair.second);
    }
    return id_v;
  }

}
#endif

