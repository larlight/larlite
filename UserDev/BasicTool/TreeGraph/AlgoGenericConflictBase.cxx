#ifndef ALGOGENERICCONFLICTBASE_CXX
#define ALGOGENERICCONFLICTBASE_CXX

#include "AlgoGenericConflictBase.h"

namespace geotree{

  AlgoGenericConflictBase::AlgoGenericConflictBase(NodeCollection *coll)
  {
    _coll = coll;
    _name = "GenericConflictBase";
  }

  void AlgoGenericConflictBase::ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling)
  {

    return;
  }

}

#endif
