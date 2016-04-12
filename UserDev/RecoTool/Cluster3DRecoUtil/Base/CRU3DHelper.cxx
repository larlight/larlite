#ifndef RECOTOOL_CRU3DHELPER_CXX
#define RECOTOOL_CRU3DHELPER_CXX

#include "CRU3DHelper.h"


namespace cluster3D {
  
  /// Generate: from 1 set of sps => 1 Params3D using indexes (association)
  void CRU3DHelper::GenerateParams3D(const std::vector<unsigned int>& point_index,
				     const larlite::event_spacepoint* points,
				     cluster3D_params &params) const
  {
    std::vector<::cluster3D::Point3D> points3D;
    GeneratePoint3D(point_index, points, points3D);
    params.SetPoints(points3D);

    return;
  }
  
  
  /// Generate: from sps vector
  void CRU3DHelper::GenerateParams3D(const std::vector<::larlite::spacepoint>& sps_v,
				     cluster3D_params &params) const
  {
    std::vector<::cluster3D::Point3D> points3D;
    GeneratePoint3D(sps_v, points3D);
    params.SetPoints(points3D);

    return;
  }
  
  /// Generate: Params vector from event storage by specifying pfpart type
  void CRU3DHelper::GenerateParams3D(::larlite::storage_manager* storage,
				     const std::string &pfpart_producer_name,
				     std::vector<cluster3D_params> &params_v) const
  {
    
    
    params_v.clear();
    
    auto ev_pfpart = storage->get_data< ::larlite::event_pfpart>(pfpart_producer_name);
    
    params_v.resize(ev_pfpart -> size(), cluster3D_params());
    
    if (!ev_pfpart)
      throw CRU3DException(Form("No pfpart data product by %s found!", pfpart_producer_name.c_str()) );
    
    if (!(ev_pfpart->size())) return;
    
    ::larlite::event_spacepoint* ev_sps = nullptr;
    
    auto const& sps_index_v = storage->find_one_ass(ev_pfpart->id(), ev_sps, pfpart_producer_name);
    
    if (!ev_sps)
      throw CRU3DException(Form("No associated spacepoints to pfpart by %s!", pfpart_producer_name.c_str()) );
    
    if (!sps_index_v.size())
      throw CRU3DException(Form("Size 0 associated spacepoints to pfpart by %s!", pfpart_producer_name.c_str()) );
    
    
    for (size_t i = 0; i < ev_pfpart->size(); ++i) {
      
      if ( i >= sps_index_v.size() ) break;
      
      auto &sps_index = sps_index_v[i];
      
      std::vector<Point3D> points3D;
      
      GeneratePoint3D(sps_index, ev_sps, points3D);
      
      params_v.at(i).SetPoints(points3D);
      
    }
    
    return;
    
  }
  
  /// Generate: from 1 set of sps => 1 set of Point3D using indexes (association)
  void CRU3DHelper::GeneratePoint3D(const std::vector<unsigned int>& sps_index,
				    const larlite::event_spacepoint* sps,
				    std::vector<Point3D> &point3D_set) const
  {
    
    if (!(sps_index.size())) throw CRU3DException(Form("Hit list empty! (%s)", __FUNCTION__));
    
    point3D_set.clear();
    point3D_set.reserve(sps_index.size());
    
    for (auto const& index : sps_index) {
      
      auto const& point = sps->at(index);
      
      Point3D p;
      
      p.x = point.XYZ()[0];
      p.y = point.XYZ()[1];
      p.z = point.XYZ()[2];
      
      point3D_set.push_back(p);
      
    }
    
    return;
  }


  /// Generate: from sps vector
  void CRU3DHelper::GeneratePoint3D(const std::vector<::larlite::spacepoint>& sps_v,
				    std::vector<Point3D> &point3D_set) const
  {
    
    if (!(sps_v.size())) throw CRU3DException(Form("Hit list empty! (%s)", __FUNCTION__));
    
    point3D_set.clear();
    point3D_set.reserve(sps_v.size());
    
    for (auto const& point : sps_v) {
      
      Point3D p;
      
      p.x = point.XYZ()[0];
      p.y = point.XYZ()[1];
      p.z = point.XYZ()[2];
      
      point3D_set.push_back(p);
      
    }
    
    return;
  }  
  
  /// Generate: vector of Point3D sets from event storage by specifying pfpart type
  void CRU3DHelper::GeneratePoint3D(larlite::storage_manager* storage,
				    const std::string &pfpart_producer_name,
                                  std::vector<std::vector<Point3D> > &point3D_set_v) const
  {
    
    point3D_set_v.clear();
    
    auto ev_pfpart = storage->get_data< ::larlite::event_pfpart>(pfpart_producer_name);
    
    if (!ev_pfpart)
      throw CRU3DException(Form("No cluster data product by %s found!", pfpart_producer_name.c_str()) );
    
    if (!(ev_pfpart->size())) return;
    
    ::larlite::event_spacepoint* ev_sps = nullptr;
    
    auto const& sps_index_v = storage->find_one_ass(ev_pfpart->id(), ev_sps, pfpart_producer_name);
    
    if (!ev_sps)
      throw CRU3DException(Form("No associated spacepoints to pfpart by %s!", pfpart_producer_name.c_str()) );
    
    if (!sps_index_v.size())
      throw CRU3DException(Form("Size 0 associated spacepoints to pfpart by %s!", pfpart_producer_name.c_str()) );
    
    point3D_set_v.reserve(ev_pfpart->size());
    
    for (size_t i = 0; i < ev_pfpart->size(); ++i) {
      
      if ( i >= sps_index_v.size() ) break;
      
      auto &hit_index = sps_index_v[i];
      
      std::vector<Point3D> points2D;
      
      GeneratePoint3D(hit_index, ev_sps, points2D);
      
      point3D_set_v.push_back(points2D);
      
    }

    return;
  }

}// namespace

#endif
