#ifndef RECOTOOL_CMERGEMANAGER_CXX
#define RECOTOOL_CMERGEMANAGER_CXX

#include "CMergeManager.h"

namespace cmtool {

  CMergeManager::CMergeManager() : CMManagerBase()
  {
    _iter_ctr=0;
    _merge_algo_v.clear();
    Reset();
  }

  void CMergeManager::Reset()
  {
    CMManagerBase::Reset();
    _tmp_merged_clusters.clear();
    _tmp_merged_indexes.clear();
    _out_clusters.clear();
    _book_keeper.Reset();
    _book_keeper_v.clear();
    if(_merge_algo_v.size()) {
      for (size_t i=0; i < _merge_algo_v.size(); i++)
	_merge_algo_v[i]->Reset();
    }
    _iter_ctr = 0;
  }

  /// FMWK function called @ beginning of Process()
  void CMergeManager::EventBegin()
  {
    // Initialization per event
    if(_merge_algo_v.size() == 0) throw CMTException("No algorithm to run!");

    // Merging algorithm
    for (auto algo : _merge_algo_v)
      algo->SetAnaFile(_fout);

    // Priority ordering algorithm
    if(_priority_algo) {
      _priority_algo->SetAnaFile(_fout);
    }
    
    // Book keeper reinitialization
    _book_keeper.Reset(_in_clusters.size());

    // Clear temporary variables
    _iter_ctr = 0;
    _tmp_merged_clusters.clear();
    _tmp_merged_indexes.clear();
    _book_keeper_v.clear();

  }
  
  /// FMWK function called @ beginning of iterative loop inside Process()
  void CMergeManager::IterationBegin()
  {

    if(_debug_mode <= kPerIteration) {
      
      size_t nclusters = _tmp_merged_clusters.size();

      if(!_iter_ctr) nclusters = _in_clusters.size();
      
      std::cout 
        << std::endl
        << Form("  Merging iteration %zu: processing %zu clusters...",_iter_ctr,nclusters)
        << std::endl;

    }

  }
  
  /// FMWK function called @ end of iterative loop inside Process()
  void CMergeManager::IterationEnd()
  {

    if(_debug_mode <= kPerIteration) {

      for (size_t i=0; i < _merge_algo_v.size(); i++)
	_merge_algo_v[i]->Report();
      if(_priority_algo) _priority_algo->Report();

      std::cout
        << Form("  Input / Output cluster length: %zu/%zu",_tmp_merged_clusters.size(),_out_clusters.size())
        << std::endl;
      
      if(_tmp_merged_clusters.size() == _out_clusters.size())
        
        std::cout << "  Did not find any newly merged clusters..." <<std::endl;
      
      if(_out_clusters.size()==1)
        
        std::cout << "  Output cluster length is 1 (= no more merging needed)" << std::endl;
      
      if(!_merge_till_converge) 
        
        std::cout << "  Non-iterative option: terminating..." << std::endl;
      
    }

    _iter_ctr++;
  }

  /// FMWK function called @ end of Process()
  void CMergeManager::EventEnd()
  {
    // Gather the full book keeping result
    for(auto const& bk : _book_keeper_v)

      _book_keeper.Combine(bk);

    _book_keeper_v.clear();
    _tmp_merged_clusters.clear();
    _tmp_merged_indexes.clear();
  }

  bool CMergeManager::IterationProcess()
  {

    // Configure input for RunMerge
    CMergeBookKeeper bk;

    size_t algo_idx = 0.;

    size_t iter_ctr = 0;

    while (algo_idx < _merge_algo_v.size() ) {
    
      if(!iter_ctr) _tmp_merged_clusters = _in_clusters;
      else _tmp_merged_clusters = _out_clusters;
      _out_clusters.clear();
      
      bk.Reset(_tmp_merged_clusters.size());
      
      std::vector<bool> merge_switch(_tmp_merged_clusters.size(),true);
      
      for(size_t i=0; i<_tmp_merged_indexes.size(); ++i)
	
	if(_tmp_merged_indexes.at(i).size()==1)
	  
	  merge_switch.at(i) = false;
      
      ComputePriority(_tmp_merged_clusters);
      
      // Run merging algorithm
      RunMerge(algo_idx,_tmp_merged_clusters, merge_switch, bk);
      
      if(_debug_mode <= kPerIteration) 
	_merge_algo_v[algo_idx]->Report();
      
      // Save output
      bk.PassResult(_tmp_merged_indexes);
      
      if(bk.size() == _tmp_merged_indexes.size())
	_out_clusters = _tmp_merged_clusters;
      else {
	_out_clusters.reserve(_tmp_merged_indexes.size());
	for(auto const& indexes_v : _tmp_merged_indexes) {
	  
	  if(indexes_v.size()==1) {
	    _out_clusters.push_back(_tmp_merged_clusters.at(indexes_v.at(0)));
	    continue;
	  }
	  
	  size_t tmp_hit_counts=0;
	  for(auto const& index : indexes_v) 
	    tmp_hit_counts += _tmp_merged_clusters.at(index).size();
	  std::vector<cluster::pt> tmp_hits;
	  tmp_hits.reserve(tmp_hit_counts);
	  
	  for(auto const& index : indexes_v) {
	    for(auto const& hit : _tmp_merged_clusters.at(index).GetHits())
	      tmp_hits.push_back(hit);
	  }
	  _out_clusters.push_back(::cluster::Cluster());
	  
	  if((*_out_clusters.rbegin()).SetHits(tmp_hits) < 1) continue;
	}
	_book_keeper_v.push_back(bk);
      }

      // if no merging occurred -> move to next algo
      if(_tmp_merged_clusters.size() == _out_clusters.size())
	algo_idx += 1;

      if( (_tmp_merged_clusters.size() != _out_clusters.size()) && (_merge_algo_v[algo_idx]->MergeTillConverge() == false) )
	algo_idx += 1;

      iter_ctr += 1;
      
    }// for all meging algorithms
      
    return true;
  }

  void CMergeManager::RunMerge(const int& algo_idx,
			       const std::vector<::cluster::Cluster> &in_clusters,
                               CMergeBookKeeper &book_keeper) const
  {
    RunMerge(algo_idx,in_clusters,
             std::vector<bool>(in_clusters.size(),true),
             book_keeper);
  }

  void CMergeManager::RunMerge(const int& algo_idx,
			       const std::vector<::cluster::Cluster> &in_clusters,
                               const std::vector<bool> &merge_flag,
                               CMergeBookKeeper &book_keeper) const
  {
    if(merge_flag.size() != in_clusters.size())
      throw CMTException(Form("in_clusters (%zu) and merge_flag (%zu) vectors must be of same length!",
                                   in_clusters.size(),
                                   merge_flag.size()
                                   )
                              );
    if(_debug_mode <= kPerIteration){
      
      std::cout
        << Form("  Calling %s with %zu clusters...",__FUNCTION__,in_clusters.size())
        <<std::endl;

    }

    //
    // Merging
    //

    // which mode? pair-wise:
    if (_merge_algo_v[algo_idx]->PairWiseMode() == true) {
      
      // Run over clusters and execute merging algorithms
      for(auto citer1 = _priority.rbegin();
	  citer1 != _priority.rend();
	  ++citer1) {
	
	auto citer2 = citer1;
	
	UChar_t plane1 = in_clusters.at((*citer1).second)._plane;
	
	while(1) {
	  citer2++;
	  if(citer2 == _priority.rend()) break;
	  
	  // Skip if not on the same plane
	  UChar_t plane2 = in_clusters.at((*citer2).second)._plane;
	  if(plane1 != plane2) continue;
	  
	  // Skip if this combination is not meant to be compared
	  if(!(merge_flag.at((*citer2).second)) && !(merge_flag.at((*citer1).second)) ) continue;
	  
	  // Skip if this combination is not allowed to merge
	  if(!(book_keeper.MergeAllowed((*citer1).second,(*citer2).second))) continue;
	  
	  if(_debug_mode <= kPerMerging){
	    
	    std::cout
	      << Form("    \033[93mInspecting a pair (%zu, %zu) for merging... \033[00m",(*citer1).second, (*citer2).second)
	      << std::endl;
	  }
	  
	  bool merge = _merge_algo_v[algo_idx]->Bool(in_clusters.at((*citer1).second),in_clusters.at((*citer2).second));
	  
	  if(_debug_mode <= kPerMerging) {
	    
	    if(merge) 
	      std::cout << "    \033[93mfound to be merged!\033[00m " 
			<< std::endl
			<< std::endl;
	    
	    else 
	      std::cout << "    \033[93mfound NOT to be merged...\033[00m" 
			<< std::endl
			<< std::endl;
	    
	  } // end looping over all sets of algorithms
	  
	  if(merge)
	    
	    book_keeper.Merge((*citer1).second,(*citer2).second);
	  
	} // end looping over all cluster pairs for citer1
	
      } // end looping over clusters

    }// if pair-wise mode


    // which mode? if all clusters together:
    else {

      auto merge_clus_idx_v = _merge_algo_v[algo_idx]->Merge(in_clusters);

      for (auto const& merge_idx_list : merge_clus_idx_v) {

	// merge_idx_list contains list of all clusters to be merged together.
	for (size_t n = 0; n < merge_idx_list.size(); n++) {
	  for (size_t m = n+1; m < merge_idx_list.size(); m++) {
	    book_keeper.Merge( merge_idx_list[n], merge_idx_list[m]);
	  }
	}
	
      }// for all clusters in list.
      
    }// if merge from full event info
    
    
    if(_debug_mode <= kPerIteration && book_keeper.GetResult().size() != in_clusters.size()) {
      
      std::cout << "  Found following clusters to be merged..." << std::endl;
      for(auto const &indexes_v : book_keeper.GetResult()) {

        if(indexes_v.size()==1) continue;
        std::cout<<"    ";
        for(auto index : indexes_v)

          std::cout<<index<<" ";
        std::cout<<" ... indexes to be merged!"<<std::endl;

      }
    }

  }

}

#endif
