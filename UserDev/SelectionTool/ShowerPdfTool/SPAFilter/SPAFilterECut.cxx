#ifndef SELECTIONTOOL_SPAFILTERECUT_CXX
#define SELECTIONTOOL_SPAFILTERECUT_CXX

#include "SPAFilterECut.h"

namespace sptool {

  SPAFilterECut::SPAFilterECut() : SPAFilterBase()
  {
    _name = "SPAFilterECut";
    SetECut(0.);
  }

  SPAOrder SPAFilterECut::Select(const SPAData &data) {

    // SPAOrder is a vector of shower, track and vertex indices to be removed
    SPAOrder rm;
    
    // loop over showers
    for (size_t s=0; s < data._showers.size(); s++){
      if (data._showers.at(s)._energy < _ECut) { rm._rm_showers.insert(s); }
    }

    // loop over tracks
    for (size_t t=0; t < data._tracks.size(); t++){
      if (data._tracks.at(t)._energy < _ECut) { rm._rm_tracks.insert(t); }
    }
	
    return SPAOrder();
  }

}

#endif
