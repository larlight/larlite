#ifndef RECOTOOL_CMATCHMANAGER_CXX
#define RECOTOOL_CMATCHMANAGER_CXX

#include "CMatchManager.h"

namespace cmtool {

  CMatchManager::CMatchManager() : CMManagerBase()
  {
    throw CMTException("Default ctor needs # planes as an argument!");
  }

  CMatchManager::CMatchManager(size_t nplanes) : CMManagerBase()
  {
    _match_algo = nullptr;
    _nplanes    = nplanes;
    Reset();
  }

  void CMatchManager::Reset()
  {
    CMManagerBase::Reset();
    if(_match_algo) _match_algo->Reset();
    if(_priority_algo) _priority_algo->Reset();
    _book_keeper.Reset();
  }

  void CMatchManager::EventBegin()
  {
    if(_debug_mode <= kPerMerging) {
      if(_match_algo)    _match_algo->SetVerbose(true);
      if(_priority_algo) _priority_algo->SetVerbose(true);
    }

    if(_match_algo)_match_algo->IterationBegin(_in_clusters);
    if(_priority_algo) _priority_algo->IterationBegin(_in_clusters);
  }
  
  void CMatchManager::IterationBegin()
  {
    if(_match_algo) _match_algo->IterationBegin(_in_clusters);
    if(_priority_algo) _priority_algo->IterationBegin(_in_clusters);
  }
  
  void CMatchManager::IterationEnd()
  {
    if(_match_algo) _match_algo->IterationEnd();
    if(_priority_algo) _priority_algo->IterationEnd();
  }

  void CMatchManager::EventEnd()
  {
    if(_match_algo) _match_algo->EventEnd();
    if(_priority_algo) _priority_algo->EventEnd();
  }

  unsigned int CMFactorial(unsigned int n)
  {return (n == 1 || n == 0) ? 1 : CMFactorial(n - 1) * n;}
  
  std::vector<std::vector<size_t> > SimpleCombination(size_t n, size_t r) {
    
    if(!n || !r) exit(1);
    if(r > n) exit(1);
    
    std::vector<bool> v(n,false);
    std::fill(v.begin()+n-r,v.end(),true);
    std::vector<std::vector<size_t> > res;
    res.reserve(CMFactorial(n)/CMFactorial(n-r)/CMFactorial(r));
    
    do {
      std::vector<size_t> tmp;
      tmp.reserve(r);
      
      for(size_t i=0; i<n; ++i) { if(v[i]) tmp.push_back(i); }
      res.push_back(tmp);
    } while (std::next_permutation(v.begin(),v.end()));
    
    return res;
  }

  std::vector<std::vector<size_t> > ClusterCombinations(const std::vector<size_t>& seed)
  {
  
    std::vector<size_t> ctr(seed.size(),0);
    
    std::vector<std::vector<size_t> > res;
    
    while(1) {
      
      res.push_back(std::vector<size_t>(seed.size(),0));
      for(size_t index=0; index<ctr.size(); ++index)
        
        (*res.rbegin())[index] = ctr.at(index);
    
      for(size_t i=0; i<ctr.size(); ++i) {
        
        size_t index = (size_t)(ctr.size()-i-1);
        
        ctr.at(index) +=1;
        
        if(ctr.at(index) < seed.at(index))
          
          break;
        
        ctr.at(index) = 0;

      }

      bool abort = true;
      for(auto const& value : ctr)
        
        abort = abort && (!(value));
      
    if(abort) break;
    }
    return res;
  }

  std::vector<std::vector<std::pair<size_t,size_t> > > PlaneClusterCombinations(const std::vector<size_t>& seed)
  {
    // Result container
    std::vector<std::vector<std::pair<size_t,size_t> > > result;

    // Loop over N-planes: start from max number of planes => down to 2 planes
    for(size_t i=0; i<seed.size(); ++i) {
      
      // If finish making clusters down to 2 palnes, break
      if(seed.size() < 2+i) break;

      // Compute possible N-plane combinations
      auto const& plane_comb_v = SimpleCombination(seed.size(),seed.size()-i);

      // Loop over possible N-plane combinations
      for(auto const& plane_comb : plane_comb_v){

        // Make a seed for cluster combinations
        std::vector<size_t> cluster_seed_v;
        cluster_seed_v.reserve(plane_comb.size());
        for(auto const& index : plane_comb) cluster_seed_v.push_back(seed[index]);
        
        // Compute cluster combinations
        for(auto const& cluster_comb : ClusterCombinations(cluster_seed_v)) {
          
          // Store result
          result.push_back(std::vector<std::pair<size_t,size_t> >());
          for(size_t i=0; i<cluster_comb.size(); ++i)
            
            (*result.rbegin()).push_back(std::make_pair(plane_comb.at(i),cluster_comb.at(i)));
        }
      }
    }
    return result;
      
  }

  bool CMatchManager::IterationProcess()
  {

    TStopwatch localWatch;

    //
    // Create plane-by-plane vectors
    //
    ComputePriority(_in_clusters);

    if(_planes.size()<2) return false;

    if(_planes.size() > _nplanes)

      throw CMTException("Found more plane IDs than specified number of planes!");

    // Index array of clusters per plane
    std::vector<std::vector<size_t> > cluster_array;

    // Resize to # of planes w/ clusters
    cluster_array.reserve(_planes.size());

    // plane-to-index map
    std::vector<size_t> plane_to_index(_nplanes,_nplanes);

    // Fill plane-to-index map
    for(size_t plane=0; plane<_nplanes; ++plane){

      if( _planes.find(plane) != _planes.end() ) {

        plane_to_index[plane] = cluster_array.size();

        cluster_array.push_back(std::vector<size_t>());
      }
    }

    // Fill cluster_array
    for(auto riter = _priority.rbegin();
        riter != _priority.rend();
        ++riter) 

      cluster_array.at( plane_to_index.at(_in_clusters.at((*riter).second).plane_id.Plane) ).push_back((*riter).second);

    // Find combinations
    std::vector<size_t> seed;
    seed.reserve(cluster_array.size());
    for(auto const& clusters_per_plane : cluster_array)

      seed.push_back(clusters_per_plane.size());
    
    auto const& combinations = PlaneClusterCombinations(seed);
    
    // Loop over combinations and call algorithm
    for(auto const& comb : combinations) {

      std::vector<const cluster::cluster_params*> ptr_v;

      std::vector<unsigned int> tmp_index_v;

      tmp_index_v.reserve(comb.size());

      ptr_v.reserve(comb.size());

      for(auto const& plane_cluster : comb) {
        
        auto const& in_cluster_index = cluster_array.at(plane_cluster.first).at(plane_cluster.second);

        tmp_index_v.push_back(in_cluster_index);

        ptr_v.push_back(&(_in_clusters.at(in_cluster_index)));

      }

      if(_debug_mode <= kPerMerging){
        
        std::cout
          << "    \033[93m"
          << "Inspecting a pair (";
        for(auto const& index : tmp_index_v)
          std::cout << index << " ";
        std::cout<<") \033[00m" << std::flush;

        localWatch.Start();

      }
      
      auto const& score = _match_algo->Float(ptr_v);

      if(_debug_mode <= kPerMerging)

        std::cout << " ... Time taken = " << localWatch.RealTime() << " [s]" << std::endl;

      if(score>0)
        
        _book_keeper.Match(tmp_index_v,score);

    }
  
    if(_debug_mode <= kPerIteration) {
      if(_match_algo) _match_algo->Report();
      if(_priority_algo) _priority_algo->Report();
    }      

    return false;

  }

}

#endif
