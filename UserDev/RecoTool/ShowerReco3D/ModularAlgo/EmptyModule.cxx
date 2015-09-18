#ifndef EMPTYMODULE_CXX
#define EMPTYMODULE_CXX

#include "EmptyModule.h"

namespace showerreco{

  void EmptyModule::do_reconstruction(const ShowerClusterSet_t &, Shower_t &){
    
    // This function takes the shower cluster set and computes the best fit 3D axis
    // and then assigns it to the shower

  }


} //showerreco

#endif