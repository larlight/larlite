#ifndef LARLITE_TRACK_CXX
#define LARLITE_TRACK_CXX

#include "track.h"

namespace larlite {

  //########################################
  void track::clear_data()
  //########################################
  {
    data_base::clear_data();  
    fID = -1; 
    fXYZ.clear(); 
    fDir.clear(); 
    fCov.clear(); 
    fdQdx.clear();
    fFitMomentum.clear(); 
  }
  
}
#endif
