#ifndef RECOTOOL_CBALGOVTXALIGN2_CXX
#define RECOTOOL_CBALGOVTXALIGN2_CXX

#include "CBAlgoVtxAlign2.h"
#include <map>
#include <algorithm>

namespace cmtool {

  //----------------------------------------
  CBAlgoVtxAlign2::CBAlgoVtxAlign2() : CBoolAlgoBase()
  //----------------------------------------
  {
    _max_angle_diff_merge  = 0.;
    _min_parpar_oangle = 0.;
    _min_nhits = 0;

    _inv_int = std::numeric_limits<int>::max();
    _inv_float = std::numeric_limits<float>::max();
    _inv_double = std::numeric_limits<double>::max();
    _inv_size = std::numeric_limits<size_t>::max();
  }

  std::vector< std::vector<size_t> > CBAlgoVtxAlign2::Merge(const std::vector<cluster::Cluster>& clus_v) {

    std::vector< std::vector<size_t> > merge_result;

    for (size_t pl=0; pl < 3; pl++) {

      if (_verbose)
	std::cout << std::endl << std::endl << std::endl << "****** PLANE " << pl << " *****" << std::endl << std::endl << std::endl << std::endl;

      // split clusters per plane:
      // and order by size
      std::vector< std::pair<float,size_t> > plane_clus_pairs_v; // pair [size, index]
      for (size_t i=0; i < clus_v.size(); i++) {
	if (clus_v[i]._plane != pl) continue;
	plane_clus_pairs_v.push_back( std::make_pair( clus_v[i]._start_pt._r , i ) );
      }

      std::sort(plane_clus_pairs_v.begin(), plane_clus_pairs_v.end());

      std::vector<size_t> plane_clus_idx_v;
      for (size_t i=0; i < plane_clus_pairs_v.size(); i++)
	plane_clus_idx_v.push_back( plane_clus_pairs_v[i].second );

      // pair links cluster index to pair of ( NHits, angle )
      // object will hold cluster angle for cluster with largest number of hits per plane
      std::pair< size_t, std::pair<float, double> > par_00, par_01;
      par_00 = par_01 = std::make_pair(0,std::make_pair(_inv_float, 0));

      // map linking cluster index and angle
      std::map<size_t, double> clus_angle_map;

      for (auto const& idx : plane_clus_idx_v) {	

	auto const& cluster = clus_v.at(idx);
	
	auto angle = cluster._angle;
	auto r     = cluster._start_pt._r;

	clus_angle_map[ idx ] = angle;

	if (_verbose){
	  std::cout << "New cluster @ Plane " << pl
		    << "\t nhits = " << cluster.size()
		    << "\t r = " << r
		    << "\t (w,t) = (" << cluster._start_pt._w << "," << cluster._start_pt._t << ")"
		    << "\t angle = " << cluster._angle << std::endl
		    << "Current par00 angle : " << par_00.second.second << "\t par01 angle : " << par_01.second.second << std::endl;
	}

	if ( r < par_00.second.first ) {
	  if (_verbose) std::cout << "\t updating par00" << std::endl;
	  par_00.second.first  = r; // update # of hits for largest cluster in plane
	  par_00.second.second = angle; // update direction from PCA for largest cluster in plane
	  par_00.first         = idx;   // update index
	}
	
	if ( ( r < par_01.second.first ) &&
	     ( fabs(angle-par_00.second.second ) > _min_parpar_oangle) &&
	     ( par_00.second.second != 0 ) ) {
	  if (_verbose) std::cout << "\t updating par01" << std::endl;
	  par_01.second.first  = r; // update # of hits for largest cluster in plane
	  par_01.second.second = angle; // update direction from PCA for largest cluster in plane
	  par_01.first         = idx;   // update index
	}
	
	if (_verbose) std::cout << std::endl << std::endl;

      }// for all clusters on this plane

      if (_verbose)
	std::cout << " par00 @ r = " << par_00.second.first
		  << " and " << par_00.second.second << " angle" << std::endl;
      if (_verbose)
	std::cout << " par01 @ r = " << par_01.second.first
		  << " and " << par_01.second.second << " angle" << std::endl;
      
      // "big" cluster 00
      auto parcluster_00 = clus_v.at( par_00.first );
      auto parangle_00   = par_00.second.second;
      
      // "big" cluster 01
      auto parcluster_01 = clus_v.at( par_01.first );
      auto parangle_01   = par_01.second.second;
      
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
	if ( (clus.first == par_00.first) || (clus.first == par_01.first) ) continue;
	
	// find cluster with respect to which angle is smaller
	double angle_small = fabs(clus_angle - parangle_00);
	double angle_large = fabs(clus_angle - parangle_01);
	int merge_with = 0; // which par to merge with? [0 -> 00, 1 -> 01]
	// flip if we got it wrong
	if (angle_small > angle_large){
	  angle_small = fabs(clus_angle - parangle_01);
	  angle_large = fabs(clus_angle - parangle_00);
	  merge_with = 1;
	}
	
	if (_verbose)
	  std::cout << "new cluster with " << cluster.size() << " hits"
		    << " @r=" << cluster._start_pt._r
		    << " and angle " << clus_angle << std::endl
		    << "\t angle small : " << angle_small << "\t angle large : " << angle_large
		    << std::endl;
	
	// NOTE
	// below IF statements are not correct
	// when parangle_01 is undefined the angle_large requirement may cause
	// merging not to happen when it really should
	
	if (merge_with == 0) {
	  if ( (angle_small < _max_angle_diff_merge) and
	       (parangle_00 != 0) and
	       (angle_large > _min_parpar_oangle) ){
	    if ( ClusterDistance(cluster, parcluster_00) > _max_merge_dist * parcluster_00.Length() ) continue;
	    indices_to_merge_00.push_back( clus_idx );
	    if (_verbose) std::cout << "MERGE WITH 00" << std::endl;
	  }
	}
	
	if (merge_with == 1) {
	  if ( (angle_small < _max_angle_diff_merge) and
	       (parangle_01 != 0) and
	       (angle_large > _min_parpar_oangle) ){
	    if ( ClusterDistance(cluster, parcluster_01) > _max_merge_dist * parcluster_01.Length() ) continue;
	    indices_to_merge_01.push_back( clus_idx );
	    if (_verbose) std::cout << "MERGE WITH 01" << std::endl;
	  }
	}
	
      }// for all clusters
      
      if (_verbose) {
	std::cout << "identified " << indices_to_merge_00.size() << " clusters to merge with par00" << std::endl;
	std::cout << "identified " << indices_to_merge_01.size() << " clusters to merge with par01" << std::endl;
      }

      if (indices_to_merge_00.size()) {
	std::vector<size_t> indices_to_merge_v {par_00.first};
	for (auto const& idx : indices_to_merge_00) indices_to_merge_v.push_back( idx );
	merge_result.push_back( indices_to_merge_v );
      }
      
      if (indices_to_merge_01.size()) {
	std::vector<size_t> indices_to_merge_v {par_01.first};
	for (auto const& idx : indices_to_merge_01) indices_to_merge_v.push_back( idx );
	merge_result.push_back( indices_to_merge_v );
      }
	
    }// for all 3 planes


    return merge_result;
  }


    // calculate maximum gap between clusters
  float CBAlgoVtxAlign2::ClusterDistance(const cluster::Cluster& c1,
					const cluster::Cluster& c2) {


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
