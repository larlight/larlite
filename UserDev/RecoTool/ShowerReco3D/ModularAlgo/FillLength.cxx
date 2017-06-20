#ifndef FILLLENGTH_CXX
#define FILLLENGTH_CXX

#include "FillLength.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

  FillLength::FillLength()
  {
    _name        = "FillLength";
  }

void FillLength::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                  Shower_t& resultShower) {
  
  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
  }
  
  auto & clusters = proto_shower.params();

  // use collection-plane cluster to compute length

  bool filled = false;
  
  for (auto const& clus : clusters) {

    if (clus.plane_id.Plane != 2) continue;
    
    auto sW  = clus.start_point.w;
    auto eW  = clus.end_point.w;
    
    // length on Y-plane projection
    auto dLY = fabs(eW - sW);
    
    resultShower.fLength = dLY / fabs(resultShower.fDCosStart[2]) ;
    
    resultShower.fOpeningAngle = clusters.front().opening_angle;
    
    filled = true;
    
  }// for all clusters    
  
  // if collection-plane info not found -> skip
  if (filled == false) {
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing collection-plane cluster";
    throw ShowerRecoException(ss.str());
  }
  
  return;
}

}
  
#endif
  
