#ifndef ERTOOL_SHOWER_CXX
#define ERTOOL_SHOWER_CXX

#include "Shower.h"

namespace ertool {

  Shower::Shower() : RecoObjBase()
  { Reset(); }

  void Shower::Reset() 
  {
    ::geoalgo::HalfLine::Start(::geoalgo::Point_t(3));
    ::geoalgo::HalfLine::Dir(::geoalgo::Point_t(3));
    RecoObjBase::Reset();
    _radius     = kINVALID_DOUBLE;
    _cone_angle = kINVALID_DOUBLE;
    _dedx       = kINVALID_DOUBLE;
    _energy     = kINVALID_DOUBLE;
  }
}
#endif
