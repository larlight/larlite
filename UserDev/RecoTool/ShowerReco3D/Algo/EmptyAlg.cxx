#ifndef SHOWERRECO3D_ALGO_EMPTYALG_CXX
#define SHOWERRECO3D_ALGO_EMPTYALG_CXX

#include "EmptyAlg.h"

namespace showerreco {
  
  EmptyAlg::EmptyAlg() : ShowerRecoAlgBase()
  {}

  Shower_t EmptyAlg::RecoOneShower(const ShowerClusterSet_t& clusters)
  {
    return Shower_t();
  }

}

#endif
