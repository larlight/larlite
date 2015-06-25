/**
 * \file AlgoGenericConflictBase.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoGenericConflictBase
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    This algorithm is called when a "generic conflict" is found
    A "generic conflict" means: the node has both a parent and 
    a sibling.
    Actions taken:
    Relation with sibling is always removed. No check on other
    correlations with other nodes are performed.
    @{*/

#ifndef ALGOGENERICCONFLICTBASE_H
#define ALGOGENERICCONFLICTBASE_H

#include "AlgoBase.h"

namespace geotree{

  class AlgoGenericConflictBase : public AlgoBase {

  public:
    
    AlgoGenericConflictBase() { _name="GenericConflictBase"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoGenericConflictBase(NodeCollection* coll);

    virtual void ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling);

  };
}

#endif
/** @} */ // end of doxygen group 
