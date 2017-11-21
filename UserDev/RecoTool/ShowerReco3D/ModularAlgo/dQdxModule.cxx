#ifndef DQDXMODULE_CXX
#define DQDXMODULE_CXX

#include "dQdxModule.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/LArProperties.h"
#include "LArUtil/DetectorProperties.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "math.h"
#include <algorithm>
#include <functional>

namespace showerreco {
  
  dQdxModule::dQdxModule()
  {
    _name = "dQdxModule";
    _tree = nullptr;
    _dtrunk = 0.;
  }
  
  void dQdxModule::initialize()
  {
    
    // degbugging tree
    if (_tree) delete _tree;
    _tree = new TTree(_name.c_str(),"dQdx tree");
    _tree->Branch("_pitch",&_pitch,"pitch/D");
    _tree->Branch("_dqdx",&_dqdx,"dqdx/D");
    _tree->Branch("_dqdx_v","std::vector<double>",&_dqdx_v);
    _tree->Branch("_dmax",&_dmax,"dmax/D");
    _tree->Branch("_nhits",&_nhits,"nhits/I");
    _tree->Branch("_ntot",&_ntot,"ntot/I");
    return;
  }
  
  void dQdxModule::do_reconstruction(const ::protoshower::ProtoShower & proto_shower, Shower_t & resultShower) {
    
    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
    }
    
    auto& clusters = proto_shower.params();
    
    // grab shower direction
    auto const& dir3D = resultShower.fDCosStart;

    auto geomHelper = larutil::GeometryHelper::GetME();
    
    // is plane 2 info available?
    bool CollPlane = false;
    
    // loop through planes
    for (size_t n = 0; n < clusters.size(); n++) {
      
      auto const& clus = clusters.at(n);
      
      // get the hits associated with this cluster
      auto const& hits = clus.hit_vector;
      
      // get the plane associated with this cluster
      auto const& pl = clus.plane_id.Plane;
      
      if (pl != 2) continue;
      
      CollPlane = true;
      
      // grab the 2D start point of the cluster
      auto& start2D = clus.start_point;
      
      // grab the pitch for this plane
      //_pitch = geomHelper->GetPitch(dir3D, (int)pl);

      double f = (1 - dir3D[1]*dir3D[1] );
      
      double dmax2D = _dtrunk * (1 - dir3D[1]*dir3D[1] );

      _pitch = geomHelper->GetPitch(dir3D, (int)pl);
      
      _dmax = 0;

      _nhits = 0;
      
      _dqdx_v.clear();

      _dqdx_v = std::vector<double>(3 * _dtrunk, 0.);
      
      // loop through hits and find those within some radial distance of the start point
      
      // loop over hits
      for (auto const &h : hits) {
	
	double d2D = std::sqrt( std::pow(h.w - start2D.w, 2) + std::pow(h.t - start2D.t, 2) );

	/* OLD
	if (d2D > dmax2D) continue;

	if (d2D > _dmax) _dmax = d2D;
	
	_dqdx_v.push_back( h.charge );
	*/
	
	// NEW
	double d3D = d2D / f;

	if (d3D >= _dtrunk) continue;

	double qcorr = ChargeCorrection(h.charge,resultShower.fXYZStart);
	

	_dqdx_v[ d3D * 3 ] += qcorr;
	
	_nhits += 1;
	
      }// loop over all hits

      std::vector<double> _dqdx_nonzero_v;
      for (auto const& dqdx : _dqdx_v)
	if (dqdx != 0) { _dqdx_nonzero_v.push_back(dqdx); }
      
      /* OLD
      std::nth_element(_dqdx_v.begin(), _dqdx_v.end(), _dqdx_v.end() );
      _dqdx = _dqdx_v[_dqdx_v.size()/2.] / _pitch;
      */

      if (_dqdx_nonzero_v.size() == 0)
	_dqdx = 0.;

      else {
	std::nth_element(_dqdx_nonzero_v.begin(), _dqdx_nonzero_v.end(), _dqdx_nonzero_v.end() );
	_dqdx = _dqdx_nonzero_v[ _dqdx_nonzero_v.size()/2.] / _pitch;
      }

      _ntot = hits.size();

      resultShower.fBestdQdxPlane = pl;
      resultShower.fdQdx_v[pl] = _dqdx;
      resultShower.fBestdQdx   = _dqdx;
      
    }// for all clusters (planes)
    
    _tree->Fill();
    
    return;
  }

  void dQdxModule::CreateResponseMap(const double& stepsize) {
    
    _responseMap.clear();
    _responseStep = stepsize;

    auto geom = larutil::Geometry::GetME();
    
    int nx = int((geom->DetHalfWidth() * 2)  / _responseStep) + 1;
    int ny = int((geom->DetHalfHeight() * 2) / _responseStep) + 1;
    int nz = int((geom->DetLength())         / _responseStep) + 1;
    
    _responseMap = std::vector< std::vector< std::vector<double> > >(nx,std::vector< std::vector<double> >(ny, std::vector<double>(nz,-1) ) );
    
    return;
  }

  double dQdxModule::ChargeCorrection(const double& q, const TVector3& vtx){

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
  
  
  void dQdxModule::SetResponse(const double& x, const double& y, const double& z, const double& q)
    
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

  double dQdxModule::NearestCell(const size_t& i, const size_t& j, const size_t& k) {

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
      
      for (auto const& xd : cellspan) {
	for (auto const& yd : cellspan) {
	  for (auto const& zd : cellspan) {
	    
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
