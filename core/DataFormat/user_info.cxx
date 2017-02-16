#ifndef LARLITE_USER_INFO_CXX
#define LARLITE_USER_INFO_CXX

#include "user_info.h"

namespace larlite {

  user_info::user_info()
    : data_base(data::kUserInfo)
  {clear_data();}
  
  //**********************************************************  
  void user_info::clear_data()
  //**********************************************************
  {
    _d_map.clear();
    _i_map.clear();
    _s_map.clear();
    _b_map.clear();
    _darray_map.clear();
    _iarray_map.clear();
    _sarray_map.clear();
    _barray_map.clear();
  }
  
  //**********************************************************
  void user_info::dump_contents()
  //**********************************************************
  {
    
    std::cout<<"  Start contents dump..."<<std::endl;
    std::string msg("");
    
    if(_d_map.size()){
      msg ="  Contents of double variables shown below.\n";
      msg+=Form("    %-20s ... Value\n","Key");
      for(std::map<std::string,double>::const_iterator iter(_d_map.begin());
	  iter!=_d_map.end();
	  ++iter)
	msg+=Form("    %-20s ... %g\n",(*iter).first.c_str(), (*iter).second);
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_i_map.size()){
      msg ="  Contents of int variables shown below.\n";
      msg+=Form("    %-20s ... Value\n","Key");
      for(std::map<std::string,int>::const_iterator iter(_i_map.begin());
	  iter!=_i_map.end();
	  ++iter)
	msg+=Form("    %-20s ... %d\n",(*iter).first.c_str(), (*iter).second);
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_s_map.size()){
      msg ="  Contents of std::string variables shown below.\n";
      msg+=Form("    %-20s ... Value\n","Key");
      for(std::map<std::string,std::string>::const_iterator iter(_s_map.begin());
	  iter!=_s_map.end();
	  ++iter)
	msg+=Form("    %-20s ... %s\n",(*iter).first.c_str(), (*iter).second.c_str());
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_b_map.size()){
      msg ="  Contents of bool variables shown below.\n";
      msg+=Form("    %-20s ... Value\n","Key");
      for(std::map<std::string,bool>::const_iterator iter(_b_map.begin());
	  iter!=_b_map.end();
	  ++iter)
	msg+=Form("    %-20s ... %d\n",(*iter).first.c_str(), (int)((*iter).second));
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_darray_map.size()){
      msg ="  Contents of double array shown below.\n";
      for(std::map<std::string,std::vector<double> >::const_iterator iter(_darray_map.begin());
	  iter!=_darray_map.end();
	  ++iter){
	msg+=Form("  Key=%-20s\n    ",(*iter).first.c_str());
	int ctr=1;
	for(std::vector<double>::const_iterator cont_iter((*iter).second.begin());
	    cont_iter!=(*iter).second.end();
	    ++cont_iter){
	  msg+=Form("%g ",(*cont_iter));
	  ctr++;
	  if(ctr%8==0)
	    msg+="\n    ";
	}
      }
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_iarray_map.size()){
      msg ="  Contents of int array shown below.\n";
      for(std::map<std::string,std::vector<int> >::const_iterator iter(_iarray_map.begin());
	  iter!=_iarray_map.end();
	  ++iter){
	msg+=Form("  Key=%-20s\n    ",(*iter).first.c_str());
	int ctr=1;
	for(std::vector<int>::const_iterator cont_iter((*iter).second.begin());
	    cont_iter!=(*iter).second.end();
	    ++cont_iter){
	  msg+=Form("%d ",(*cont_iter));
	  ctr++;
	  if(ctr%8==0)
	    msg+="\n    ";
	}
      }
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_sarray_map.size()){
      msg ="  Contents of std::string array shown below.\n";
      for(std::map<std::string,std::vector<std::string> >::const_iterator iter(_sarray_map.begin());
	  iter!=_sarray_map.end();
	  ++iter){
	msg+=Form("  Key=%-20s\n    ",(*iter).first.c_str());
	int ctr=1;
	for(std::vector<std::string>::const_iterator cont_iter((*iter).second.begin());
	    cont_iter!=(*iter).second.end();
	    ++cont_iter){
	  msg+=Form("%s ",(*cont_iter).c_str());
	  ctr++;
	  if(ctr%8==0)
	    msg+="\n    ";
	}
      }
      std::cout<<msg.c_str()<<std::endl;
    }
    
    if(_barray_map.size()){
      msg ="  Contents of bool array shown below.\n";
      for(std::map<std::string,std::vector<bool> >::const_iterator iter(_barray_map.begin());
	  iter!=_barray_map.end();
	  ++iter){
	msg+=Form("  Key=%-20s\n    ",(*iter).first.c_str());
	int ctr=1;
	for(std::vector<bool>::const_iterator cont_iter((*iter).second.begin());
	    cont_iter!=(*iter).second.end();
	    ++cont_iter){
	  msg+=Form("%d ",(int)(*cont_iter));
	  ctr++;
	  if(ctr%8==0)
	    msg+="\n    ";
	}
      }
      std::cout<<msg.c_str()<<std::endl;
    }
    std::cout<<"  End of dump..."<<std::endl;
  }

  //**********************************************************  
  void user_info::append(std::string key, double value)
  //**********************************************************
  {
    if(_darray_map.find(key)==_darray_map.end()) 
      _darray_map[key]=std::vector<double>();
    _darray_map[key].push_back(value);
  }
  
  //**********************************************************
  void user_info::append(std::string key, int value)
  //**********************************************************
  {
    if(_iarray_map.find(key)==_iarray_map.end())
      _iarray_map[key]=std::vector<int>();
    _iarray_map[key].push_back(value);
  }
  
  //**********************************************************
  void user_info::append(std::string key, std::string value)
  //**********************************************************
  {
    if(_sarray_map.find(key)==_sarray_map.end())
      _sarray_map[key]=std::vector<std::string>();
    _sarray_map[key].push_back(value);
  }
  
  //**********************************************************
  void user_info::append(std::string key, bool value)
  //**********************************************************
  {
    if(_barray_map.find(key)==_barray_map.end())
      _barray_map[key]=std::vector<bool>();
    _barray_map[key].push_back(value);
  }
  
  //**********************************************************
  double user_info::get_double(std::string key) const 
  //**********************************************************
  {  
    std::map<std::string,double>::const_iterator item(_d_map.find(key));
    if(item==_d_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return (*item).second;
  }

  //**********************************************************
  int user_info::get_int(std::string key) const 
  //**********************************************************
  {
    std::map<std::string,int>::const_iterator item(_i_map.find(key));
    if(item==_i_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return (*item).second;
  }

  //**********************************************************  
  std::string user_info::get_string(std::string key) const 
  //**********************************************************
  {
    std::map<std::string,std::string>::const_iterator item(_s_map.find(key));
    if(item==_s_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return (*item).second;
  }
  
  //**********************************************************  
  bool user_info::get_bool(std::string key) const 
  //**********************************************************
  {
    std::map<std::string,bool>::const_iterator item(_b_map.find(key));
    if(item==_b_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return (*item).second;
  }

  //**********************************************************  
  std::vector<double>* user_info::get_darray(std::string key)  
  //**********************************************************
  {
    std::map<std::string,std::vector<double> >::iterator item(_darray_map.find(key));
    if(item==_darray_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return &((*item).second);
  }

  //**********************************************************  
  std::vector<int>* user_info::get_iarray(std::string key)  
  //**********************************************************
  {
    std::map<std::string,std::vector<int> >::iterator item(_iarray_map.find(key));
    if(item==_iarray_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return &((*item).second);
  }

  //**********************************************************  
  std::vector<std::string>* user_info::get_sarray(std::string key)  
  //**********************************************************
  {
    std::map<std::string,std::vector<std::string> >::iterator item(_sarray_map.find(key));
    if(item==_sarray_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return &((*item).second);
  }

  //**********************************************************  
  std::vector<bool>* user_info::get_barray(std::string key)  
  //**********************************************************
  {
    std::map<std::string,std::vector<bool> >::iterator item(_barray_map.find(key));
    if(item==_barray_map.end()){
      std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
	        << "Key " << key.c_str() << " does not exist!" << std::endl;
      return 0;
    }
    return &((*item).second);
  }

  event_user::event_user(std::string name) : event_base(data::kUserInfo,name)
  {clear_data();}

}
  
#endif
