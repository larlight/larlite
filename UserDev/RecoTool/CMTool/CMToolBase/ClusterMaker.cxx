#ifndef CLUSTERMAKER_CXX
#define CLUSTERMAKER_CXX

#include "ClusterMaker.h"

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

namespace cluster {

  ClusterMaker::ClusterMaker() {

    _vtx_w_cm = {0.,0.,0.};
    _vtx_t_cm = {0.,0.,0.};

    _wire2cm  = larutil::GeometryHelper::GetME()->WireToCm();
    _time2cm  = larutil::GeometryHelper::GetME()->TimeToCm();
    
  }

  void ClusterMaker::MakeClusters(::larlite::storage_manager* storage,
				  const std::string& clus_producer,
				  const std::string& vtx_producer,
				  std::vector<cluster::Cluster>& cluster) {

    // load input clusters
    auto ev_clus = storage->get_data<::larlite::event_cluster>(clus_producer);

    // load associated hits
    larlite::event_hit *ev_hit = nullptr;
    auto const& ass_cluster_hit_v = storage->find_one_ass(ev_clus->id(), ev_hit, ev_clus->name());

    MakeClusters(storage, ass_cluster_hit_v, clus_producer, vtx_producer, cluster);
    
    return;
  }

  void ClusterMaker::MakeClusters(::larlite::storage_manager* storage,
				  const std::vector<std::vector<unsigned int> >& clus_hit_indexes,
				  const std::string& clus_producer,
				  const std::string& vtx_producer,
				  std::vector<cluster::Cluster>& cluster) {

    // load input clusters
    auto ev_clus = storage->get_data<::larlite::event_cluster>(clus_producer);
    // load vertex
    auto ev_vtx  = storage->get_data<::larlite::event_vertex>(vtx_producer);
    // load associated hits
    larlite::event_hit *ev_hit = nullptr;
    auto const& ass_cluster_hit_v = storage->find_one_ass(ev_clus->id(), ev_hit, ev_clus->name());

    if ( (!ev_clus) || (!ev_hit) ){
      std::cout << "NO CLUSTERS" << std::endl;
      return;
    }
    
    if (loadVertex(ev_vtx) == false) {
      std::cout << "NO VERTEX" << std::endl;
      return;
    }

    cluster.clear();
    cluster.reserve(ass_cluster_hit_v.size());

    for (auto const& hit_idx_v : clus_hit_indexes) {

      ::cluster::Cluster clus;

      std::vector<::cluster::pt> cluster_pts;

      auto const& pl = ev_hit->at(hit_idx_v[0]).WireID().Plane;

      std::vector<::cluster::pt> pts;
      
      GetClusterPts(ev_hit, hit_idx_v, pts);
      
      clus.SetHits(pts);

      cluster.push_back(clus);
      
    }// for all input clusters

  }

  void ClusterMaker::GetClusterPts(const ::larlite::event_hit* ev_hit,
				   const std::vector<unsigned int>& hit_idx_v,
				   std::vector<::cluster::pt>& pt_v) {

    pt_v.clear();
    pt_v.reserve(hit_idx_v.size());

    for (auto const& hit_idx : hit_idx_v) {

      auto const& hit = ev_hit->at(hit_idx);

      auto pl = hit.WireID().Plane;

      float  q = hit.Integral();

      float hw = hit.WireID().Wire * _wire2cm;
      float ht = hit.PeakTime()    * _time2cm;
      
      float r = sqrt( (ht - _vtx_t_cm[pl]) * (ht - _vtx_t_cm[pl]) +
		      (hw - _vtx_w_cm[pl]) * (hw - _vtx_w_cm[pl]) );

      float cos_theta = (hw - _vtx_w_cm[pl]) / r;
      float sin_theta = (ht - _vtx_t_cm[pl]) / r;
      float theta_rad = atan2(sin_theta,cos_theta);
      float theta_deg = theta_rad * 180. / 3.1415;

      theta_deg += 180.;

      ::cluster::pt thispt(r,theta_deg,hw,ht,q,pl);

      pt_v.push_back(thispt);
      
    }// for all his in cluster

    return;
  }// create polar cluster


  

  bool ClusterMaker::loadVertex(::larlite::event_vertex* ev_vtx) {
    
    if (ev_vtx->size() != 1) return false;
    
    // get vertex position on each plane
    if ( (ev_vtx->size() == 1) ){
      auto const& vtx = ev_vtx->at(0);
      
      std::vector<double> xyz = {vtx.X(), vtx.Y(), vtx.Z()};
      
      auto geoH = larutil::GeometryHelper::GetME();
      auto geom = larutil::Geometry::GetME();
      
      for (size_t pl = 0; pl < 3; pl++){
	double *origin;
	origin = new double[3];
	geom->PlaneOriginVtx(pl,origin);
	auto const& pt = geoH->Point_3Dto2D(xyz,pl);
	_vtx_w_cm[pl] = pt.w;
	_vtx_t_cm[pl] = pt.t + 800 * _time2cm - origin[0];
      }
    }
    
    return true;
  }



  

}

#endif
