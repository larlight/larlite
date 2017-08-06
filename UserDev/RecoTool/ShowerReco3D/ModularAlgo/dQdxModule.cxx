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

      double dmax2D = _dtrunk * (1 - dir3D[1]*dir3D[1] );

      _pitch = geomHelper->GetPitch(dir3D, (int)pl);
      
      // store summed charge over initial shower trunk
      double Q  = 0.;

      _dmax = 0.;

      _nhits = 0;
      
      // loop through hits and find those within some radial distance of the start point
      
      // loop over hits
      for (auto const &h : hits) {
	
	double d2D = sqrt( pow(h.w - start2D.w, 2) + pow(h.t - start2D.t, 2) );

	if (d2D > dmax2D) continue;

	if (d2D > _dmax) _dmax = d2D;
	
	Q += h.charge;
	
	_nhits += 1;
	
      }// loop over all hits

      _dqdx = Q / (_pitch * _nhits);

      _ntot = hits.size();

      resultShower.fBestdQdxPlane = pl;
      resultShower.fdQdx_v[pl] = _dqdx;
      resultShower.fBestdQdx   = _dqdx;
      
    }// for all clusters (planes)
    
    _tree->Fill();
    
    return;
  }
} //showerreco

#endif
