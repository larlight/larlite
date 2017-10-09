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
    auto ev_vertex = storage->get_data<::larlite::event_vertex>(_vtx_producer);

    std::array<double,3> xyz;
    std::array<larutil::Point2D, 3> xz_v;

    if (ev_vertex and (ev_vertex->size() == 1) ) {
      proto_shower.hasVertex(true);
      auto const& vtx = ev_vertex->front(); // only 1 vertex @ a time?
      vtx.XYZ(xyz.data());
      proto_shower._vertexes.push_back( TVector3(xyz[0],xyz[1],xyz[2]) );

      for(size_t plane=0; plane<3; ++plane) {
	auto pt = geomH->Point_3Dto2D(xyz.data(),plane);
	xz_v[plane] = pt;
      }

    }// if there is vertex info
    else {
      if (_debug) std::cout << "No vertex @ found, SKIP!" << std::endl;
      return;
    }

    // Try to get the clusters if they are there
    ::larlite::event_cluster * ev_clust = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());
    
    std::cout << "GOT: " << ass_cluster_v.size() << " associated clusters" << std::endl;
    if (ev_clust and (!ev_clust->empty()) ) {

      // Want to get the hits from the cluster too:
      ::larlite::event_hit * ev_hit = nullptr;
      auto const& ass_hit_v = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());

      if (ev_hit and (! ev_hit->empty()) ) {

	// prepare a vector of clusters
	std::vector<const larlite::cluster*> cluster_v;
	// prepare a vector of vector of hits
	// 1st index is the cluster index
	// then each subvector contains the larlite hits for that cluster
	std::vector< std::vector< const larlite::hit*> > cluster_hits_vv;

	// if there are any clusters:
	if ( ev_clust && ev_clust->size() != 0) {
	  for (auto j_clust : ass_cluster_v.at(proto_shower_pfpart)) {
	    cluster_v.push_back( &ev_clust->at( j_clust ) );
	    std::vector< const larlite::hit* > cluster_hits_v;
	    auto const& ass_hits = ass_hit_v.at(j_clust);
	    for (auto const&  hit_idx : ass_hits) {
	      cluster_hits_v.push_back( &ev_hit->at(hit_idx) );
	    }
	    cluster_hits_vv.emplace_back( std::move(cluster_hits_v) );
	  }// for all clusters associated to the PFParticle
	}// if clusters were found
	// if no clusters for this PFParticle -> no 2D information

	proto_shower.hasCluster2D(true);

	if (cluster_v.empty()) {
	  proto_shower.hasCluster2D(false);
	}

	
	size_t n_filled_cluster = 0;
	std::vector<bool> valid_cluster_v(cluster_v.size(),false);
	// valid_cluster_v.resize(cluster_v.size());
	// for(auto& v : valid_cluster_v) v = false;

	for (size_t i = 0; i < cluster_v.size(); i++) {
	  if (!cluster_hits_vv[i].empty()) {
	    n_filled_cluster += 1;
	    valid_cluster_v[i] = true;
	  }
	}

	// resize _params according to number of clusters
	proto_shower._params.resize( n_filled_cluster );

	int proto_idx = -1;
	// fill 2D information, if available
	for (size_t i = 0; i < cluster_v.size(); i++) {

	  if (!valid_cluster_v[i]) continue;
	  
	  proto_idx += 1;
	  
	  assert (proto_idx < proto_shower._params.size());

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

	}// for all input clusters


      }// if there are associated hits
      else {
	if (_debug) std::cout << "No associated hits!" << std::endl;
	proto_shower.hasCluster2D(false);
      }
    }// if there are associated clusters
    else { 
      if (_debug) std::cout << "No associated clusters!" << std::endl;
      proto_shower.hasCluster2D(false);
    }

    
    return;
  }
  
  
}// namespace

#endif
