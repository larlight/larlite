#ifndef RECOTOOL_CBALGOFAKE_CXX
#define RECOTOOL_CBALGOFAKE_CXX

#include "CBAlgoFake.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoFake::CBAlgoFake() : CBoolAlgoBase()
  //----------------------------------------
  {
    _flip = false;
    _ctr  = 0;
    // Nothing to be done in the base class
  }

  //--------------------------------------------------------
  bool CBAlgoFake::Bool(const ::cluster::Cluster &cluster1,
                        const ::cluster::Cluster &cluster2)
  //--------------------------------------------------------
  {

    std::cout << "Pair-wise called" << std::endl;
    
    if(cluster1.size() && cluster2.size()) {
      _ctr++;
      if( (_ctr%64) == 0)
        _flip = (!_flip);
      return _flip;
    }
    else return false;
  }

  std::vector< std::vector<size_t> > CBAlgoFake::Merge(const std::vector<::cluster::Cluster>& clus_v) {

    std::cout << "Full view merging called" << std::endl;

    std::vector< std::vector<size_t> > merge_result{ {}, {}, {} };

    for (size_t clus_idx = 0; clus_idx < clus_v.size(); clus_idx++) {

      auto const& clus = clus_v.at(clus_idx);

      merge_result[clus._plane].push_back(clus_idx);
      
    }

    return merge_result;
  }

  
}

#endif
