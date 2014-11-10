/**
 * \file CfgFileReader.h
 *
 * \ingroup FhiclLite
 * 
 * \brief Class def header for a class CfgFileReader
 *
 * @author kazuhiro
 */

/** \addtogroup FhiclLite

    @{*/
#ifndef CFGFILEREADER_H
#define CFGFILEREADER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "FhiclLiteException.h"
#include "PSet.h"
/**
   \class CfgFileReader
   User defined class CfgFileReader ... these comments are used to generate
   doxygen documentation!
 */
class CfgFileReader{

public:

  /// Default constructor
  CfgFileReader();

  /// Default destructor
  virtual ~CfgFileReader(){};

  void Reset();

  void ReadFile(std::string fname);

protected:

  enum Mode_t {
    kIdle,            /// Default or after "}"
    kKeyDone,
    //   kPSetKeyDone,     /// After a single word string within kIdle
    kSearchParamKey,  /// After "{" or after a single word within kSearchParamValue
    //    kParamKeyDone,    
    kSearchParamValue /// After ":"
  };

  void ReadWord(std::string word);

  void ProcessWord(const std::string& word);

  std::set<std::string> keywords;
  
  std::string key_name;

  Mode_t mode;

  std::vector<fcl::PSet> _set;

  fcl::PSet *_thisSet;
  fcl::PSet *_topSet;

  // vareiable to keep track of how many open PSets there are
  // reason: if reaches the end of file and not == 0 -> Bad!
  int _NopenPSet;
  
};

#endif
/** @} */ // end of doxygen group 

