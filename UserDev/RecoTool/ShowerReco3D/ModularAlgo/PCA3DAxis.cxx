#ifndef PCA3DAXISMODULE_CXX
#define PCA3DAXISMODULE_CXX

#include "PCA3DAxis.h"
#include <sstream>

namespace showerreco {
  
  void PCA3DAxis::do_reconstruction(const ProtoShower & proto_shower,
                                    Shower_t& resultShower) {
    
    // auto & clusters = proto_shower.params();
    
    
    // This is really simple.  Read in the 3D params and use that to
    // set the 3D axis of the shower.  Right now, no checks against the
    // 2D projection but should be added somewhere.
    
    
    //if the module does not have 2D info -> fail the reconstruction
    if (!proto_shower.hasCluster3D()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 3D cluster";
      throw ShowerRecoException(ss.str());
    }
    
    
    resultShower.fDCosStart = proto_shower.params3D().principal_dir;
    
    return;
  }
  
  
} //showerreco

#endif
