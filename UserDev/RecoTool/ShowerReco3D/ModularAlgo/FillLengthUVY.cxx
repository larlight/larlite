#ifndef FILLLENGTHUVY_CXX
#define FILLLENGTHUVY_CXX

#include "FillLengthUVY.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

  FillLengthUVY::FillLengthUVY()
  {
    _name = "FillLengthUVY";
  }

  void FillLengthUVY::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
					Shower_t& resultShower) {
    
    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
    }
  
    auto geomHelper = larutil::GeometryHelper::GetME();

    auto & clusters = proto_shower.params();

    const auto& dir3D = resultShower.fDCosStart;

    std::array<double,3> length_v;
    std::array<double,3> opening_angle_v;

    std::array<double,3> plane_f_v;

    for(auto& v : length_) 
      v = -1.0*::larlite::data::kINVALID_DOUBLE;
    
    opening_angle_v = length_v;
    plane_f_v = length_v;
    pitch_v = length_v;
    
    TVector3 dir3D_tv(dir3D);    
    for(size_t plane=0; plane<3; ++plane)
      plane_f_v[plane] = geomHelper->Project_3DLine_OnPlane(dir3D_tv,plane).Mag();

    for (auto const& clus : clusters) {
      size_t plane = clus.plane_id.Plane;
      
      double sW  = clus.start_point.w;
      double sT  = clus.start_point.t;
      double eW  = clus.end_point.w;
      double eT  = clus.end_point.t;
      
      double d2D = std::sqrt(std::pow(sW - eW,2) + std::pow(sT - eT,2));
      
      auto f = plane_f_v.at(plane);

      double length = d2D / f;

      length_v[plane] = length;
      opening_angle_v[plane] = clus.opening_angle;
    }
  
    resultShower.fLength = std::max(length_v);
    resultShower.fOpeningAngle = std::max(opening_angle_v);
  
    return;
  }

}
  
#endif
  
