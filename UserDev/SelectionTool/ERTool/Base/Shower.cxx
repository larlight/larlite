#ifndef ERTOOL_SHOWER_CXX
#define ERTOOL_SHOWER_CXX

#include "Shower.h"

namespace ertool {

  Shower::Shower() : RecoObjBase()
  { 
    Reset(); 
  }

  Shower::Shower(const ::geoalgo::Vector& start,
		 const ::geoalgo::Vector& dir,
		 double length, double radius) :
    ::geoalgo::Cone(start,dir,length,radius)
  { }


  void Shower::Reset() 
  {
    ::geoalgo::Cone::Start(::geoalgo::Point_t(3));
    ::geoalgo::Cone::Dir(::geoalgo::Point_t(1,1,1));
    ::geoalgo::Cone::Length(1.);
    ::geoalgo::Cone::Radius(1.);
    RecoObjBase::Reset();
    _dedx       = kINVALID_DOUBLE;
    _energy     = kINVALID_DOUBLE;
  }
}
#endif
