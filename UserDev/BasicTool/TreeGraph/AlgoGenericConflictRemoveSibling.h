/**
 * \file AlgoGenericConflictRemoveSibling.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoGenericConflictRemoveSibling
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

#ifndef ALGOGENERICCONFLICTREMOVESIBLING_H
#define ALGOGENERICCONFLICTREMOVESIBLING_H

//#include "AlgoMultipleParentsBase.h"
#include "AlgoBase.h"

namespace geotree{

  class AlgoGenericConflictRemoveSibling : public AlgoBase {

  public:
    
    AlgoGenericConflictRemoveSibling() { _name="GenericConflictRemoveSibling"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoGenericConflictRemoveSibling(NodeCollection* coll);

    void ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling);

  };
}

#endif
/** @} */ // end of doxygen group 
