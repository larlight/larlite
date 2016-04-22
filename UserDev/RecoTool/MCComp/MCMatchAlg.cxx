#ifndef RECOTOOL_MCMATCHALG_CXX
#define RECOTOOL_MCMATCHALG_CXX

#include "MCMatchAlg.h"

namespace btutil {

  MCMatchAlg::MCMatchAlg() {
    _view_to_plane.clear();
    _tick_offset = 0.;
    _verbose = false;
  }

  bool MCMatchAlg::BuildMap(const std::vector< unsigned int>     &g4_trackid_v,
			    const std::vector< ::larlite::simch> &simch_v,
			    const std::vector< ::larlite::hit>   &hit_v,
			    const std::vector<std::vector<unsigned int> > &cluster_hit_association
			    )
  {
    fBTAlgo.Reset(g4_trackid_v,simch_v);
    
    return BuildMap(hit_v, cluster_hit_association);
  }

  bool MCMatchAlg::BuildMap(const std::vector<std::vector< unsigned int> > &g4_trackid_v,
			    const std::vector< ::larlite::simch> &simch_v,
			    const std::vector< ::larlite::hit>   &hit_v,
			    const std::vector<std::vector<unsigned int> > &cluster_hit_association
			    )
  {

    fBTAlgo.Reset(g4_trackid_v,simch_v);

    return BuildMap(hit_v, cluster_hit_association);
  }

  bool MCMatchAlg::BuildMap(const std::vector< ::larlite::hit>   &hit_v,
			    const std::vector<std::vector<unsigned int> > &cluster_hit_association)
  {
    if(hit_v.empty() || cluster_hit_association.empty())
      //  return false;
      throw MCBTException("Hit or HitAssociation Empty!");

    size_t num_mcobj = fBTAlgo.NumParts();
    if(!num_mcobj)
      //return false;
      throw MCBTException("MCShower or MCTrack empty!");

    if (_verbose)
      std::cout << "Building MAP " << std::endl;
    
    size_t num_cluster = cluster_hit_association.size();
    auto geo = ::larutil::Geometry::GetME();
    //art::ServiceHandle<geo::Geometry> geo;

    //
    // Perform back-tracking
    //
    // (1) Load cluster/hit data product
    // (2) Loop over all clusters and find charge fraction to each MC object
    // (3) For each MC object, find a cluster per plane with the highest charge
    
    // Loop over clusters & get charge fraction per MC object
    _summed_mcq.clear();
    _cluster_mcq_v.clear();
    _cluster_plane_id.clear();

    _summed_mcq.resize(num_mcobj+1,std::vector<double>(geo->Nplanes(),0));
    _cluster_mcq_v.reserve(num_cluster);
    _cluster_plane_id.reserve(num_cluster);

    for(auto const& h_index_v : cluster_hit_association){

      size_t plane = geo->Nplanes();

      // Create hit list
      std::vector<WireRange_t> wr_v;
      wr_v.reserve(h_index_v.size());

      for(auto const& index : h_index_v) {
	
	WireRange_t wr;
	auto const& h = hit_v.at(index);
	wr.ch    = h.Channel();
	wr.start = h.PeakTime() - 3 * h.RMS() + _tick_offset;
	//wr.start = h.StartTick();
	wr.end   = h.PeakTime() + 3 * h.RMS() + _tick_offset;
	//wr.end   = h.EndTick();
	wr_v.push_back(wr);
	if(plane==geo->Nplanes()) plane = h.WireID().Plane;
	//if(plane==geo->Nplanes()) plane = h.View();
      }

      if (_verbose)
	std::cout << "plane is " << plane << " for cluster index " << _cluster_plane_id.size() << std::endl;

      _cluster_plane_id.push_back(plane);

      auto mcq_v = fBTAlgo.MCQ(wr_v);


      for(size_t i=0; i<mcq_v.size(); ++i)

	_summed_mcq[i][plane] += mcq_v[i]; 

      _cluster_mcq_v.push_back(mcq_v);

    }

    //
    // Find the best matched pair (and its charge) per MC object
    //
    _bmatch_id.clear();
    _bmatch_id.resize(num_mcobj,std::vector<int>(geo->Nplanes(),-1));

    std::vector<std::vector<double> > bmatch_mcq (num_mcobj,std::vector<double>(geo->Nplanes(),0));

    for(size_t mc_index=0; mc_index < num_mcobj; ++mc_index) {

      for(size_t cluster_index=0; cluster_index < num_cluster; ++cluster_index) {

	//if((_cluster_mcq_v[cluster_index].back()) < 0) continue;

	auto plane = _cluster_plane_id.at(cluster_index);
	
	double q = _cluster_mcq_v[cluster_index][mc_index];

	if (_verbose)
	  std::cout << "Charge for cluster idx " << cluster_index << " & mcs index " << mc_index << " : " << q << std::endl;  
	
	if( bmatch_mcq[mc_index][plane] < q ) {

	  bmatch_mcq[mc_index][plane] = q;
	  _bmatch_id[mc_index][plane] = cluster_index;
	  
	}
      }
    }

    return true;
  }

  double MCMatchAlg::ClusterCorrectness(const size_t cluster_index,
					const size_t mc_index) const
  {    

    if(!_bmatch_id.size()) 
      throw MCBTException("Preparation not done yet!");

    if( cluster_index >= _cluster_mcq_v.size()) 
      throw MCBTException(Form("Input cluster index (%zu) out of range (%zu)!",
			       cluster_index,
			       _cluster_mcq_v.size())
			  );

    if( mc_index >= _bmatch_id.size() )
      throw MCBTException(Form("Input MC object index (%zu) out of range (%zu)!",
			       mc_index,
			       _bmatch_id.size())
			  );

    auto plane = _cluster_plane_id.at(cluster_index);

    auto best_cluster_index = _bmatch_id.at(mc_index).at(plane);

    if(best_cluster_index < 0) return -1;

    return _cluster_mcq_v.at(cluster_index).at(mc_index) / _cluster_mcq_v.at(best_cluster_index).at(mc_index);

  }

  std::pair<size_t,double> MCMatchAlg::MatchCorrectness(const std::vector<unsigned int> cluster_indices) const
  {

    if(!_bmatch_id.size()) 
      throw MCBTException("Preparation not done yet!");

    if( cluster_indices.size() > _cluster_mcq_v.size()) 
      throw MCBTException(Form("Input cluster indices length (%zu) > # of registered clusters (%zu)!",
			       cluster_indices.size(),
			       _cluster_mcq_v.size())
			  );

    if(!cluster_indices.size()) throw MCBTException("Input cluster indices empty!");

    // Compute efficiency per MC
    std::vector<double> match_eff(_bmatch_id.size(),1);
    
    for(auto const& cluster_index : cluster_indices) {

      for(size_t mc_index=0; mc_index < _bmatch_id.size(); ++mc_index) {

	double correctness = ClusterCorrectness(cluster_index, mc_index);
	if (_verbose)
	  std::cout  << "Cluster correctnes @ mc idx " << mc_index << " & clus idx " << cluster_index << " : " << correctness << std::endl;
	
	if(correctness>=0)
	  match_eff.at(mc_index) *= correctness;
	
      }
    }

    std::pair<size_t,double> result(0,-1);
    
    // Find the best qratio
    for(size_t mc_index=0; mc_index < match_eff.size(); ++mc_index) {

      if (_verbose)
	std::cout << "match eff @ mc idx " << mc_index << " is " << match_eff.at(mc_index) << std::endl;
      
      if(match_eff.at(mc_index) < result.second) continue;

      result.second = match_eff.at(mc_index);
      
      result.first  = mc_index; 
      
    }
    return result;
  }

  std::pair<double,double>  MCMatchAlg::ClusterEP(const size_t cluster_index,
						  const size_t mc_index) const
  {
    if(!_bmatch_id.size()) 
      throw MCBTException("Preparation not done yet!");

    if( cluster_index >= _cluster_mcq_v.size()) 
      throw MCBTException(Form("Input cluster index (%zu) out of range (%zu)!",
			       cluster_index,
			       _cluster_mcq_v.size())
			  );

    if( mc_index >= _bmatch_id.size() )
      throw MCBTException(Form("Input MC object index (%zu) out of range (%zu)!",
			       mc_index,
			       _bmatch_id.size())
			  );

    // efficiency = this cluster's mcq for specified mc obj / total mcq by this mcshower in all clusters
    // purity = this cluster's mcq for this mcshower / total mcq from all mc obj in this cluster

    std::pair<double,double> result;

    auto plane = _cluster_plane_id.at(cluster_index);

    if (_verbose){
      std::cout << "Cluster idx : " << cluster_index << " w/ plane : " << plane << std::endl;
      std::cout << "Summed MCQ for mc idx " << mc_index << " @ plane " << plane << " : " << _summed_mcq[mc_index][plane] << std::endl;
    }

    // total charge deposited by MC object in this plane
    double mcq_tot = _summed_mcq[mc_index][plane];
    if (mcq_tot > 0)
      result.first = _cluster_mcq_v[cluster_index][mc_index] / mcq_tot;
    else
      result.first = 0.;

    double cluster_mcq_total = 0;
    for(auto const& q : _cluster_mcq_v[cluster_index]) cluster_mcq_total += q;

    if (cluster_mcq_total > 0)
      result.second =  _cluster_mcq_v[cluster_index][mc_index] / cluster_mcq_total;
    else
      result.second = 0.;

    return result;
  }


  std::pair<size_t,std::pair<double,double>> MCMatchAlg::MatchEP(const std::vector<unsigned int> cluster_indices) const
  {
    
    if(!_bmatch_id.size()) 
      throw MCBTException("Preparation not done yet!");
    
    if( cluster_indices.size() > _cluster_mcq_v.size()) 
      throw MCBTException(Form("Input cluster indices length (%zu) > # of registered clusters (%zu)!",
			       cluster_indices.size(),
			       _cluster_mcq_v.size())
			  );
    
    if(!cluster_indices.size()) throw MCBTException("Input cluster indices empty!");
    
    // Compute efficiency/purity per MC
    std::vector<double> match_eff(_bmatch_id.size(),1);
    std::vector<double> match_pur(_bmatch_id.size(),1);
    
    for(auto const& cluster_index : cluster_indices) {
      
      for(size_t mc_index=0; mc_index < _bmatch_id.size(); ++mc_index) {

	auto EffPur = ClusterEP(cluster_index,mc_index);
	if (_verbose){
	  std::cout  << "Cluster efficiency @ mc idx " << mc_index << " & clus idx " << cluster_index << " : " << EffPur.first << std::endl;
	  std::cout  << "Cluster purity     @ mc idx " << mc_index << " & clus idx " << cluster_index << " : " << EffPur.second << std::endl;
	}
	
	match_eff.at(mc_index) *= EffPur.first;
	match_pur.at(mc_index) *= EffPur.second;
	
      }
    }

    // [best MC index, [ efficiency, purity ] ]
    std::pair<size_t, std::pair<double,double> > result(0, std::make_pair(0.,0.) );
    
    // Find the best qratio
    for(size_t mc_index=0; mc_index < match_eff.size(); ++mc_index) {

      if (_verbose){
	std::cout << "match eff @ mc idx " << mc_index << " is " << match_eff.at(mc_index) << std::endl;
	std::cout << "match pur @ mc idx " << mc_index << " is " << match_pur.at(mc_index) << std::endl;
	std::cout << "Current best purity : " << result.second.second << std::endl;
      }
      
      // if we have a better purity -> update results
      if(match_pur.at(mc_index) > result.second.second){
	// set efficiency and purity
	result.second.first  = match_eff.at(mc_index);
	result.second.second = match_pur.at(mc_index);
	if (_verbose)
	  std::cout << "Updating Eff & Purity w/ " << result.second.first << " & " << result.second.second << std::endl;
	// set MC index
	result.first  = mc_index;
      }// if we have a better purity
    }// for all MC indices
    return result;
  }

  const std::vector<int>& MCMatchAlg::BestClusters(const size_t mc_index) const
  {
    if(!_bmatch_id.size()) 
      throw MCBTException("Preparation not done yet!");

    if( mc_index >= _bmatch_id.size() )
      throw MCBTException(Form("Input MC object index (%zu) out of range (%zu)!",
			       mc_index,
			       _bmatch_id.size())
			  );

    return _bmatch_id[mc_index];

  }

  std::pair<double,double> MCMatchAlg::BestClusterEP(const size_t mc_index,
						     const size_t plane_id) const
  {

    auto c_index_v = BestClusters(mc_index);

    if(c_index_v.size() <= plane_id)
      throw MCBTException(Form("Plane ID %zu exceeds # of planes recorded in data (%zu)...",
			       plane_id,
			       c_index_v.size())
			  );

    std::pair<double,double> result(0,0);

    if(c_index_v[plane_id]<0) return result;

    return ClusterEP(c_index_v[plane_id],mc_index);

  }

}
#endif
