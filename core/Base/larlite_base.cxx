#ifndef LARLITE_BASE_CXX
#define LARLITE_BASE_CXX

#include "larlite_base.h"
#include "AnalysisConstants.h"
#include "GeoTypes.h"
#include "FrameworkConstants.h"
#include "RawConstants.h"
#include "DataFormatConstants.h"
#include "GeoConstants.h"
#include "MCConstants.h"

namespace larlite {
  //#####################################################
  void larlite_base::set_verbosity(msg::Level level)
  //#####################################################
  {
    
    _verbosity_level=level;
    
    for(size_t i=(size_t)(msg::kDEBUG); i<(size_t)(msg::kMSG_TYPE_MAX); ++i)
      _verbosity[i]=false;
    
    switch(level){
    case msg::kDEBUG:
      _verbosity[msg::kDEBUG]=true;
    case msg::kINFO:
      _verbosity[msg::kINFO]=true;
    case msg::kNORMAL:
      _verbosity[msg::kNORMAL]=true;
    case msg::kWARNING:
      _verbosity[msg::kWARNING]=true;
    case msg::kERROR:
      _verbosity[msg::kERROR]=true;
    case msg::kMSG_TYPE_MAX:
      break;
    }
    
  }
}

#endif
