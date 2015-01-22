#ifndef RECOTOOL_CMERGEMANAGER_CXX
#define RECOTOOL_CMERGEMANAGER_CXX

#include "CMergeManager.h"

namespace cmtool {

  CMergeManager::CMergeManager() : CMManagerBase()
  {
    _iter_ctr=0;
    _merge_algo = nullptr;
    _separate_algo = nullptr;
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
    if(_merge_algo)    _merge_algo->Reset();
    if(_separate_algo) _separate_algo->Reset();
    _iter_ctr = 0;
  }

  /// FMWK function called @ beginning of Process()
  void CMergeManager::EventBegin()
  {
    // Initialization per event
    if(!_merge_algo) throw CMTException("No algorithm to run!");

    // Merging algorithm
    _merge_algo->SetAnaFile(_fout);
    _merge_algo->EventBegin(_in_clusters);

    // Separation algorithm
    if(_separate_algo) {
      _separate_algo->SetAnaFile(_fout);
      _separate_algo->EventBegin(_in_clusters);
    }

    // Priority ordering algorithm
    if(_priority_algo) {
      _priority_algo->SetAnaFile(_fout);
      _priority_algo->EventBegin(_in_clusters);
    }

    // Verbosity setting
    if(_debug_mode <= kPerMerging) {
      _merge_algo->SetVerbose(true);
      if(_separate_algo) _separate_algo->SetVerbose(true);
      if(_priority_algo) _priority_algo->SetVerbose(true);
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

    if(!_iter_ctr) {

      _merge_algo->IterationBegin(_in_clusters);
      if(_separate_algo) _separate_algo->IterationBegin(_in_clusters);
      if(_priority_algo) _priority_algo->IterationBegin(_in_clusters);

    }else{

      _merge_algo->IterationBegin(_in_clusters);
      if(_separate_algo) _separate_algo->IterationBegin(_out_clusters);
      if(_priority_algo) _priority_algo->IterationBegin(_out_clusters);

    }

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

    _merge_algo->IterationEnd();
    if(_separate_algo) _separate_algo->IterationEnd();
    if(_priority_algo) _priority_algo->IterationEnd();

    if(_debug_mode <= kPerIteration) {

      _merge_algo->Report();
      if(_separate_algo) _separate_algo->Report();
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

    // Call EventEnd for algorithms
    _merge_algo->EventEnd();
    if(_separate_algo) _separate_algo->EventEnd();
    if(_priority_algo) _priority_algo->EventEnd();

    _book_keeper_v.clear();
    _tmp_merged_clusters.clear();
    _tmp_merged_indexes.clear();
  }

  bool CMergeManager::IterationProcess()
  {

    // Configure input for RunMerge
    CMergeBookKeeper bk;
    
    if(!_iter_ctr) _tmp_merged_clusters = _in_clusters;
    else _tmp_merged_clusters = _out_clusters;
    _out_clusters.clear();

    bk.Reset(_tmp_merged_clusters.size());

    std::vector<bool> merge_switch(_tmp_merged_clusters.size(),true);

    for(size_t i=0; i<_tmp_merged_indexes.size(); ++i)
      
      if(_tmp_merged_indexes.at(i).size()==1)
	
	merge_switch.at(i) = false;
    
    ComputePriority(_tmp_merged_clusters);
    
    // Run separation algorithm
    if(_separate_algo)
      
      RunSeparate(_tmp_merged_clusters, bk);

    // Run merging algorithm
    RunMerge(_tmp_merged_clusters, merge_switch, bk);
    if(_debug_mode <= kPerIteration)
      
      _merge_algo->Report();
    
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
	  tmp_hit_counts += _tmp_merged_clusters.at(index).GetHitVector().size();
	std::vector<larutil::PxHit> tmp_hits;
	tmp_hits.reserve(tmp_hit_counts);
	
	for(auto const& index : indexes_v) {
	  for(auto const& hit : _tmp_merged_clusters.at(index).GetHitVector())
	    tmp_hits.push_back(hit);
	}
	_out_clusters.push_back(::cluster::ClusterParamsAlg());
	(*_out_clusters.rbegin()).SetVerbose(false);
	(*_out_clusters.rbegin()).DisableFANN();
	
	if((*_out_clusters.rbegin()).SetHits(tmp_hits) < 1) continue;
	(*_out_clusters.rbegin()).FillParams(true,true,true,true,true,false);
	(*_out_clusters.rbegin()).FillPolygon();
      }
      _book_keeper_v.push_back(bk);
    }

    // Break if no more merging occurred
    if(_tmp_merged_clusters.size() == _out_clusters.size())
      
      return false;

    if(_out_clusters.size() == _planes.size())

      return false;
    
    return true;
  }

  void CMergeManager::RunMerge(const std::vector<cluster::ClusterParamsAlg> &in_clusters,
			       CMergeBookKeeper &book_keeper) const
  {
    RunMerge(in_clusters,
	     std::vector<bool>(in_clusters.size(),true),
	     book_keeper);
  }

  void CMergeManager::RunMerge(const std::vector<cluster::ClusterParamsAlg> &in_clusters,
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
    
    // Run over clusters and execute merging algorithms
    for(auto citer1 = _priority.rbegin();
	citer1 != _priority.rend();
	++citer1) {
      
      auto citer2 = citer1;

      UChar_t plane1 = in_clusters.at((*citer1).second).Plane();

      while(1) {
	citer2++;
	if(citer2 == _priority.rend()) break;

	// Skip if not on the same plane
	UChar_t plane2 = in_clusters.at((*citer2).second).Plane();
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
	
	bool merge = _merge_algo->Bool(in_clusters.at((*citer1).second),in_clusters.at((*citer2).second));

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

  void CMergeManager::RunSeparate(const std::vector<cluster::ClusterParamsAlg> &in_clusters,
				  CMergeBookKeeper &book_keeper) const
  {
    /*
    if(separate_flag.size() != in_clusters.size())
      throw CMTException(Form("in_clusters (%zu) and separate_flag (%zu) vectors must be of same length!",
				   in_clusters.size(),
				   separate_flag.size()
				   )
			      );
    */
    if(_debug_mode <= kPerIteration){
      
      std::cout
	<< Form("  Calling %s with %zu clusters...",__FUNCTION__,in_clusters.size())
	<<std::endl;

    }

    //
    // Separation
    //
    
    // Run over clusters and execute merging algorithms
    for(size_t cindex1 = 0; cindex1 < in_clusters.size(); ++cindex1) {
      
      UChar_t plane1 = in_clusters.at(cindex1).Plane();
      
      for(size_t cindex2 = cindex1+1; cindex2 < in_clusters.size(); ++cindex2) {
	
	// Skip if not on the same plane
	UChar_t plane2 = in_clusters.at(cindex2).Plane();
	if(plane1 != plane2) continue;
	
	// Skip if this combination is not meant to be compared
	//if(!(separate_flag.at(cindex2))) continue;
	
	if(_debug_mode <= kPerMerging){
	  
	  std::cout
	    << Form("    \033[93mInspecting a pair (%zu, %zu) for separation... \033[00m",cindex1,cindex2)
	    << std::endl;
	}
	
	bool separate = _separate_algo->Bool(in_clusters.at(cindex1),in_clusters.at(cindex2));
	
	if(_debug_mode <= kPerMerging) {
	  
	  if(separate) 
	    std::cout << "    \033[93mfound to be separated!\033[00m " 
		      << std::endl
		      << std::endl;
	  
	  else 
	    std::cout << "    \033[93mfound NOT to be separated...\033[00m" 
		      << std::endl
		      << std::endl;
	  
	} // end looping over all sets of algorithms
	
	if(separate)
	  
	  book_keeper.ProhibitMerge(cindex1,cindex2);
	  
      } // end looping over all cluster pairs for citer1
      
    } // end looping over clusters

  }

}

#endif
