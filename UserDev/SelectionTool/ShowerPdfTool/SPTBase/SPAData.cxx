#ifndef SPADATA_CXX
#define SPADATA_CXX

#include "SPAData.h"

namespace sptool {

  SPAShower::SPAShower() 
  { Reset(); }

  void SPAShower::Reset() 
  {
    HalfLine::Start(::geoalgo::Point_t(3));
    HalfLine::Dir(::geoalgo::Point_t(3));

    _radius     = kINVALID_DOUBLE;
    _cone_angle = kINVALID_DOUBLE;
    _energy     = kINVALID_DOUBLE;
    _dedx       = kINVALID_DOUBLE;
    _cosmogenic = kINVALID_DOUBLE;
  }

  SPATrack::SPATrack()
  { Reset(); }

  void SPATrack::Reset()
  {
    Trajectory::clear();
    _energy     = kINVALID_DOUBLE;
    _cosmogenic = kINVALID_DOUBLE;
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
