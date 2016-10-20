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

  event_chstatus::event_chstatus(std::string name) : event_base(data::kChStatus,name) { clear_data();}
  
}

#endif
