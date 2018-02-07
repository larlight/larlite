#ifndef TRUNKANGLE3DFROMVERTEXQWEIGHTED_CXX
#define TRUNKANGLE3DFROMVERTEXQWEIGHTED_CXX

#include "TrunkAngle3DFromVtxQweighted.h"
#include "LArUtil/GeometryHelper.h"
#include "Base/DataFormatConstants.h"
#include <math.h>
#include <sstream>
#include <algorithm>
#include <cassert>

namespace showerreco {
  
  TrunkAngle3DFromVtxQweighted::TrunkAngle3DFromVtxQweighted() {
    _name = "TrunkAngle3DFromVtxQweighted";
    _dtrunk = kDOUBLE_MAX;
  }

  void TrunkAngle3DFromVtxQweighted::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
						       Shower_t& resultShower) {

    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasVertex()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing Vertex";
      throw ShowerRecoException(ss.str());
    }

    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
    }

    auto geomH = larutil::GeometryHelper::GetME();
    
    // get the proto-shower 3D vertex
    auto const& vtx3D_v = proto_shower.vertexes();

    // are there many vertices? if more than 1 this algo does not know what to do
    if (vtx3D_v.size() != 1){
      std::cout << "Number of vertices is " << vtx3D_v.size() << " is not one!" << std::endl;
      return;
    }

    // take the vtx for the neutrino interaction
    auto const& vtx = vtx3D_v[0];

    std::vector<double> dir3D = {0,0,0};

    auto & clusters = proto_shower.params();

    // planes with largest number of hits used to get 3D direction
    std::vector<int> planeHits(3,0);
    std::vector<larutil::Point2D> planeDir(3);
    
    // we want an energy for each plane
    for (size_t n = 0; n < clusters.size(); n++) {
      
      // get the hits associated with this cluster
      auto const& hits = clusters.at(n).hit_vector;
      
      // get the plane associated with this cluster
      auto const& pl = clusters.at(n).plane_id.Plane;

      // grab the 2D start point of the cluster
      auto& start2D = clusters.at(n).start_point;

      // get the charge-averaged 2D vector pointing from vtx in shower direction
      larutil::Point2D weightedDir;
      weightedDir.w = 0;
      weightedDir.t = 0;
      double Qtot = 0;
      int nhits = 0;
      //for (auto const& hit : hits){

      assert(_dtrunk != kDOUBLE_MAX);

      for(size_t hid=0; hid<hits.size(); ++hid) {
	const auto& hit = hits.at(hid);

	double d2D = sqrt( pow(hit.w - start2D.w, 2) + pow(hit.t - start2D.t, 2) );

	if (d2D > _dtrunk) continue;

	//std::cout << "@hid=" << hid << " hit=(" << hit.w << "," << hit.t << ") and d2D=" << d2D << std::endl;
	
	weightedDir.w += (hit.w - start2D.w) * hit.charge;
	weightedDir.t += (hit.t - start2D.t) * hit.charge;

	Qtot += hit.charge;
	nhits += 1;
      }
      
      //std::cout << std::endl;
      //std::cout << "@plane=" << pl << " Qtot=" << Qtot << " nhits=" << nhits << std::endl;
      //std::cout << std::endl;

      weightedDir.w /= Qtot;
      weightedDir.t /= Qtot;

      planeHits[pl] = nhits;
      planeDir[pl]  = weightedDir;
      
    }// for all planes

    int pl_max = larlite::data::kINVALID_INT;
    int pl_mid = larlite::data::kINVALID_INT;
    int pl_min = larlite::data::kINVALID_INT;

    int n_max  = -1.0*larlite::data::kINVALID_INT;
    int n_min  =      larlite::data::kINVALID_INT;

    for (size_t pl=0; pl < planeHits.size(); pl++){
      if (planeHits[pl] > n_max){
	pl_max = pl;
	n_max  = planeHits[pl];
      }
      if (planeHits[pl] < n_min){
	pl_min = pl;
	n_min  = planeHits[pl];
      }
    }

    assert(pl_max != larlite::data::kINVALID_INT);
    assert(pl_min != larlite::data::kINVALID_INT);

    // find the medium plane
    for(int pp=0; pp<3; ++pp) {
      if (pp == pl_max) continue;
      if (pp == pl_min) continue;
      pl_mid = pp;
    }

    assert(pl_mid != larlite::data::kINVALID_INT);

    double slope_max, slope_mid;
    double angle_max, angle_mid;
    slope_max = planeDir[pl_max].t / planeDir[pl_max].w;
    angle_max = atan(slope_max);
    angle_max = atan2( planeDir[pl_max].t , planeDir[pl_max].w );
    slope_mid = planeDir[pl_mid].t / planeDir[pl_mid].w;
    angle_mid = atan(slope_mid);
    angle_mid = atan2( planeDir[pl_mid].t , planeDir[pl_mid].w );
    
    double theta, phi;
    geomH->Get3DAxisN(pl_max, pl_mid,
		      angle_max, angle_mid,
		      phi, theta);
    
    resultShower.fDCosTrunk[1] = sin(theta);
    resultShower.fDCosTrunk[0] = cos(theta) * sin(phi);
    resultShower.fDCosTrunk[2] = cos(theta) * cos(phi);
    
    return;
  }
  
  
} //showerreco

#endif
