#ifndef ESTIMATELENGTH_CXX
#define ESTIMATELENGTH_CXX

#include "EstimateLength.h"
#include "LArUtil/GeometryHelper.h"
#include "Base/DataFormatConstants.h"

namespace showerreco {

  EstimateLength::EstimateLength()
  {
    _name        = "EstimateLength";
  }

void EstimateLength::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                  Shower_t& resultShower) {
  
  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
  }
  
  auto & clusters = proto_shower.params();

  // use collection-plane cluster to compute length

  resultShower.fLength       = -1.0*::larlite::data::kINVALID_DOUBLE;
  resultShower.fOpeningAngle = -1.0*::larlite::data::kINVALID_DOUBLE;

  for (auto const& clus : clusters) {

    if (clus.plane_id.Plane != 2) continue;
    
    auto sW  = clus.start_point.w;
    auto eW  = clus.end_point.w;
    
    // length on Y-plane projection
    auto dLY = fabs(eW - sW);
    
    resultShower.fLength = dLY / fabs(resultShower.fDCosStart[2]) ;
    
    resultShower.fOpeningAngle = clusters.front().opening_angle;
    
  }// for all clusters    
  
  std::cout << "...done estimate length" << std::endl;
  return;
}

}
  
#endif
  
