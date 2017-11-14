#ifndef FILTERSHOWERS_CXX
#define FILTERSHOWERS_CXX

#include "FilterShowers.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

void FilterShowers::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                    Shower_t& resultShower) {

  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()) {
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
    throw ShowerRecoException(ss.str());
  }

  // get the proto-shower 3D vertex
    auto const& vtx3D_v = proto_shower.vertexes();

    // are there many vertices? if more than 1 this algo does not know what to do
    if (vtx3D_v.size() != 1){
      std::cout << "Number of vertices is " << vtx3D_v.size() << " is not one!" << std::endl;
      return;
    }

    // take the vtx -> start point direction as the 3D direction
    auto const& vtx = vtx3D_v[0];

    auto geomH = larutil::GeometryHelper::GetME();

    // project vertex onto this plane
    auto const& vtx2D = geomH->Point_3Dto2D(vtx,2);
  
  auto & clusters = proto_shower.params();

  // get 3D shower direction projected on collection-plane
  double slope3D = resultShower.fDCosStart[0] / resultShower.fDCosStart[2];
  slope3D       /= sqrt( ( resultShower.fDCosStart[0] * resultShower.fDCosStart[0] ) +
			 ( resultShower.fDCosStart[2] * resultShower.fDCosStart[2] ) );
  

  // angle between projected 3D direction on plane (slope3D)
  // and average hit direction
  double clusterhitangle = 0.;
  
  // is there a collection-plane shower?
  bool collectioncluster = false;
  
  for (auto const& clus : clusters) {

    if (clus.plane_id.Plane != 2) continue;

    // get hits and start-point, calculate average angle of hits w.r.t. projected
    // shower direction

    collectioncluster = true;

    // cluster hit closest to vertex is the point from which to compute hit angle
    double sw, st;
    double vtxdmin = 10000000.;

    for (auto const& hit : clus.hit_vector) {
      double dd = ( (hit.w-vtx2D.w)*(hit.w-vtx2D.w) + (hit.t-vtx2D.t)*(hit.t-vtx2D.t) );
      if (dd < vtxdmin) { vtxdmin = dd; sw = hit.w; st = hit.t; }
    }    
    
    // get cluster start point
    //auto start = clus.start_point;

    if (_verbose)
      std::cout << "start pt @ " << sw << ", " << st << std::endl;

    for (auto const& hit : clus.hit_vector) {

      if ( (hit.t == st) || (hit.w == sw) ) {
	if (_verbose)
	  std::cout << "found overlapping hit!" << std::endl;
	continue;
      }

      double hitslope = (hit.t - st) / (hit.w - sw );
      double hitangle = fabs( atan( ( hitslope - slope3D ) / ( 1 + slope3D * hitslope ) ) ); 
      clusterhitangle += hitangle;
      //std::cout << "\t hit @ [ " << hit.w << ", " << hit.t << "] -> [" << (hit.w-sw) << ", " << (hit.t-st) << "] has angle : " << hitangle * 180. / 3.14 << std::endl;
    }

    clusterhitangle /= ( clus.hit_vector.size() - 1);
    clusterhitangle = fabs(clusterhitangle);

  }// for all clusters

  //if the module does not have 2D cluster info -> fail the reconstruction
  if ( collectioncluster == false ) {
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D collection-plane cluster";
    throw ShowerRecoException(ss.str());
  }

  clusterhitangle *= (180./3.14);

  if (_verbose) {
    std::cout << "Hit Angle = " << clusterhitangle << std::endl;
    std::cout << "Opening Angle = " << resultShower.fOpeningAngle * 180 / 3.14 << std::endl;
  }

  //if (clusterhitangle > _anglecut) {
  if ( (clusterhitangle > (resultShower.fOpeningAngle * 180 / 3.14)) and (clusterhitangle > _anglecut) ) {
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " Hit angle w.r.t. 3D dir on Y plane is too large : " << clusterhitangle;
    throw ShowerRecoException(ss.str());
  }
}


} //showerreco

#endif
