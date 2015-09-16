#ifndef SHOWERRECOMODULEBASE_CXX
#define SHOWERRECOMODULEBASE_CXX

#include "ShowerRecoModuleBase.h"

namespace showerreco{

  ShowerRecoModuleBase::ShowerRecoModuleBase()
    : _tree(nullptr)
  {
    _verbose = false; 
  }

}

#endif
