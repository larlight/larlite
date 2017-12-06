#ifndef RECOTOOL_CBALGOVTXALIGN_CXX
#define RECOTOOL_CBALGOVTXALIGN_CXX

#include "CBAlgoVtxAlign.h"
#include <map>
#include <algorithm>

namespace cmtool {

  //----------------------------------------
  CBAlgoVtxAlign::CBAlgoVtxAlign() : CBoolAlgoBase()
  //----------------------------------------
  {
    _max_angle_diff_merge  = 0.;
    _min_gammagamma_oangle = 0.;
    _min_nhits = 0;
  }

  std::vector< std::vector<size_t> > CBAlgoVtxAlign::Merge(const std::vector<::cluster::Cluster>& clus_v) {

    std::vector< std::vector<size_t> > merge_result{};

    for (size_t pl=0; pl < 3; pl++) {

      if (_verbose)
	std::cout << std::endl << std::endl << std::endl << "****** PLANE " << pl << " *****" << std::endl << std::endl << std::endl << std::endl;

      // split clusters per plane:
      // and order by size
      std::vector< std::pair<size_t,size_t> > plane_clus_pairs_v; // pair [size, index]
      for (size_t i=0; i < clus_v.size(); i++) {
	if (clus_v[i]._plane == pl) {
	  plane_clus_pairs_v.push_back( std::make_pair( clus_v[i].size(), i ) );
	}
      }

      std::sort(plane_clus_pairs_v.rbegin(), plane_clus_pairs_v.rend());

      std::vector<size_t> plane_clus_idx_v;
      for (size_t i=0; i < plane_clus_pairs_v.size(); i++)
	plane_clus_idx_v.push_back( plane_clus_pairs_v[i].second );


      
      // pair links cluster index to pair of ( NHits, angle )
      // object will hold cluster angle for cluster with largest number of hits per plane
      std::pair< size_t, std::pair<size_t, double> > gamma_00(std::make_pair( 0, std::make_pair( 0, 0.) ) );
      std::pair< size_t, std::pair<size_t, double> > gamma_01(std::make_pair( 0, std::make_pair( 0, 0.) ) );


      // map linking cluster index and angle
      std::map<size_t, double> clus_angle_map;

      /*
      // sort clusters by size
      // map contains nhits -> index
      std::map<int,int> clus_size_map;

      for (auto const& idx : plane_clus_idx_v)
	clus_size_map[ clus_v[idx].size() ] = idx;
      */

      for (auto const& idx : plane_clus_idx_v) {
	
	//std::map<int,int>::reverse_iterator it;
	//for (it = clus_size_map.rbegin(); it != clus_size_map.rend(); it++) {

	//auto idx = it->second;
	auto const& cluster = clus_v.at(idx);

	auto nhits = cluster.size();
	auto angle = cluster._angle;

	clus_angle_map[ idx ] = angle;

	if (_verbose){
	  std::cout << "New cluster @ Plane " << pl
		    << "\t Nhits = " << nhits
		    << "\t angle = " << cluster._angle << std::endl
		    << "Current gamma00 angle : " << gamma_00.second.second << "\t gamma01 angle : " << gamma_01.second.second << std::endl;
	}

	if ( nhits > gamma_00.second.first ) {
	  if (_verbose) std::cout << "\t updating gamma00" << std::endl;
	  gamma_00.second.first  = nhits; // update # of hits for largest cluster in plane
	  gamma_00.second.second = angle; // update direction from PCA for largest cluster in plane
	  gamma_00.first         = idx;   // update index
	}
	
	if ( ( nhits > gamma_01.second.first ) &&
	     ( fabs(angle-gamma_00.second.second ) > _min_gammagamma_oangle) &&
	     ( gamma_00.second.second != 0 ) ) {
	  if (_verbose) std::cout << "\t updating gamma01" << std::endl;
	  gamma_01.second.first  = nhits; // update # of hits for largest cluster in plane
	  gamma_01.second.second = angle; // update direction from PCA for largest cluster in plane
	  gamma_01.first         = idx;   // update index
	}
	
	if (_verbose) std::cout << std::endl << std::endl;

      }// for all clusters on this plane

      if (_verbose)
	std::cout << " gamma00 has " << gamma_00.second.first << " hits"
		  << " and " << gamma_00.second.second << " angle" << std::endl;
      if (_verbose)
	std::cout << " gamma01 has " << gamma_01.second.first << " hits"
		  << " and " << gamma_01.second.second << " angle" << std::endl;

      // "big" cluster 00
      auto gammacluster_00 = clus_v.at( gamma_00.first );
      auto gammaangle_00   = gamma_00.second.second;
      
      // "big" cluster 01
      auto gammacluster_01 = clus_v.at( gamma_01.first );
      auto gammaangle_01   = gamma_01.second.second;
      
      // loop through indices. which to merge?
      // keep track in a vector
      std::vector<size_t> indices_to_merge_00;
      std::vector<size_t> indices_to_merge_01;
      
      for (auto const& clus : clus_angle_map) {
	
	auto clus_idx   = clus.first;
	auto clus_angle = clus.second;
	auto cluster    = clus_v.at(clus_idx);

	// skip clusters with less then threshold of hits
	if (cluster.size() < _min_nhits) continue;
	
	// don't merge cluster with itself!
	if ( (clus.first == gamma_00.first) || (clus.first == gamma_01.first) ) continue;
	
	// find cluster with respect to which angle is smaller
	double angle_small = fabs(clus_angle - gammaangle_00);
	double angle_large = fabs(clus_angle - gammaangle_01);
	int merge_with = 0; // which gamma to merge with? [0 -> 00, 1 -> 01]
	// flip if we got it wrong
	if (angle_small > angle_large){
	  angle_small = fabs(clus_angle - gammaangle_01);
	  angle_large = fabs(clus_angle - gammaangle_00);
	  merge_with = 1;
	}
	
	if (_verbose)
	  std::cout << "new cluster with " << cluster.size()
		    << " hits and angle " << clus_angle << std::endl
		    << "\t angle small : " << angle_small << "\t angle large : " << angle_large
		    << std::endl;
	
	// NOTE
	// below IF statements are not correct
	// when gammaangle_01 is undefined the angle_large requirement may cause
	// merging not to happen when it really should
	
	if (merge_with == 0) {
	  if ( (angle_small < _max_angle_diff_merge) and
	       (gammaangle_00 != 0) and
	       (angle_large > _min_gammagamma_oangle) ){
	    if ( ClusterDistance(cluster, gammacluster_00) > _max_merge_dist * gammacluster_00.Length() ) continue;
	    indices_to_merge_00.push_back( clus_idx );
	    if (_verbose) std::cout << "MERGE WITH 00" << std::endl;
	  }
	}
	
	if (merge_with == 1) {
	  if ( (angle_small < _max_angle_diff_merge) and
	       (gammaangle_01 != 0) and
	       (angle_large > _min_gammagamma_oangle) ){
	    if ( ClusterDistance(cluster, gammacluster_01) > _max_merge_dist * gammacluster_01.Length() ) continue;
	    indices_to_merge_01.push_back( clus_idx );
	    if (_verbose) std::cout << "MERGE WITH 01" << std::endl;
	  }
	}
	
      }// for all clusters
      
      if (_verbose) {
	std::cout << "identified " << indices_to_merge_00.size() << " clusters to merge with gamma00" << std::endl;
	std::cout << "identified " << indices_to_merge_01.size() << " clusters to merge with gamma01" << std::endl;
      }

      if (indices_to_merge_00.size()) {
	std::vector<size_t> indices_to_merge_v {gamma_00.first};
	for (auto const& idx : indices_to_merge_00) indices_to_merge_v.push_back( idx );
	merge_result.push_back( indices_to_merge_v );
      }
      
      if (indices_to_merge_01.size()) {
	std::vector<size_t> indices_to_merge_v {gamma_01.first};
	for (auto const& idx : indices_to_merge_01) indices_to_merge_v.push_back( idx );
	merge_result.push_back( indices_to_merge_v );
      }
	
    }// for all 3 planes



    return merge_result;
  }


    // calculate maximum gap between clusters
  float CBAlgoVtxAlign::ClusterDistance(const ::cluster::Cluster& c1,
					const ::cluster::Cluster& c2) {


    if ( (c1._start_pt._r > c2._start_pt._r) && (c1._start_pt._r < c2._end_pt._r) )
      return 0.;

    if ( (c2._start_pt._r > c1._start_pt._r) && (c2._start_pt._r < c1._end_pt._r) )
      return 0.;

    if (c1._start_pt._r > c2._end_pt._r) return c1._start_pt._r - c2._end_pt._r;
    
    if (c2._start_pt._r > c1._end_pt._r) return c2._start_pt._r - c1._end_pt._r;

    return -1;
  }

  
}

#endif
