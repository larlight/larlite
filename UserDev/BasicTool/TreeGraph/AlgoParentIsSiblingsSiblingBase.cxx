#ifndef ALGOPARENTISSIBLINGSSIBLINGBASE_CXX
#define ALGOPARENTISSIBLINGSSIBLINGBASE_CXX

#include "AlgoParentIsSiblingsSiblingBase.h"

namespace geotree{

  AlgoParentIsSiblingsSiblingBase::AlgoParentIsSiblingsSiblingBase(NodeCollection* coll)
  {
    _coll = coll;
    _name = "ParentIsSiblingsSiblingBase";
  }

  void AlgoParentIsSiblingsSiblingBase::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    return;
  }

}

#endif
