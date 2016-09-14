#ifndef __FHICLLITE_UTILFUNC_H__
#define __FHICLLITE_UTILFUNC_H__

#include "PSet.h"

namespace fcllite {

  /// Given a configuration file (full path), read & parse contents to create larcv::PSet
  std::string ConfigFile2String(std::string fname);
  /// Given a configuration file (full path), create and return larcv::PSet
  PSet CreatePSetFromFile(std::string fname,std::string cfg_name="cfg");

}

#endif
