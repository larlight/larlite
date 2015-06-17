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
  keywords.insert(":"); // for key->param separator
  keywords.insert("{"); // for begin of PSet
  keywords.insert("}"); // for end of PSet
  Reset();
  SetVerbose(false);
}

void CfgFileReader::Reset()
{
  key_name="";
  mode=kIdle;
  _PSetManager.Reset();  
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
    if( mode != kKeyDone )
      throw fcl::FhiclLiteException("Wrong { usage");
    if( key_name.empty() )
      throw fcl::FhiclLiteException("PSet name not set!");

    if(_verbose) { std::cout<<"Found {. PSET: "<<key_name.c_str() << "\tMode: " << mode << std::endl; }
    key_name = "";

    // found the beginning of a set
    // modify variables to reflect change
    if (_PSetManager.getCurrentSet()){
      //create a new sub-Set of current set
      _PSetManager.addSubSet(key_name);
    }
    else{
      // create a new pset
      _PSetManager.addSet(fcl::PSet(key_name.c_str()));
    }

    mode = kIdle;
  }

  else if( word == ":") {
    if( mode != kKeyDone )
      throw fcl::FhiclLiteException("Wrong : usage");
    if( key_name.empty() )
      throw fcl::FhiclLiteException("Param name not set!");
    mode = kSearchParamValue;
    if (_verbose) { std::cout<<"KEY: "<<key_name.c_str() << "\tMode: " << mode << std::endl; }
  }

  else if( word == "}") { 
    if( mode != kIdle )
      throw fcl::FhiclLiteException("Wrong } usage");
    mode = kIdle;
    if (_verbose) { std::cout << "found }.\tMode: " << mode << std::endl; }
    key_name="";
    // reached the end of the set
    // modify variables to reflect change
    _PSetManager.closeCurrentSet();
  }

  else {
    if(mode==kIdle) {
      if(!key_name.empty()) {
	if (_verbose) { std::cout << key_name.c_str() << " ... " << word.c_str() << std::endl; }
	throw fcl::FhiclLiteException("Two consecutive strings?!");
      }
      mode = kKeyDone;
      key_name = word;
    }
    else if(mode==kSearchParamValue) {
      // add this parameter to current set
      _PSetManager.addParameter(key_name,word.c_str());
      mode = kIdle;
      key_name = "";
      if (_verbose) { std::cout<<"VALUE: "<<word.c_str() << "\tMode: " << mode << std::endl; }
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

  // now use PSet "dump" to test it - temporary
  _PSetManager.dump();
  /*
  for (size_t i=0; i < _set.size(); i++){
    std::string dumpOutput = _set.at(i).dump(0);
    std::cout << dumpOutput << std::endl;
  }
  */

  filestrm.close();
  //linestream >> val1 >> val2;
}

#endif
