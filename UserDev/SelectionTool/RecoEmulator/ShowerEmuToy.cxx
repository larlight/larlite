#ifndef __TRACKEMUTOY_CXX__
#define __TRACKEMUTOY_CXX__

#include "ShowerEmuToy.h"

namespace recoemu {

  void ShowerEmuToy::Configure(const ::fcllite::PSet &pset)
  {}
  
  recoemu::Shower_t ShowerEmuToy::Emulate(const recoemu::Shower_t& mc)
  {
    Shower_t result;
    // return result;
    return mc;
  }

}

#endif
