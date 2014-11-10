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
}

void CfgFileReader::Reset()
{
  key_name="";
  mode=kIdle;
  _thisSet = 0;
  _topSet  = 0;
  _NopenPSet = 0;
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

    std::cout<<"Found {. PSET: "<<key_name.c_str() << "\tMode: " << mode << std::endl;
    _NopenPSet += 1;
    key_name = "";

    // found the beginning of a set
    // modify variables to reflect change
    if (_thisSet){
      // a set is open...create the new set as part of it
      _topSet = _thisSet;
      _topSet->set(key_name.c_str(), fcl::PSet(key_name.c_str()) );
      _thisSet = &_topSet->get_pset_writeable(key_name.c_str());
    }
    else{
    // create a new pset
    _set.push_back(fcl::PSet(key_name.c_str()));
    _thisSet = &_set.back();
    }

    mode = kIdle;
  }

  else if( word == ":") {
    if( mode != kKeyDone )
      throw fcl::FhiclLiteException("Wrong : usage");
    if( key_name.empty() )
      throw fcl::FhiclLiteException("Param name not set!");
    mode = kSearchParamValue;
    std::cout<<"KEY: "<<key_name.c_str() << "\tMode: " << mode << std::endl;
  }

  else if( word == "}") { 
    if( mode != kIdle )
      throw fcl::FhiclLiteException("Wrong } usage");
    _NopenPSet -= 1;
    if (_NopenPSet < 0)
      throw fcl::FhiclLiteException("Wrong } usage -- Too many }");
    mode = kIdle;
    std::cout << "found }.\tMode: " << mode << std::endl;
    key_name="";
    // reached the end of the set
    // modify variables to reflect change
    _thisSet = _topSet;
    _topSet = 0;
  }

  else {
    if(mode==kIdle) {
      if(!key_name.empty()) {
	std::cout<<key_name.c_str()<< " ... " <<word.c_str()<<std::endl;
	throw fcl::FhiclLiteException("Two consecutive strings?!");
      }
      mode = kKeyDone;
      key_name = word;
    }
    else if(mode==kSearchParamValue) {
      _thisSet->set(key_name,word.c_str());
      mode = kIdle;
      key_name = "";
      std::cout<<"VALUE: "<<word.c_str() << "\tMode: " << mode << std::endl;;
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
  for (size_t i=0; i < _set.size(); i++){
    std::string dumpOutput = _set.at(i).dump(0);
    std::cout << dumpOutput << std::endl;
  }

  filestrm.close();
  //linestream >> val1 >> val2;
}

#endif
