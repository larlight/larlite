#ifndef CORRELATION_CXX
#define CORRELATION_CXX

#include "Correlation.h"

namespace geotree{

  Correlation::Correlation(double s, ::larlite::geoalgo::Point_t vtx,
			   ::geotree::RelationType_t type)
  {
    _score = s;
    _vtx   = vtx;
    _type  = type;
  }

  void Correlation::EditCorrelation(double s, ::larlite::geoalgo::Point_t vtx,
			   ::geotree::RelationType_t type)
  {
    _score = s;
    _vtx   = vtx;
    _type  = type;
  }


}

#endif
