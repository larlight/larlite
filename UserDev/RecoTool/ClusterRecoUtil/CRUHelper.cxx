#ifndef RECOTOOL_CRUHELPER_CXX
#define RECOTOOL_CRUHELPER_CXX

#include "CRUHelper.h"

namespace cluster {

  /// Generate: from 1 set of hits => 1 CPAN using indexes (association)
  void CRUHelper::GenerateCPAN(const std::vector<unsigned int>& hit_index,
			       const ::larlite::event_hit* hits,
			       ClusterParamsAlg &cpan) const
  {
    std::vector<larutil::PxHit> pxhits;
    GeneratePxHit(hit_index,hits,pxhits);
    cpan.SetHits(pxhits);
  }
  
  /// Generate: CPAN vector from event storage by specifying cluster type
  void CRUHelper::GenerateCPAN(::larlite::storage_manager* storage,
				const std::string &cluster_producer_name,
			       std::vector<cluster::ClusterParamsAlg> &cpan_v) const
  {
    std::vector<std::vector<larutil::PxHit> > pxhits_v;
    GeneratePxHit(storage,cluster_producer_name,pxhits_v);
    cpan_v.clear();
    cpan_v.resize(pxhits_v.size(),ClusterParamsAlg());

    for(size_t i=0; i<pxhits_v.size(); ++i)

      cpan_v.at(i).SetHits(pxhits_v.at(i));

  }
  
  /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
  void CRUHelper::GeneratePxHit(const std::vector<unsigned int>& hit_index,
				const ::larlite::event_hit* hits,
				std::vector<larutil::PxHit> &pxhits) const
  {

    if(!(hit_index.size())) throw CRUException(Form("Hit list empty! (%s)",__FUNCTION__));
    
    pxhits.clear();
    pxhits.reserve(hit_index.size());

    auto geo  = ::larutil::Geometry::GetME();
    auto geou = ::larutil::GeometryUtilities::GetME();

    UChar_t plane = geo->ChannelToPlane(hits->at(hit_index.at(0)).Channel());

    for(auto const& index : hit_index) {
      
      auto const& hit = hits->at(index);

      ::larutil::PxHit h;

      h.t = hit.PeakTime() * geou->TimeToCm();
      h.w = hit.Wire()     * geou->WireToCm();

      h.charge = hit.Charge();
      h.peak   = hit.Charge(true);
      h.plane  = plane;

      pxhits.push_back(h);
    }      

  }
  
  /// Generate: vector of PxHit sets from event storage by specifying cluster type
  void CRUHelper::GeneratePxHit(::larlite::storage_manager* storage,
				const std::string &cluster_producer_name,
				std::vector<std::vector<larutil::PxHit> > &pxhits_v) const
  {

    pxhits_v.clear();

    auto ev_cluster = storage->get_data< ::larlite::event_cluster>(cluster_producer_name);
    
    if(!ev_cluster)
      
      throw CRUException(Form("No cluster data product by %s found!",
			      cluster_producer_name.c_str())
			 );

    if(!(ev_cluster->size())) return;

    auto hit_producers = ev_cluster->association_keys(::larlite::data::kHit);

    if(!hit_producers.size())

      throw CRUException(Form("No associated hits to clusters by %s!",
			      cluster_producer_name.c_str())
			 );

    if(hit_producers.size()!=1)

      throw CRUException(Form("More than one type of associated hits for clusters by %s!",
			      cluster_producer_name.c_str())
			 );

    auto ev_hits = storage->get_data< ::larlite::event_hit>(hit_producers[0]);
    
    if(!ev_hits) 

      throw CRUException(Form("Associated hit by %s not found!",
			      hit_producers[0].c_str()
			      )
			 );

    auto ass = ev_cluster->association(::larlite::data::kHit, hit_producers[0]);
    
    pxhits_v.reserve(ev_cluster->size());

    for(size_t i=0; i<ev_cluster->size(); ++i) {

      if( i >= ass.size() ) break;

      auto &hit_index = ass[i];

      std::vector<larutil::PxHit> pxhits;

      GeneratePxHit(hit_index, ev_hits, pxhits);

      pxhits_v.push_back(pxhits);

    }
  }

}

#endif
