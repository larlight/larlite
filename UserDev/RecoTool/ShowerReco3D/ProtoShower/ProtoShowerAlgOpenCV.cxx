#ifndef PROTOSHOWERALGOPENCV_CXX
#define PROTOSHOWERALGOPENCV_CXX

#include "ProtoShowerAlgOpenCV.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"
#include "TVector3.h"
#include <math.h>

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/seed.h"
#include "DataFormat/vertex.h"

namespace showerreco {
  
  ProtoShowerAlgOpenCV::ProtoShowerAlgOpenCV()
    : _params_alg(nullptr)
  {
    // if no algo for calculating cluster params -> set up default ones
    if (!_params_alg) {
      _params_alg = new ::cluster::DefaultParamsAlg();
    }
  }

  void ProtoShowerAlgOpenCV::GenerateProtoShower(::larlite::storage_manager* storage,
						 ::larlite::event_pfpart* ev_pfpart,
						 const size_t proto_shower_pfpart,
						 showerreco::ProtoShower & proto_shower)
  {

    // larlite::cluster data-products in wire-time
    // need to convert them to cm space
    auto geom    = ::larutil::Geometry::GetME();
    auto geomH   = ::larutil::GeometryHelper::GetME();
    auto detProp = ::larutil::DetectorProperties::GetME();

    // For each data product, fetch it and feed
    // it into the proto showers if possible
    //
    // That means using cluster_params and cluster3D_params if needed
    
    // Try to get the clusters if they are there
    ::larlite::event_cluster * ev_clust = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());
    
    // Want to get the hits from the cluster too:
    ::larlite::event_hit * ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());

    // prepare a vector of clusters
    std::vector<::larlite::cluster> cluster_v;
    // prepare a vector of vector of hits
    // 1st index is the cluster index
    // then each subvector contains the larlite hits for that cluster
    std::vector< std::vector< ::larlite::hit> > cluster_hits_v_v;
    // if there are any clusters:
    if ( ev_clust && ev_clust->size() != 0) {
      for (auto j_clust : ass_cluster_v.at(proto_shower_pfpart)) {
	cluster_v.push_back( ev_clust->at( j_clust ) );
	std::vector< ::larlite::hit > cluster_hits_v;
	auto const& ass_hits = ass_hit_v.at(j_clust);
	for (auto const&  hit_idx : ass_hits)
	  cluster_hits_v.push_back( ev_hit->at(hit_idx) );
	cluster_hits_v_v.push_back( cluster_hits_v );
      }// for all clusters associated to the PFParticle
    }// if clusters were found
    // if no clusters for this PFParticle -> no 2D information
    proto_shower.hasCluster2D(true);
    if (cluster_v.size() == 0)
      proto_shower.hasCluster2D(false);
    
    // resize _params according to number of clusters
    proto_shower._params.resize( cluster_v.size() );
    
    // fill 2D information, if available
    for (size_t i=0; i < cluster_v.size(); i++){
      _cru_helper.GenerateParams( cluster_hits_v_v[i], proto_shower._params.at( i ) );
      _params_alg->FillParams( proto_shower._params.at( i ) );

      // now fill quantities specifically to LArOpenCV
      auto const& clus = cluster_v[i];
      
      double * origin;
      origin = new double[3];
      geom->PlaneOriginVtx( clus.Plane().Plane, origin);
      float planeOffset = origin[0];


      auto const& sw = clus.StartWire() * geomH->WireToCm();
      auto const& ew = clus.EndWire()   * geomH->WireToCm();
      auto const& st = ( clus.StartTick() - detProp->TriggerOffset() ) * geomH->TimeToCm() + planeOffset;
      auto const& et = ( clus.EndTick()   - detProp->TriggerOffset() )  * geomH->TimeToCm() + planeOffset;

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

    }// for all input clusters

    // get vertex associated to cluster
    ::larlite::event_vertex * ev_vtx = nullptr;
    auto const& ass_vtx_v = storage->find_one_ass(ev_clust->id(), ev_vtx, ev_clust->name());

    // fill vertex information
    if (!ev_vtx or (ev_vtx->size() == 0) )
      proto_shower.hasVertex(false);
    else{
      for (auto j_clust : ass_cluster_v.at(proto_shower_pfpart)) {
	// associated vtx indices for this cluster
	auto const& vtx_idx_v = ass_vtx_v[ j_clust ];
	if (vtx_idx_v.size() != 0){
	  for (auto const& vtx_idx : vtx_idx_v){
	    auto const& vtx = ev_vtx->at(vtx_idx);
	    double xyz[3];
	    vtx.XYZ(xyz);
	    proto_shower._vertexes.push_back(TVector3(xyz[0],xyz[1],xyz[2]));
	  }// for all associated vertices
	}// if there are associated vertices
      }// for all clusters
    }// if there are vertices
      

    return;
  }
  

}// namespace

#endif
