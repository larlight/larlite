#ifndef ALGOGENERICCONFLICTREMOVESIBLING_CXX
#define ALGOGENERICCONFLICTREMOVESIBLING_CXX

#include "AlgoGenericConflictRemoveSibling.h"

namespace geotree{

  AlgoGenericConflictRemoveSibling::AlgoGenericConflictRemoveSibling(NodeCollection* coll)
  {
    _coll = coll;
    _name = "GenericConflictRemoveSibling";
  }

  void AlgoGenericConflictRemoveSibling::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    // since we are starting fresh, clear correlations currently stored
    ClearCorrelations();
    
    // prepare a std::pair holder
    std::pair<NodeID_t,NodeID_t> nodePair;

    // if sibling does not have the same parent -> remove sibling relation
    if (_coll->GetNode(sibling).hasParent() == false){
      if (_verbose) { std::cout << "\tsibling does not have the same parent. Remove sibling realtion " << std::endl; } 
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      nodePair = std::make_pair(id,sibling);
      _corr_v[nodePair] = corr;
    }
    
    // if parent exists but is different -> remove sibling relation
    else if (_coll->GetNode(sibling).getParent() != parent){
      if (_verbose) { std::cout << "\tsibling parent different from this node's parent. Remove sibling realtion " << std::endl; } 
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      nodePair = std::make_pair(id,sibling);
      _corr_v[nodePair] = corr;
    }

    return;
  }

}

#endif
