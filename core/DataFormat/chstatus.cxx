#ifndef LARLITE_CHSTATUS_CXX
#define LARLITE_CHSTATUS_CXX

#include "chstatus.h"

namespace larlite{

  chstatus::chstatus() : data_base(data::kChStatus)
  {}

  void chstatus::set_status(const geo::PlaneID& plane, const std::vector<short>& status_v)
  { fPlaneID = plane; fStatus_v = status_v; }

  void chstatus::set_status(const geo::PlaneID& plane, std::vector<short>&& status_v)
  { fPlaneID = plane; fStatus_v = std::move(status_v); }
  
}

#endif
