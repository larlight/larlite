#ifndef SPAMANAGER_CXX
#define SPAMANAGER_CXX

#include "SPAManager.h"

namespace sptool {

  SPAManager::SPAManager() 
    : _algo   (nullptr)
    , _filter (nullptr)
  {}

  std::vector<std::pair<float,SPMatch_t> > SPAManager::Process(const SPAData &data)
  {
    if(!_algo) 

      throw SPAException("Algorithm is not set!");

    // Result container to be returned
    std::vector<std::pair<float,SPMatch_t> > result;

    // Return if # showers is less than needed
    if(_algo->_nshowers > data._showers.size()) return result;

    //
    // First, find out valid showers among input by running SPAFilter
    //
    auto order = _filter->Select(data);

    // Return if # remaining showers is less than needed
    if( data._showers.size() < order._rm_showers.size() + _algo->_nshowers ) return result;

    // Create a maping to the original data's _showers vector index
    std::vector<size_t> shower_index_map;
    shower_index_map.reserve(data._showers.size() - order._rm_showers.size());
    for(size_t i=0; i<data._showers.size(); ++i) {
      if(order._rm_showers.find(i) == order._rm_showers.end())
	shower_index_map.push_back(i);
    }

    // Create a reduced copy
    SPAData red_data(data);
    red_data.ApplyOrder(order);

    if(shower_index_map.size() != red_data._showers.size())
      throw SPAException("<<Process>> Logic error: reduced data's # showers and index map entries do not match!");

    // Create a copy that will be handed to the algorithms
    SPAData input(red_data);
    input._showers.clear();

    //
    // Loop over valid shower indexies and call SPAlgo
    //
    std::multimap<float,SPMatch_t> score_map;
    std::vector<bool> comb_index_v(red_data._showers.size());
    std::fill(comb_index_v.begin() + _algo->_nshowers, comb_index_v.end(), true);

    do {

      std::vector<size_t> index_v;
      index_v.reserve(_algo->_nshowers);

      input._showers.clear();
      input._showers.reserve(_algo->_nshowers);

      for (int i = 0; i < comb_index_v.size(); ++i)
	
	if (!comb_index_v[i]) {
	  input._showers.push_back(red_data._showers[i]);
	  index_v.push_back(shower_index_map[i]);
	}

      score_map.insert(std::make_pair(_algo->Select(input),index_v));

    } while (std::next_permutation(comb_index_v.begin(), comb_index_v.end()));

    //
    // Now return result. Do not allow duplicate usage of the same shower.
    //
    std::set<size_t> used_index;
    for(auto iter = score_map.rbegin();
	iter != score_map.rend();
	++iter) {
      
      if((*iter).first<0) break;

      bool pass = false;
      for(auto const& index : (*iter).second)
	
	if(used_index.find(index) != used_index.end()) {
	  pass = true;
	  break;
	}

      if(pass) continue;
      
      result.push_back(std::make_pair((*iter).first,(*iter).second));
    }

    return result;
  }

}
#endif
