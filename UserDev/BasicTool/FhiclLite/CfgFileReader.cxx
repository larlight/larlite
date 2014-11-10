#ifndef CFGFILEREADER_CXX
#define CFGFILEREADER_CXX

#include "CfgFileReader.h"

void strip(std::string& str, const std::string& key)
{
  if(str.find(key) != 0) return;
  while(str.find(key) == 0)

    str = str.substr(key.size(),str.size());

}

CfgFileReader::CfgFileReader()
{
  keywords.insert(":");
  keywords.insert("{");
  keywords.insert("}");
  Reset();
}

void CfgFileReader::Reset()
{
  key_name="";
  mode=kIdle;
}

void CfgFileReader::ProcessWord(const std::string& word) 
{
  /*
  kIdle            /// Default or after "}"
  kPSetKeyDone     /// After a single word string within kIdle
  kSearchParamKey  /// After "{" or after a single word within kSearchParamValue
  kSearchParamValue /// After ":"
  */

  if( word == "{" ) { 
    if( mode != kPSetKeyDone)
      throw fcl::FhiclLiteException("Wrong { usage");
    if( key_name.empty() )
      throw fcl::FhiclLiteException("PSet name not set!");
    std::cout<<"PSET: "<<key_name.c_str()<<std::endl;
    mode = kSearchParamKey;
  }

  else if( word == ":") {
    if( mode != kParamKeyDone )
      throw fcl::FhiclLiteException("Wrong : usage");
    if( key_name.empty() )
      throw fcl::FhiclLiteException("Param name not set!");
    std::cout<<"KEY: "<<key_name.c_str()<<std::endl;
    mode = kSearchParamValue;
  }

  else if( word == "}") { 
    if( mode != kSearchParamKey )
      throw fcl::FhiclLiteException("Wrong } usage");
    mode = kIdle;
    key_name="";
  }

  else {
    if(mode==kIdle) {
      if(!key_name.empty()) {
	std::cout<<key_name.c_str()<< " ... " <<word.c_str()<<std::endl;
	throw fcl::FhiclLiteException("Two consecutive strings?!");
      }
      mode = kPSetKeyDone;
      key_name = word;
    }
    else if(mode==kSearchParamKey) {
      mode = kParamKeyDone;
      key_name = word;      
    }
    else if(mode==kSearchParamValue) {
      std::cout<<"VALUE: "<<word.c_str()<<std::endl;;
      mode = kSearchParamKey;
      key_name = "";
    }
  }

}

void CfgFileReader::ReadWord(std::string word)
{
  if(word.empty()) return;

  for(auto const& key : keywords) {

    if(word != key && word.find(key)<word.size()) {
      
      size_t pos = word.find(key);
      
      if(pos) ReadWord(word.substr(0,pos));
      ReadWord(key);
      ReadWord(word.substr(pos+1,word.size()));
      return;
    }
  }
  ProcessWord(word);
}

void CfgFileReader::ReadFile(std::string fname)
{
  std::ifstream filestrm(fname.c_str());
  std::string   line;

  while(std::getline(filestrm, line)) {

    if(line.empty()) continue;
    //std::cout<<line.c_str()<<std::endl;
    
    std::stringstream linestrm(line);
    std::string       valid_line;

    std::getline(linestrm, valid_line, '#');

    if(valid_line.empty()) continue;

    std::stringstream wordstrm(valid_line);
    std::string       word;

    while(wordstrm>>word)

      ReadWord(word);

  }

  filestrm.close();
  //linestream >> val1 >> val2;
}

#endif
