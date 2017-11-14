#ifndef PROTOSHOWERALGDL_CXX
#define PROTOSHOWERALGDL_CXX

#include "ProtoShowerAlgDL.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"
#include "TVector3.h"
#include <math.h>
#include <array>
#include <limits>
#include <cassert>

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/vertex.h"

#include "ShowerReco3D/Base/ShowerRecoException.h"

namespace protoshower {

  ProtoShowerAlgDL::ProtoShowerAlgDL()
    : _params_alg(nullptr)
  {
    // if no algo for calculating cluster params -> set up default ones
    if (!_params_alg) {
      _params_alg = new ::cluster::DefaultParamsAlg();
    }
    _name = "ProtoShowerAlgDL";
    _debug = false;
  }

  void ProtoShowerAlgDL::GenerateProtoShower(::larlite::storage_manager* storage,
					     ::larlite::event_pfpart* ev_pfpart,
					     const size_t proto_shower_pfpart,
					     protoshower::ProtoShower & proto_shower)
  {

    // larlite::cluster data-products in wire-time
    // need to convert them to cm space
    auto geom    = larutil::Geometry::GetME();
    auto geomH   = larutil::GeometryHelper::GetME();
    auto detProp = larutil::DetectorProperties::GetME();

    // For each data product, fetch it and feed
    // it into the proto showers if possible
    //
    // That means using cluster_params and cluster3D_params if needed

    // Do the vertex:
    larlite::event_vertex *ev_vertex = nullptr;
    auto const& ass_vertex_v = storage->find_one_ass(ev_pfpart->id(), ev_vertex, ev_pfpart->name());

    std::cout << "@pfpart=" << proto_shower_pfpart << std::endl;
    std::cout << "ev_vertex size: " << ev_vertex->size() << std::endl;

    if (!ev_vertex) {
      std::cout << "No vertex found?" << std::endl;
      throw std::exception();
      return;
    }

    // get the associated vertex
    std::array<double,3> xyz;
    std::array<larutil::Point2D, 3> xz_v;

    proto_shower.hasVertex(true);
    auto vtxid_v = ass_vertex_v.at(proto_shower_pfpart);
    assert(vtxid_v.size()==1);
    auto const& vtx = ev_vertex->at(vtxid_v.front());
    vtx.XYZ(xyz.data());
    proto_shower._vertexes.push_back( TVector3(xyz[0],xyz[1],xyz[2]) );

    std::cout << "@vertex=(" << xyz[0] << "," << xyz[1] << "," << xyz[2] << ")" << std::endl;
    for(size_t plane=0; plane<3; ++plane) {
      auto pt = geomH->Point_3Dto2D(xyz.data(),plane);
      xz_v[plane] = pt;
      std::cout << "@plane=" << plane << " : (" << pt.w << "," << pt.t << ")" << std::endl;
    }

    ::larlite::event_cluster * ev_clust = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());

    std::cout << "GOT: " << ev_clust->size() << " event clusters" << std::endl;
    
    ::larlite::event_hit * ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());

    std::cout << "GOT: " << ev_hit->size() << " event hits" << std::endl;

    std::vector<const larlite::cluster*> cluster_v;
    std::vector< std::vector< const larlite::hit*> > cluster_hits_vv;

    cluster_v.reserve(ev_clust->size());
    cluster_hits_vv.reserve(ev_clust->size());

    for (auto j_clust : ass_cluster_v.at(proto_shower_pfpart)) {
      const auto& clust = ev_clust->at(j_clust);
      cluster_v.push_back(&clust);
      cluster_hits_vv.resize(cluster_hits_vv.size()+1);
      auto& cluster_hits_v = cluster_hits_vv.back();
      
      for (auto const&  hit_idx : ass_hit_v.at(j_clust)) {
	cluster_hits_v.push_back( &ev_hit->at(hit_idx) );
      }
    }
    
    if (cluster_v.empty()) {
      std::cout << "...no cluster2d!!!!" << std::endl;
      std::cout << std::endl;
      proto_shower.hasCluster2D(false);
      return;
    }

    std::cout << "number of clusters = " << cluster_v.size() << std::endl;
    
    size_t n_filled_cluster = 0;
    std::vector<bool> valid_cluster_v(cluster_v.size(),false);

    for (size_t i = 0; i < cluster_v.size(); i++) {
      if (!cluster_hits_vv[i].empty()) {
	n_filled_cluster += 1;
	valid_cluster_v[i] = true;
      }
    }

    if (!n_filled_cluster) {
      std::cout << "...clusters exists but all empty" << std::endl;
      std::cout << std::endl;
      proto_shower.hasCluster2D(false);
      return;
    }

    // resize _params according to number of clusters
    proto_shower._params.resize( n_filled_cluster );

    int proto_idx = -1;
    // fill 2D information, if available
    for (size_t i = 0; i < cluster_v.size(); i++) {

      if (!valid_cluster_v[i]) continue;

      proto_shower.hasCluster2D(true);
	  
      proto_idx += 1;
	  
      assert ((size_t)proto_idx < proto_shower._params.size());

      _cru_helper.GenerateParams( cluster_hits_vv[i], proto_shower._params.at( proto_idx ) );
      _params_alg->FillParams( proto_shower._params.at( proto_idx ) );

      // now fill quantities specifically to LArDL
      auto const& clus = cluster_v[i];

      double* origin = new double[3];
      assert ((int)clus->Plane().Plane >= 0);
      assert ((int)clus->Plane().Plane <= 2);

      geom->PlaneOriginVtx( clus->Plane().Plane, origin);
      float planeOffset = origin[0];

      //
      // Set the start&end wire&tick using vertex
      //

      const larlite::hit* start_hit = nullptr;
      double min_dr = std::numeric_limits<double>::max();

      size_t plane = clus->Plane().Plane;
	    
      // get the closest hit to the 2D vertex
      for(const auto hit : cluster_hits_vv[i]) {
	    
	double x  = hit->WireID().Wire;
	double dx = xz_v.at(plane).w;
	dx -= x;
	auto dx2 = dx*dx;
	    
	double y  = hit->PeakTime();
	double dy = xz_v.at(plane).t;
	dy -= y;
	auto dy2 = dy*dy;
	    
	auto dr = std::sqrt(dx2 + dy2);
	      
	if (dr < min_dr) {
	  min_dr = dr;
	  start_hit = hit;
	}
      }

      if (!start_hit) 
	throw showerreco::ShowerRecoException("Closest hit to vertex fould not be found");

      const larlite::hit* end_hit   = nullptr;
      double max_dr = std::numeric_limits<double>::lowest();
	  
      for(const auto hit : cluster_hits_vv[i]) {
	double x  = hit->WireID().Wire;
	double dx = start_hit->WireID().Wire;
	dx -= x;
	auto dx2 = dx*dx;
	    
	double y  = hit->PeakTime();
	double dy = start_hit->PeakTime();
	dy -= y;
	auto dy2 = dy*dy;
	    
	auto dr = std::sqrt(dx2 + dy2);
	      
	if (dr > max_dr) {
	  max_dr = dr;
	  end_hit = hit;
	}

      }

      if (!end_hit) 
	throw showerreco::ShowerRecoException("Farthest hit from vertex could not be found");

      auto const& sw = (start_hit->WireID().Wire) * geomH->WireToCm();
      auto const& ew = (end_hit->WireID().Wire)   * geomH->WireToCm();
      auto const& st = ( start_hit->PeakTime() - detProp->TriggerOffset() ) * geomH->TimeToCm() + planeOffset;
      auto const& et = ( end_hit->PeakTime()   - detProp->TriggerOffset() ) * geomH->TimeToCm() + planeOffset;

      // start point
      proto_shower._params.at(proto_idx).start_point.w = sw;
      proto_shower._params.at(proto_idx).start_point.t = st;

      // end point
      proto_shower._params.at(proto_idx).end_point.w = ew;
      proto_shower._params.at(proto_idx).end_point.t = et;

      // is this really needed?
      proto_shower._params.at(proto_idx).slope_2d = (et - st) / (ew - sw);

      // angle calculated in the same way as in ClusterParams, as the
      // arc-tangent of the slope
      proto_shower._params.at(proto_idx).angle_2d = clus->StartAngle();

    }
    
    return;
  }
  
  
}// namespace

#endif
