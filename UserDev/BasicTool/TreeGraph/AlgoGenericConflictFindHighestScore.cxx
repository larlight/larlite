#ifndef ALGOGENERICCONFLICTFINDHIGHESTSCORE_CXX
#define ALGOGENERICCONFLICTFINDHIGHESTSCORE_CXX

#include "AlgoGenericConflictFindHighestScore.h"

namespace geotree{

  AlgoGenericConflictFindHighestScore::AlgoGenericConflictFindHighestScore(NodeCollection* coll)
  {
    _coll = coll;
    _name = "GenericConflictFindHighestScore";
  }

  void AlgoGenericConflictFindHighestScore::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    // since we are starting fresh, clear correlations currently stored
    ClearCorrelations();
    
    // prepare a std::pair holder
    std::pair<NodeID_t,NodeID_t> nodePair;

    // get parent and sibling score
    double parentScore  = _coll->GetNode(id).getScore(parent);
    double siblingScore = _coll->GetNode(id).getScore(sibling);

    // if parent score is higher than sibling's
    if (parentScore > siblingScore){
      if (_verbose) { std::cout << "\tParent's score is larger than sibling's. Remove corr. w/ sibling " << std::endl; } 
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      nodePair = std::make_pair(id,sibling);
      _corr_v[nodePair] = corr;
    }
    else{
      // if sibling's score is larger
      if (_verbose) { std::cout << "\tSibling's score is larger than parent's. Remove corr. w/ parent" << std::endl; } 
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      nodePair = std::make_pair(id,parent);
      _corr_v[nodePair] = corr;
    }
    
    return;
  }

}

#endif
