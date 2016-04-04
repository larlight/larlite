#ifndef PROTOSHOWERALGOPENCV_CXX
#define PROTOSHOWERALGOPENCV_CXX

#include "ProtoShowerAlgOpenCV.h"
#include "LArUtil/GeometryHelper.h"
#include "TVector3.h"
#include <math.h>

namespace showerreco {

  ProtoShowerAlgOpenCV::ProtoShowerAlgOpenCV() {
  }

  void ProtoShowerAlgOpenCV::GenerateProtoShower(const std::vector<::larlite::cluster> &clus_v,
						 const std::vector<std::vector<::larlite::hit> > &hit_v,
						 const std::vector<::larlite::spacepoint> &sps_v,
						 const std::vector<::larlite::vertex> &vtx_v,
						 showerreco::ProtoShower & proto_shower)
  {
    
    
    proto_shower.hasCluster2D(true);
    if (clus_v.size() == 0)
      proto_shower.hasCluster2D(false);

    // resize _params according to number of clusters
    proto_shower._params.resize( clus_v.size() );


    // get cluster data-product information from which to fill ClusterParams

    // larlite::cluster data-products in wire-time
    // need to convert them to cm space
    auto geomH = ::larutil::GeometryHelper::GetME();

    for (size_t i=0; i < clus_v.size(); i++){

      auto const& clus = clus_v[i];

      auto const& sw = clus.StartWire() * geomH->WireToCm();
      auto const& ew = clus.EndWire()   * geomH->WireToCm();
      auto const& st = clus.StartTick() * geomH->TimeToCm();
      auto const& et = clus.EndTick()   * geomH->TimeToCm();

      // start point
      proto_shower._params.at(i).start_point.w = sw;
      proto_shower._params.at(i).start_point.t = st;
      // end point
      proto_shower._params.at(i).end_point.w = ew;
      proto_shower._params.at(i).end_point.t = et;

      proto_shower._params.at(i).slope_2d = (et-st)/(ew-sw);
      // angle calculated in the same way as in ClusterParams, as the
      // arc-tangent of the slope
      proto_shower._params.at(i).angle_2d = atan( (et-st)/(ew-sw) );
      
    }// for all clusters      

    return;
  }
  

}// namespace

#endif
