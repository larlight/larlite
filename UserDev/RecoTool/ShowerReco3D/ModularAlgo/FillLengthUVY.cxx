#ifndef FILLLENGTHUVY_CXX
#define FILLLENGTHUVY_CXX

#include "FillLengthUVY.h"
#include "LArUtil/GeometryHelper.h"

#include <algorithm>


namespace showerreco {

  FillLengthUVY::FillLengthUVY()
  {
    _name = "FillLengthUVY";
    _qfraction = 0.8;
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

    std::array<TVector2,3> line_dir_v;

    for(auto& v : length_v) 
      v = -1.0*::larlite::data::kINVALID_DOUBLE;
    
    opening_angle_v = length_v;
    plane_f_v = length_v;

    const auto& vertex = proto_shower.vertexes().front();
    
    float alpha = 10;
    
    for(size_t plane=0; plane<3; ++plane) {
      plane_f_v[plane] = geomHelper->Project_3DLine_OnPlane(dir3D, plane).Mag();
      
      auto startPoint2D = geomHelper->Point_3Dto2D(vertex, plane);
      auto secondPoint3D = vertex + alpha * dir3D;
      auto secondPoint2D = geomHelper->Point_3Dto2D(secondPoint3D, plane);
      TVector2 dir(secondPoint2D.w - startPoint2D.w, secondPoint2D.t - startPoint2D.t);
      dir *= 1.0 / dir.Mod();
      line_dir_v[plane] = dir;
    }

    for (auto const& clus : clusters) {
      size_t plane = clus.plane_id.Plane;
      
      double sW  = clus.start_point.w;
      double sT  = clus.start_point.t;

      //
      // loop over hits and find the total charge on this plane
      // project the hit onto the 2D line and calculate the 
      // projected distance
      //
      
      auto const& hits = clus.hit_vector;

      float qsum = 0;
      float d2D  = 0;
      std::vector<std::pair<float,float>> dist_v(hits.size());

      for(const auto& hit : hits) qsum += hit.charge;

      const auto& dr_w = line_dir_v[plane].X();
      const auto& dr_t = line_dir_v[plane].Y();

      for(size_t hid = 0; hid < hits.size(); ++hid) {
	const auto& hit = hits[hid];

	float ptw = hit.w - sW;
	float ptt = hit.t - sT;

	d2D  = ptw * dr_w;
	d2D += ptt * dr_t;

	dist_v[hid] = std::make_pair(d2D,hit.charge / qsum);
      }
      std::sort(std::begin(dist_v),std::end(dist_v),
		[](const std::pair<float,float>& lhs, const std::pair<float,float>& rhs)
		{ return lhs.first < rhs.first; });

      qsum = 0;
      d2D = 0;
      for(const auto& dist_pair : dist_v) {
	d2D   = dist_pair.first;
	qsum += dist_pair.second;
	if (qsum>_qfraction) break;
      }
      
      auto f = plane_f_v.at(plane);

      double length = d2D / f;

      length_v[plane] = length;
      opening_angle_v[plane] = clus.opening_angle;
    }
  
    resultShower.fLength = *std::max_element(std::begin(length_v),std::end(length_v));
    resultShower.fOpeningAngle = *std::max_element(std::begin(opening_angle_v),std::end(opening_angle_v));
  
    return;
  }

}
  
#endif
  
