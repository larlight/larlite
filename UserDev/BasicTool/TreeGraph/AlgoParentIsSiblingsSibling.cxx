#ifndef ALGOPARENTISSIBLINGSSIBLING_CXX
#define ALGOPARENTISSIBLINGSSIBLING_CXX

#include "AlgoParentIsSiblingsSibling.h"

namespace geotree{

  AlgoParentIsSiblingsSibling::AlgoParentIsSiblingsSibling(NodeCollection* coll)
  {
    _coll = coll;
    _name = "ParentIsSiblingsSibling";
  }

  void AlgoParentIsSiblingsSibling::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    // since we are starting fresh, clear correlations currently stored
    ClearCorrelations();

    // prepare an std::pair to determine nodes for which correlation should be applied
    std::pair<NodeID_t,NodeID_t> nodePair;

    // we made it this far -> we have a problem!
    // 3 nodes: id, sibling, parent
    // scores to compare:
    // A) (id,parent) + (id,sibling)     -> this node is child of parent and sibling of s
    // B) (id,parent) + (parent,sibling) -> this node is child of parent and sibling is related to parent, not to this node
    // whichever is larger wins
    // basically compare (id,sibling) and (parent,sibling)
    double A = _coll->GetNode(id).getScore(parent);
    double B = _coll->GetNode(parent).getScore(sibling);
    if (A > B){
      if (_verbose) { std::cout << "keep sibling. Remove relation between parent and sibling" << std::endl; }
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      nodePair = std::make_pair(parent,sibling);
      _corr_v[nodePair] = corr;
    }
    else{
      if (_verbose) { std::cout << "keep parent. Remove relation with sibling" << std::endl; }
      Correlation corr(-1, larlite::geoalgo::Point_t(), ::geotree::RelationType_t::kUnknown);
      nodePair = std::make_pair(id,sibling);
      _corr_v[nodePair] = corr;
    }

    return;
  }

}

#endif
