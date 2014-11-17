#ifndef RECOTOOL_MCSHOWERMATCHALG_CXX
#define RECOTOOL_MCSHOWERMATCHALG_CXX

#include "MCShowerMatchAlg.h"

namespace larlite {

  MCShowerMatchAlg::MCShowerMatchAlg() {

    _name = "MCShowerMatchAlg";
    _view_to_plane.clear();
    fBTAlgo.SetMaxEnergyCut(2000);
    fBTAlgo.SetMinEnergyCut(20);
  }

  bool MCShowerMatchAlg::BuildMap(storage_manager* storage,
				  const std::string cluster_producer)
  {
    auto geo = ::larutil::Geometry::GetME();

    //
    // Load all necessary data products. Return if necessary.
    //

    // Load MCShower and SimCh
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");

    auto ev_simch    = storage->get_data<event_simch>("largeant");

    if(!ev_mcshower) {
      print(msg::kERROR,__FUNCTION__,"MCShower data product not found!");
      return false;
    }

    if(!ev_simch) {
      print(msg::kERROR,__FUNCTION__,"SimCh data product not found!");
      return false;
    }

    // Return if no mcshower
    if(ev_mcshower->size()<1) {
      print(msg::kERROR,__FUNCTION__,"MCShower product found, but there are no mcshowers in this event!");
      return false;
    }

    // Load cluster
    auto ev_cluster = storage->get_data<event_cluster>(cluster_producer);

    if(!ev_cluster) {
      
      print(msg::kERROR,__FUNCTION__,Form("Cluster by \"%s\" not found...",
					  cluster_producer.c_str()
					  )
	    );
      return false;
      
    }
    else if(ev_cluster->size()<1) {
      print(msg::kERROR,__FUNCTION__,"Cluster product found, but there are no clusters in this event!");
      return false;
    }
    // Load association
    auto hit_ass_key = ev_cluster->association_keys(data::kHit);

    if(!(hit_ass_key.size())) {

      print(msg::kERROR,__FUNCTION__,
	    Form("Cluster by \"%s\" has no association to hit...",cluster_producer.c_str())
	    );
      return false;
    }

    auto hit_ass = ev_cluster->association(data::kHit,hit_ass_key[0]);

    // Load hit
    auto ev_hit = storage->get_data<event_hit>(hit_ass_key[0]);

    if(!ev_hit) {

      print(msg::kERROR,__FUNCTION__,
	    Form("Hit by \"%s\" not found...",
		 hit_ass_key[0].c_str()
		 )
	    );
      return false;
    }else if(!(ev_hit->size())) return false;


    //
    // Prepare Back tracking algorithm
    //

    fBTAlgo.BuildMap(*ev_mcshower, *ev_simch);

    //
    // Perform back-tracking
    //
    // (1) Load cluster/hit data product
    // (2) Loop over all clusters and find charge fraction to each MCShower
    // (3) For each MCShower, find a cluster per plane with the highest charge
    
    // Loop over clusters & get charge fraction per MCShower
    _summed_mcq.clear();
    _cluster_mcq_v.clear();
    _cluster_plane_id.clear();

    _summed_mcq.resize(fBTAlgo.UniqueShowerID().size()+1,std::vector<double>(geo->Nplanes(),0));
    _cluster_mcq_v.reserve(ev_cluster->size());
    _cluster_plane_id.reserve(ev_cluster->size());

    for(auto const& h_index_v : hit_ass) {

      auto const& h = ev_hit->at(h_index_v[0]);

      size_t plane = geo->ChannelToPlane(h.Channel());

      // Create hit list
      std::vector<WireRange_t> hit_v;
      hit_v.reserve(h_index_v.size());

      for(auto const& index : h_index_v) {
	
	WireRange_t w;
	auto const& h = ev_hit->at(index);
	w.ch    = h.Channel();
	w.start = h.StartTime();
	w.end   = h.EndTime();
	hit_v.push_back(w);

      }

      _cluster_plane_id.push_back(plane);

      auto mcq_v = fBTAlgo.MCShowerQ(hit_v);

      for(size_t i=0; i<mcq_v.size(); ++i)
	_summed_mcq[i][plane] += mcq_v[i];

      _cluster_mcq_v.push_back(mcq_v);

    }

    //
    // Find the best matched pair (and its charge) per MCShower
    //
    // index number of MCShowers (in event_mcshower) that are taken into account 
    auto& mcshower_id = fBTAlgo.UniqueShowerID();

    _bmatch_id.clear();
    _bmatch_id.resize(mcshower_id.size(),std::vector<int>(geo->Nplanes(),-1));

    std::vector<std::vector<double> > bmatch_mcq (mcshower_id.size(),std::vector<double>(geo->Nplanes(),0));

    for(size_t shower_index=0; shower_index < mcshower_id.size(); ++shower_index) {

      for(size_t cluster_index=0; cluster_index < ev_cluster->size(); ++cluster_index) {

	if((*(_cluster_mcq_v.at(cluster_index).rbegin())) < 0) continue;

	auto plane = _cluster_plane_id.at(cluster_index);
	
	double q = _cluster_mcq_v.at(cluster_index).at(shower_index);
	
	if( bmatch_mcq.at(shower_index).at(plane) < q ) {

	  bmatch_mcq.at(shower_index).at(plane) = q;
	  _bmatch_id.at(shower_index).at(plane) = cluster_index;

	}
      }
    }

    _mcshower_index_v.resize(ev_mcshower->size(),-1);
    for(size_t i=0; i<fBTAlgo.UniqueShowerID().size(); ++i)

      _mcshower_index_v.at(fBTAlgo.UniqueShowerID()[i]) = i;

    fClusterProducer = cluster_producer;
    return true;
  }

  size_t MCShowerMatchAlg::__IndexConversion__(const size_t mcshower_index) const
  {
    if(mcshower_index >= _mcshower_index_v.size())
      throw MCShowerBTException(Form("MCShower index %zu exceeds # of MCShower data in the file (%zu)!",
				     mcshower_index,
				     _mcshower_index_v.size()));

    if(_mcshower_index_v[mcshower_index] < 0)
      throw MCShowerBTException(Form("MCShower index %zu not among the relevant MCShower list!",
				     mcshower_index)
				);
    return _mcshower_index_v[mcshower_index];

  }
  
  double MCShowerMatchAlg::ClusterCorrectness(const size_t cluster_index,
					      const size_t mcshower_index) const
  {    

    if(!_bmatch_id.size()) 
      throw MCShowerBTException("Preparation not done yet!");

    if( cluster_index >= _cluster_mcq_v.size()) 
      throw MCShowerBTException(Form("Input cluster index (%zu) out of range (%zu)!",
				     cluster_index,
				     _cluster_mcq_v.size())
				);

    auto shower_index = __IndexConversion__(mcshower_index);

    auto plane = _cluster_plane_id.at(cluster_index);

    auto best_cluster_index = _bmatch_id.at(shower_index).at(plane);
    
    return _cluster_mcq_v.at(cluster_index).at(shower_index) / _cluster_mcq_v.at(best_cluster_index).at(shower_index);

  }

  std::pair<size_t,double> MCShowerMatchAlg::ShowerCorrectness(const std::vector<unsigned int> cluster_indices) const
  {    
    
    if(!cluster_indices.size()) throw MCShowerBTException("Input cluster indices empty!");

    auto& mcshower_id = fBTAlgo.UniqueShowerID();

    // Compute efficiency per MCShower
    std::vector<double> match_eff(mcshower_id.size(),1);
    
    for(auto const& cluster_index : cluster_indices) {

      for(size_t shower_index=0; shower_index < mcshower_id.size(); ++shower_index)

	match_eff.at(shower_index) *= ClusterCorrectness(cluster_index, mcshower_id[shower_index]);
    }

    std::pair<size_t,double> result(0,-1);
    
    // Find the best qratio
    for(size_t shower_index=0; shower_index < mcshower_id.size(); ++shower_index) {
      
      if(match_eff.at(shower_index) < result.second) continue;

      result.second = match_eff.at(shower_index);
      
      result.first = mcshower_id.at(shower_index);
      
    }
    return result;
  }

  std::pair<double,double>  MCShowerMatchAlg::ClusterEP(const size_t cluster_index,
							const size_t mcshower_index) const
  {

    if(cluster_index >= _cluster_plane_id.size())
      throw MCShowerBTException(Form("Cluster index %zu exceeds the # of loaded clusters (by \"%s\")!",
				     cluster_index,
				     fClusterProducer.c_str())
				);

    auto shower_index = __IndexConversion__(mcshower_index);

    // efficiency = this cluster's mcq for specified mcshower / total mcq by this mcshower in all clusters
    // purity = this cluster's mcq for this mcshower / total mcq from all mcshower in this cluster

    std::pair<double,double> result;

    auto plane = _cluster_plane_id[cluster_index];

    result.first  = _cluster_mcq_v[cluster_index][shower_index] / _summed_mcq[shower_index][plane];

    double cluster_mcq_total = 0;
    for(auto const& q : _cluster_mcq_v[cluster_index]) cluster_mcq_total += q;

    result.second =  _cluster_mcq_v[cluster_index][shower_index] / cluster_mcq_total;

    return result;
  }

  const std::vector<int>& MCShowerMatchAlg::BestClusters(const size_t mcshower_index) const
  {
    auto shower_index = __IndexConversion__(mcshower_index);

    return _bmatch_id[shower_index];

  }

  std::pair<double,double> MCShowerMatchAlg::BestClusterEP(const size_t mcshower_index,
							   const size_t plane_id) const
  {

    auto c_index_v = BestClusters(mcshower_index);

    if(c_index_v.size() <= plane_id)
      throw MCShowerBTException(Form("Plane ID %zu exceeds # of planes recorded in data (%zu)...",
				     plane_id,
				     c_index_v.size())
				);

    std::pair<double,double> result(0,0);

    if(c_index_v[plane_id]<0) return result;

    return ClusterEP(c_index_v[plane_id],mcshower_index);

  }

}
#endif
