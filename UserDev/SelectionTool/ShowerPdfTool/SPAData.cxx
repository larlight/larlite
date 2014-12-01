#ifndef SPADATA_CXX
#define SPADATA_CXX

#include "SPAData.h"

namespace sptool {

  SPAShower::SPAShower() 
    : _start(3)
    , _dir(3)
  {
    Reset();
  }

  void SPAShower::Reset() 
  {
    for(size_t i=0; i<_start.size(); ++i) {
      _start [i] = kINVALID_DOUBLE;
      _dir   [i] = kINVALID_DOUBLE;
    }

    _radius     = kINVALID_DOUBLE;
    _cone_angle = kINVALID_DOUBLE;
    _energy     = kINVALID_DOUBLE;
    _dedx       = kINVALID_DOUBLE;

  }

  SPAData::SPAData()
    : _showers()
    , _tracks()
    , _vtxs()
  { Reset(); }

  void SPAData::Reset()
  {
    _showers.clear();
    _tracks.clear();
    _vtxs.clear();
  }
}
#endif
