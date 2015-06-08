#ifndef BASICTOOL_FHICLLITE_PSET_CXX
#define BASICTOOL_FHICLLITE_PSET_CXX

#include "PSet.h"

namespace fclite {

  PSet::PSet(const std::string name,
	     const std::string data)
  {
    if(name.empty()) throw FhiclLiteException("Cannot make PSet with an empty name!");
    _name = name;
    if(!data.empty()) this->add(data);
  }

  const std::vector<std::string> PSet::value_keys () const
  {
    std::vector<std::string> res;
    res.reserve(_data_value.size());
    for(auto const& key_value : _data_value) res.push_back(key_value.first);
    return res;
  }
  const std::vector<std::string> PSet::pset_keys  () const
  {
    std::vector<std::string> res;
    res.reserve(_data_pset.size());
    for(auto const& key_value : _data_pset) res.push_back(key_value.first);
    return res;
  }
  bool PSet::contains_value (const std::string& key) const
  {
    return (_data_value.find(key) != _data_value.end());
  }
  bool PSet::contains_pset (const std::string& key) const
  {
    return (_data_pset.find(key) != _data_pset.end());
  }

  void PSet::strip(std::string& str, const std::string& key)
  {
    if(str.find(key) != 0) return;
    while(str.find(key) == 0)
      str = str.substr(key.size(),str.size());
  }
  
  void PSet::rstrip(std::string& str, const std::string& key)
  {
    size_t index = str.rfind(key);
    if(index >= str.size()) return;
    while(key.size() == (str.size() - index)) {
      str = str.substr(0,index);
      index = str.rfind(key);
      if(index >= str.size()) break;
    }
  }

  void PSet::no_space(std::string& txt){
    strip  ( txt, " "  );
    strip  ( txt, "\t" );
    strip  ( txt, " "  );
    rstrip ( txt, " "  );
    rstrip ( txt, "\t" );
    rstrip ( txt, " "  );
    if(txt.find(" ") < txt.size())
      throw FhiclLiteException("Space not allowed!");
    if(txt.find("\t") < txt.size())
      throw FhiclLiteException("Tab not allowed!");
  }

  std::pair<PSet::KeyChar_t,size_t> PSet::search(const std::string& txt, const size_t start) const
  {
    std::pair<KeyChar_t,size_t> res(kNone,size_t(-1));
    size_t index = 0;
    index = txt.find(":",start);
    if(index != std::string::npos && index < res.second) {
      res.first  = kParamDef;
      res.second = index;
    }
    index = txt.find("{",start);
    if(index != std::string::npos && index < res.second) {
      res.first  = kBlockStart;
      res.second = index;
    }
    index = txt.find("}",start);
    if(index != std::string::npos && index < res.second) {
      res.first  = kBlockEnd;
      res.second = index;
    }
    return res;
  }

  void PSet::add_value(std::string key,
		       std::string value)
  {
    //std::cout<<"  "<<key<<" => "<<value<<std::endl;
    if( _data_value.find(key) != _data_value.end() ||
        _data_pset.find(key)  != _data_pset.end() ) {
      std::string msg;
      msg = " Duplicate key: \"" + key + "\"";
      throw FhiclLiteException(msg.c_str());
    }
    no_space(key);
    if(key.empty()) throw FhiclLiteException("Empty key cannot be registered!");
    no_space(value);
    if(value.empty()) throw FhiclLiteException("Empty value cannot be registered!");
    _data_value[key]=value;
  }

  void PSet::add_pset(std::string key,
		      std::string value)
  {
    if( _data_value.find(key) != _data_value.end() ||
        _data_pset.find(key)  != _data_pset.end() ) {
      std::string msg;
      msg = " Duplicate key: \"" + key + "\"";
      throw FhiclLiteException(key.c_str());
    }
    no_space(key);
    if(key.empty()) throw FhiclLiteException("Empty key cannot be registered!");
    strip(value," ");
    rstrip(value," ");
    _data_pset.emplace(key,PSet(key,value));
  }
  
  void PSet::add(const std::string& contents)
  {
    if(contents.size()<1) return;
    size_t index=0;
    while(contents.find(" ",index) == index)
      index +=1;
    if(index >= contents.size()) return;
    
    size_t end_index=contents.size()-1;
    while(contents.rfind(" ",end_index) == end_index)
      end_index -= 1;
    if(end_index <= index || end_index > contents.size()) return;

    std::string key,value,tmp;
    KeyChar_t last_mark=kNone;

    while(index <= end_index) {
      
      auto next_marker = this->search(contents,index);
      if(next_marker.second > end_index) break;
      if(next_marker.first == kNone) break;
      /*
      std::cout<<"index: "<<index<<std::endl;
      std::cout<<"marker: "<<next_marker.second<<std::endl;
      std::cout<<"type  : "<<next_marker.first<<std::endl;
      std::cout<<"last  : "<<last_mark<<std::endl;
      std::cout<<"Inspecting: "<<"\""<<contents.substr(index,(next_marker.second-index))<<"\""<<std::endl;
      */      
      if(next_marker.first == kParamDef) {
	if(last_mark ==  kNone || last_mark == kBlockEnd){
	  key = contents.substr(index,(next_marker.second-index));
	  no_space(key);
	}
	else if(last_mark == kParamDef) {
	  tmp = contents.substr(index,(next_marker.second-index));
	  //std::cout<<"Inspecting: \""<<tmp<<"\"" <<std::endl;
	  strip(tmp," ");
	  rstrip(tmp," ");
	  size_t sep_index = tmp.find(" ");
	  if(sep_index >= tmp.size())
	    throw FhiclLiteException("Invalid format (key:value)");
	  
	  value = tmp.substr(0,sep_index);
	  // complete pair
	  this->add_value(key,value);
	  
	  key = value = "";
	  key = tmp.substr(sep_index+1,(tmp.size()-sep_index-1));
	  no_space(key);
	}
	
      }else if(next_marker.first == kBlockEnd)
	throw FhiclLiteException("Block end logic error!");
      else if(next_marker.first == kBlockStart){
	//std::cout<<"Block start!"<<std::endl;
	if(last_mark != kParamDef)
	  throw FhiclLiteException("Invalid paramter set start!");
	
	// fast forward till this block ends
	int start_ctr = 1;
	index = next_marker.second + 1;
	while(start_ctr && next_marker.second <= end_index) {
	  
	  next_marker = this->search(contents,next_marker.second+1);
	  switch(next_marker.first){
	  case kBlockStart:
	    start_ctr +=1;
	    break;
	  case kBlockEnd:
	    start_ctr -=1;
	    break;
	  default:
	    break;
	  }
	}
	if(start_ctr) {
	  std::string msg;
	  msg = "Invalid block:\n" + contents.substr(index,next_marker.second-index) + "\n";
	  throw FhiclLiteException(msg.c_str());
	}
	value = contents.substr(index,next_marker.second-index);
	//std::cout<<"PSET!\n"<<value<<"\n"<<std::endl;
	// complete key/value
	this->add_pset(key,value);

	key="";
	value="";
      }
      else throw FhiclLiteException("Unknown error!");
      
      index = next_marker.second+1;
      last_mark = next_marker.first;
    }

    if(index <= end_index) {
      
      if(!value.empty()) throw FhiclLiteException("Non-empty value @ process-end!");
      if(key.empty()) throw FhiclLiteException("Empty key @ process-end!");
      
      tmp = contents.substr(index+1,end_index-index);
      no_space(tmp);
      if(tmp.empty()) throw FhiclLiteException("Empty value @ end!");
      value = tmp;
      this->add_value(key,value);

      key = "";
      value = "";
    }
    
    if(!key.empty() || !value.empty())
      throw FhiclLiteException("Unpaired key:value!");

  }
  
  std::string PSet::dump(size_t indent_size) const
  {
    
    std::string res,out_indent,in_indent;
    for(size_t i=0; i<indent_size; ++i) out_indent += " ";
    res = out_indent + _name + " : {\n";
    for(size_t i=0; i<res.size(); ++i) in_indent += " ";
    for(auto const& key_value : _data_value)

      res += in_indent + key_value.first + " : " + key_value.second + "\n";

    for(auto const& key_value : _data_pset)

      res += key_value.second.dump(in_indent.size()) + "\n";

    res += out_indent + "}\n";
    return res;
  }

  const PSet& PSet::get_pset(const std::string& key) const
  {
    auto iter = _data_pset.find(key);
    if( iter == _data_pset.end() ) {
      std::string msg;
      msg = "Key does not exist: \"" + key + "\"";
      throw FhiclLiteException(msg);
    }
    return (*iter).second;

  }

}

#endif
