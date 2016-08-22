#ifndef RECOTOOL_CRUHELPER_CXX
#define RECOTOOL_CRUHELPER_CXX

#include "CRUHelper.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"


namespace cluster {

  /// Generate: from hit vector
  void CRUHelper::GenerateParams(const std::vector<::larlite::hit>& hit_v,
				 cluster_params &params) const
  {
    std::vector<larutil::Hit2D> hits2d;
    GenerateHit2D(hit_v, hits2d);
    params.SetHits(hits2d);
    
  }
  
  
  /// Generate: from 1 set of hits => 1 CPAN using indexes (association)
  void CRUHelper::GenerateParams(const std::vector<unsigned int>& hit_index,
				 const ::larlite::event_hit* hits,
				 cluster_params &params) const
  {
    std::vector<larutil::Hit2D> hits2d;
    GenerateHit2D(hit_index, hits, hits2d);
    params.SetHits(hits2d);
    
  }
  
  /// Generate: CPAN vector from event storage by specifying cluster type
  void CRUHelper::GenerateParams(::larlite::storage_manager* storage,
				 const std::string &cluster_producer_name,
				 std::vector<cluster::cluster_params> &params_v) const
  {
    
    
    params_v.clear();
    
    auto ev_cluster = storage->get_data< ::larlite::event_cluster>(cluster_producer_name);
    
    params_v.resize(ev_cluster -> size(), cluster_params());
    
    if (!ev_cluster)
      throw CRUException(Form("No cluster data product by %s found!", cluster_producer_name.c_str()) );
    
    if (!(ev_cluster->size())) return;

    ::larlite::event_hit* ev_hits = nullptr;
    
    auto const& hit_index_v = storage->find_one_ass(ev_cluster->id(), ev_hits, cluster_producer_name);
    
    if (!ev_hits)
      throw CRUException(Form("No associated hits to clusters by %s!", cluster_producer_name.c_str()) );
    
    if (!hit_index_v.size())
      throw CRUException(Form("Size 0 associated hits to clusters by %s!", cluster_producer_name.c_str()) );
    
    
    for (size_t i = 0; i < ev_cluster->size(); ++i) {
      
      if ( i >= hit_index_v.size() ) break;
      
      auto &hit_index = hit_index_v[i];
      
      std::vector<larutil::Hit2D> hits2d;
      
      GenerateHit2D(hit_index, ev_hits, hits2d);
      
      params_v.at(i).SetHits(hits2d);
      params_v.at(i).original_producer = ev_cluster -> at(i).OriginalClusterProducer();
      params_v.at(i).original_indexes = ev_cluster -> at(i).OriginalClusterIndexes();
    }
    
    return;
  }
  
  /// Generate: from 1 set of hits => 1 set of larutil::Hit2D using indexes (association)
  void CRUHelper::GenerateHit2D(const std::vector<unsigned int>& hit_index,
				const ::larlite::event_hit* hits,
				std::vector<larutil::Hit2D> &hits2d) const
  {
    
    if (!(hit_index.size())) throw CRUException(Form("Hit list empty! (%s)", __FUNCTION__));
    
    hits2d.clear();
    hits2d.reserve(hit_index.size());
    
    auto geo  = ::larutil::Geometry::GetME();
    auto geoH = ::larutil::GeometryHelper::GetME();
    auto detProp = ::larutil::DetectorProperties::GetME();
    
    UChar_t plane = geo->ChannelToPlane(hits->at(hit_index.at(0)).Channel());
    
    double * origin;
    origin = new double[geo->Nviews()];
    geo->PlaneOriginVtx(plane, origin);
    float planeOffset = origin[0];
    
    for (auto const& index : hit_index) {
      
      auto const& hit = hits->at(index);
      
      ::larutil::Hit2D h;
      
      h.t = (hit.PeakTime() - detProp->TriggerOffset()) * geoH->TimeToCm() + planeOffset;
      h.w = hit.WireID().Wire * geoH->WireToCm();
      
      h.charge = hit.Integral();
      h.peak   = hit.PeakAmplitude();
      h.plane  = plane;
      
      hits2d.push_back(h);
    }
    
    delete origin;

    return;
  }

  
  /// Generate: from hit vector
  void CRUHelper::GenerateHit2D(const std::vector<::larlite::hit>& hit_v,
				std::vector<larutil::Hit2D> &hits2d) const
  {
    
    if (!(hit_v.size())) throw CRUException(Form("Hit list empty! (%s)", __FUNCTION__));
    
    hits2d.clear();
    hits2d.reserve(hit_v.size());
    
    auto geo  = ::larutil::Geometry::GetME();
    auto geoH = ::larutil::GeometryHelper::GetME();
    auto detProp = ::larutil::DetectorProperties::GetME();
    
    UChar_t plane = geo->ChannelToPlane(hit_v[0].Channel());
    
    double * origin;
    origin = new double[geo->Nviews()];
    geo->PlaneOriginVtx(plane, origin);
    float planeOffset = origin[0];
    
    for (auto const& hit : hit_v) {
      
      ::larutil::Hit2D h;
      
      h.t = (hit.PeakTime() - detProp->TriggerOffset()) * geoH->TimeToCm() + planeOffset;
      h.w = hit.WireID().Wire * geoH->WireToCm();
      
      h.charge = hit.Integral();
      h.peak   = hit.PeakAmplitude();
      h.plane  = plane;
      
      hits2d.push_back(h);
    }
    
    delete origin;
    
    return;
  }
  
  /// Generate: vector of PxHit sets from event storage by specifying cluster type
  void CRUHelper::GenerateHit2D(::larlite::storage_manager* storage,
				const std::string &cluster_producer_name,
				std::vector<std::vector<larutil::Hit2D> > &hits2d_v) const
  {
    
    hits2d_v.clear();
    
    auto ev_cluster = storage->get_data< ::larlite::event_cluster>(cluster_producer_name);
    
    if (!ev_cluster)
      throw CRUException(Form("No cluster data product by %s found!", cluster_producer_name.c_str()) );
    
    if (!(ev_cluster->size())) return;
    
    ::larlite::event_hit* ev_hits = nullptr;
    
    auto const& hit_index_v = storage->find_one_ass(ev_cluster->id(), ev_hits, cluster_producer_name);
    
    if (!ev_hits)
      throw CRUException(Form("No associated hits to clusters by %s!", cluster_producer_name.c_str()) );
    
    if (!hit_index_v.size())
      throw CRUException(Form("Size 0 associated hits to clusters by %s!", cluster_producer_name.c_str()) );
    
    hits2d_v.reserve(ev_cluster->size());
    
    for (size_t i = 0; i < ev_cluster->size(); ++i) {
      
      if ( i >= hit_index_v.size() ) break;
      
      auto &hit_index = hit_index_v[i];
      
      std::vector<larutil::Hit2D> hits2d;
      
      GenerateHit2D(hit_index, ev_hits, hits2d);
      
      hits2d_v.push_back(hits2d);
      
    }

    return;
  }
  
}// namespace

#endif
