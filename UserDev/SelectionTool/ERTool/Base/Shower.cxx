#ifndef ERTOOL_SHOWER_CXX
#define ERTOOL_SHOWER_CXX

#include "Shower.h"

namespace ertool {

  Shower::Shower() : RecoObjBase()
  { Reset(); }

  Shower::Shower(const ::geoalgo::Vector& start,
		 const ::geoalgo::Vector& dir,
		 double height, double radius)
  {
    Reset();
    ::geoalgo::Cone::Start(start);
    ::geoalgo::Cone::Dir(dir);
    ::geoalgo::Cone::Height(height);
    ::geoalgo::Cone::Radius(radius);
  }


  void Shower::Reset() 
  {
    ::geoalgo::Cone::Start(::geoalgo::Point_t(3));
    ::geoalgo::Cone::Dir(::geoalgo::Point_t(3));
    ::geoalgo::Cone::Height(0.);
    ::geoalgo::Cone::Radius(0.);
    ::geoalgo::Cone::Angle(0.);
    RecoObjBase::Reset();
    _dedx       = kINVALID_DOUBLE;
    _energy     = kINVALID_DOUBLE;
  }
}
#endif
