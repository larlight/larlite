#ifndef LARLITE_LARUTILCONFIG_CXX
#define LARLITE_LARUTILCONFIG_CXX

#include "LArUtilConfig.h"

namespace larutil {

  larlite::geo::DetId_t LArUtilConfig::_detector = larlite::geo::kMicroBooNE;

  bool LArUtilConfig::SetDetector(larlite::geo::DetId_t type)
  {
    if(_detector == type) return true;
    bool status = true;
    switch(type) {

    case larlite::geo::kArgoNeuT: 
    case larlite::geo::kMicroBooNE:
      _detector = type;
      break;
    case larlite::geo::kBo:
    case larlite::geo::kLBNE10kt:
    case larlite::geo::kJP250L:
    case larlite::geo::kLBNE35t:
    case larlite::geo::kLBNE34kt:
    case larlite::geo::kCSU40L:
    case larlite::geo::kLArIAT:
    case larlite::geo::kICARUS:
    case larlite::geo::kDetIdMax:
      larlite::Message::send(larlite::msg::kERROR,__FUNCTION__,
			      Form("Detector type: %d not supported!",type));
      status = false;
    }
    return status;
  }

}

#endif
