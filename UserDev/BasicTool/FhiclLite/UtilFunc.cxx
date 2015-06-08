#ifndef BASICTOOL_FHICLLITE_UTILFUNC_CXX
#define BASICTOOL_FHICLLITE_UTILFUNC_CXX

#include "UtilFunc.h"
#include "FhiclLiteException.h"
namespace fclite {

  template<> std::string FromString( const std::string& value)
  { return value; }

  template<> float FromString( const std::string& value )
  { return std::stof(value); }

  template<> double FromString( const std::string& value )
  { return std::stod(value); }

  template<> unsigned short FromString( const std::string& value)
  { return std::stoul(value); }

  template<> unsigned int FromString( const std::string& value)
  { return std::stoul(value); }

  template<> unsigned long FromString( const std::string& value)
  { return std::stoul(value); }

  template<> short FromString( const std::string& value )
  { return std::stoi(value); }

  template<> int FromString( const std::string& value )
  { return std::stoi(value); }

  template<> long FromString( const std::string& value )
  { return std::stol(value); }

  template<> bool FromString(const std::string& value )
  {
    std::string tmp=value;
    std::transform(tmp.begin(),tmp.end(),tmp.begin(),::tolower);
    if( value == "true"  || value == "1" ) return true;
    if( value == "false" || value == "0" ) return false;
    tmp = "Invalid bool expression: " + tmp;
    throw FhiclLiteException(tmp);
    return false;
  }

  template<> std::vector< std::string    > FromString (const std::string& value )
  {
    std::vector<std::string> res;
    if(value.find("[") != 0 || (value.rfind("]")+1) != value.size()) {
      std::string msg;
      msg = "Invalid vector expression: " + value;
      throw FhiclLiteException(msg);
    }
    size_t index = 1;
    while((index+1) < value.size()) {
      size_t next_index = value.find(",",index);
      if(next_index >= value.size()) break;
      res.push_back(value.substr(index,next_index-index));
      index = next_index + 1;
    }
    if( (index+1) < value.size() )
      res.push_back(value.substr(index,value.size()-index-1));
    return res;
   }

  template<> std::vector< float > FromString< std::vector< float > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<float> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<float>(v) );
    return res;
  }

  template<> std::vector< double > FromString< std::vector< double > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<double> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<double>(v) );
    return res;
  }

  template<> std::vector< short > FromString< std::vector< short > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<short> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<short>(v) );
    return res;
  }

  template<> std::vector< int > FromString< std::vector< int > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<int> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<int>(v) );
    return res;
  }

  template<> std::vector< long > FromString< std::vector< long > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<long> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<long>(v) );
    return res;
  }

  template<> std::vector< unsigned short > FromString< std::vector< unsigned short > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<unsigned short> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<unsigned short>(v) );
    return res;
  }

  template<> std::vector< unsigned int > FromString< std::vector< unsigned int > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<unsigned int> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<unsigned int>(v) );
    return res;
  }

  template<> std::vector< unsigned long > FromString< std::vector< unsigned long > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<unsigned long> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<unsigned long>(v) );
    return res;
  }

  template<> std::vector< bool > FromString< std::vector< bool > > (const std::string& value )
  {
    auto str_v = FromString<std::vector<std::string> >(value);
    std::vector<bool> res;
    res.reserve(str_v.size());
    for(auto const& v : str_v)
      res.push_back( FromString<bool>(v) );
    return res;
  }
  
  template <class T> std::string ToString(const T& value)
  { return std::to_string(value); }

  template<> std::string ToString<std::string>(const std::string& value)
  { return std::string(value); }
  
}
#endif
