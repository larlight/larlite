#ifndef DQDXMODULEUVY_CXX
#define DQDXMODULEUVY_CXX

#include "dQdxModuleUVY.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/LArProperties.h"
#include "LArUtil/DetectorProperties.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "math.h"
#include <algorithm>
#include <functional>
#include <array>
#include <cassert>

namespace showerreco {
  
  dQdxModuleUVY::dQdxModuleUVY()
  {
    _name = "dQdxModuleUVY";
    _dtrunk = kDOUBLE_MAX;
    _use_trunk = false;
  }
  
  void dQdxModuleUVY::initialize()
  {
    return;
  }
  
  void dQdxModuleUVY::do_reconstruction(const ::protoshower::ProtoShower & proto_shower, Shower_t & resultShower) {

    
    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
    }
    
    auto geomHelper = larutil::GeometryHelper::GetME();

    auto& clusters = proto_shower.params();
    
    TVector3 dir3D;

    if(_use_trunk)
      dir3D = resultShower.fDCosTrunk;
    else
      dir3D = resultShower.fDCosStart;
    
    std::array<double,3> plane_f_v;
    std::array<double,3> pitch_v;
    
    for(size_t plane=0; plane<3; ++plane) {
      plane_f_v[plane] = geomHelper->Project_3DLine_OnPlane(dir3D,plane).Mag();
      pitch_v[plane]   = geomHelper->GetPitch(dir3D,plane);
    }
    
    // loop through planes
    for (size_t n = 0; n < clusters.size(); n++) {
      
      // get cluster
      const auto& clus = clusters.at(n);
      
      // get the hits associated with this cluster
      const auto& hits = clus.hit_vector;
      
      // get the plane associated with this cluster
      const auto& pl = clus.plane_id.Plane;
      
      // grab the 2D start point of the cluster
      auto& start2D = clus.start_point;
      
      double f = plane_f_v.at(pl);
      double pitch  = pitch_v[pl];

      assert (_dtrunk != kDOUBLE_MAX);

      std::vector<double> dqdx_v(3 * _dtrunk, 0.);
      
      // loop through hits and find those within some radial distance of the start point
      // loop over hits
      for (const auto &h : hits) {
	
	double d2D = sqrt( pow(h.w - start2D.w, 2) + pow(h.t - start2D.t, 2) );

	double d3D = d2D / f;

	if (d3D >= _dtrunk) continue;

	double qcorr = ChargeCorrection(h.charge,resultShower.fXYZStart);

	dqdx_v[ d3D * 3 ] += qcorr;
	
      } // loop over all hits

      static std::vector<double> dqdx_nonzero_v;
      dqdx_nonzero_v.clear();
      dqdx_nonzero_v.reserve(hits.size());
      for (const auto& dqdx : dqdx_v) {
	if (dqdx) dqdx_nonzero_v.push_back(dqdx); 
      }

      double dqdx = -1.0*larlite::data::kINVALID_DOUBLE;

      if (dqdx_nonzero_v.empty()) dqdx = 0.;
      else {
	std::nth_element(dqdx_nonzero_v.begin(), dqdx_nonzero_v.end(), dqdx_nonzero_v.end() );
	size_t dqdx_idx = (size_t) (((float) dqdx_nonzero_v.size()) / 2.0);
	dqdx  =  dqdx_nonzero_v.at(dqdx_idx);
	dqdx /= pitch;
      }

      resultShower.fBestdQdxPlane = pl;
      resultShower.fdQdx_v[pl]    = dqdx;
      resultShower.fBestdQdx      = -1.0*larlite::data::kINVALID_DOUBLE;
      
    } // for all clusters (planes)
    
    return;
  }

  void dQdxModuleUVY::CreateResponseMap(const double& stepsize) {
    
    _responseMap.clear();
    _responseStep = stepsize;

    auto geom = larutil::Geometry::GetME();
    
    int nx = int((geom->DetHalfWidth() * 2)  / _responseStep) + 1;
    int ny = int((geom->DetHalfHeight() * 2) / _responseStep) + 1;
    int nz = int((geom->DetLength())         / _responseStep) + 1;
    
    _responseMap = std::vector< std::vector< std::vector<double> > >(nx,std::vector< std::vector<double> >(ny, std::vector<double>(nz,-1) ) );
    
    return;
  }

  double dQdxModuleUVY::ChargeCorrection(const double& q, const TVector3& vtx){

    auto geom = larutil::Geometry::GetME();

    // find cells in map
    size_t i = vtx.X() / _responseStep;
    size_t j = (geom->DetHalfHeight() + vtx.Y()) / _responseStep;
    size_t k = vtx.Z() / _responseStep;

    // make sure not out of bounds:
    if ( (i < 0) or (j < 0) or (k < 0) or
	 (i >= _responseMap.size()) or
	 (j >= _responseMap[0].size()) or
	 (k >= _responseMap[0][0].size()) ) {
      return q;
    }

    double corr = _responseMap[i][j][k];

    if (corr <= 0) {
      //std::cout << "No correction available..." << std::endl;
      // find nearest non-empty box
      corr = NearestCell(i,j,k);
    }

    return q * corr;
  }
  
  
  void dQdxModuleUVY::SetResponse(const double& x, const double& y, const double& z, const double& q)
    
  {

    auto geom = larutil::Geometry::GetME();
    
    if ( (x < 0) || (x > 2 * geom->DetHalfWidth()) ) {
      std::cout << "X value out of bounds." << std::endl;
      return;
    }

    if ( (y < -geom->DetHalfHeight()) || (y > geom->DetHalfHeight()) ) {
      std::cout << "y value out of bounds." << std::endl;
      return;
    }

    if ( (z < 0) || (z > geom->DetLength()) ) {
      std::cout << "X value out of bounds." << std::endl;
      return;
    }

    // find vector position
    size_t i = x / _responseStep;
    size_t j = (geom->DetHalfHeight() + y) / _responseStep;
    size_t k = z / _responseStep;

    //std::cout << "x, y, z " << x << ", " << y << ", " << z << "\t i, j, k " << i << ", " << j << ", "  << k << std::endl;
    
    _responseMap[i][j][k] = q;

    return;
  }

  double dQdxModuleUVY::NearestCell(const size_t& i, const size_t& j, const size_t& k) {

    double val;
    int    ctr;

    std::vector<int> cellspan;

    for (int cellsize = 1; cellsize < 4; cellsize++) {

      val = 0;
      ctr = 0;

      cellspan.clear();
      for (int n=1; n < cellsize; n++){
	cellspan.push_back(-(n+1));
	cellspan.push_back(n+1);
      }
      
      for (const auto& xd : cellspan) {
	for (const auto& yd : cellspan) {
	  for (const auto& zd : cellspan) {
	    
	    if ( (i+xd >= 0) and (j+yd > 0) and (k+zd > 0) and
		 (i+xd < _responseMap.size()) and
		 (j+yd < _responseMap[0].size()) and
		 (k+zd < _responseMap[0][0].size()) ) {
	      
	      if ( _responseMap[i+xd][j+yd][k+zd] > 0){
		val += _responseMap[i+xd][j+yd][k+zd];
		ctr += 1;
	      }
	      
	    }// if not out of bounds
	    
	  }// z loop
	}// y loop
      }// x loop

      if (val != 0) {
	val /= ctr;
	//std::cout << "val is " << val << std::endl;
	return val;
      }
      
    }// for all cell-sizes

    // if we never found non-zero correction, return 1
    //std::cout << "no neighboring correction found..return 1" << std::endl;
    return 1;
  }

  
} //showerreco

#endif
