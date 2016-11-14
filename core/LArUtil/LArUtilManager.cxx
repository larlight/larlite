#ifndef LARUTILMANAGER_CXX
#define LARUTILMANAGER_CXX

#include "LArUtilManager.h"

namespace larutil {

  bool LArUtilManager::Reconfigure(larlite::geo::DetId_t type)
  {

    if(type == LArUtilConfig::Detector()) return true;

    bool status = LArUtilConfig::SetDetector(type);

    if(!status) return status;

    return ReconfigureUtilities();

  }

  bool LArUtilManager::ReconfigureUtilities()
  {
    bool status = true;

    // Geometry
    larlite::Message::send(larlite::msg::kNORMAL,__FUNCTION__,"Reconfiguring Geometry");
    Geometry* geom = (Geometry*)(Geometry::GetME(false));
    geom->SetFileName(Form("%s/LArUtil/dat/%s",
			   getenv("LARLITE_COREDIR"),
			   kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str()));
    geom->SetTreeName(kTREENAME_GEOMETRY);
    status = status && geom->LoadData(true);

    // LArProperties
    larlite::Message::send(larlite::msg::kNORMAL,__FUNCTION__,"Reconfiguring LArProperties");
    LArProperties* larp = (LArProperties*)(LArProperties::GetME(false));
    larp->SetFileName(Form("%s/LArUtil/dat/%s",
			   getenv("LARLITE_COREDIR"),
			   kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str()));
    larp->SetTreeName(kTREENAME_LARPROPERTIES);
    status = status && larp->LoadData(true);

    // DetectorProperties
    larlite::Message::send(larlite::msg::kNORMAL,__FUNCTION__,"Reconfiguring DetectorProperties");
    DetectorProperties* detp = (DetectorProperties*)(DetectorProperties::GetME(false));
    detp->SetFileName(Form("%s/LArUtil/dat/%s",
			   getenv("LARLITE_COREDIR"),
			   kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str()));
    detp->SetTreeName(kTREENAME_DETECTORPROPERTIES);
    status = status && detp->LoadData(true);

    if(status){

      // GeometryHelper
      larlite::Message::send(larlite::msg::kNORMAL,__FUNCTION__,"Reconfiguring GeometryHelper...");
      GeometryHelper* gser = (GeometryHelper*)(GeometryHelper::GetME());
      gser->Reconfigure();

      // GeometryHelper
      larlite::Message::send(larlite::msg::kNORMAL,__FUNCTION__,"Reconfiguring GeometryHelper...");
      GeometryHelper* ghelp = (GeometryHelper*)(GeometryHelper::GetME());
      ghelp->Reconfigure();

    }

    return status;
  }

}

#endif
