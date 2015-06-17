#ifndef ERTOOL_SHOWER_CXX
#define ERTOOL_SHOWER_CXX

#include "Shower.h"
#include "ERToolConstants.h"
namespace ertool {

  Shower::Shower() : RecoObjBase()
  {
    _reco_type = kShower;
    Reset(); 
  }

  Shower::Shower(const ::geoalgo::Vector& start,
		 const ::geoalgo::Vector& dir,
		 double length, double radius) :
    ::geoalgo::Cone(start,dir,length,radius)
  { _reco_type = kShower; }


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
