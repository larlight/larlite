/**
 * \file AlgoParentIsSiblingsSiblingBase.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoParentIsSiblingsSiblingBase
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    This algorithm is called when a node is found
    to have a parent and a sibling that are siblings
    with each other.
    Actions taken:
    None : Base Class
    @{*/

#ifndef ALGOPARENTISSIBLINGSSIBLINGBASE_H
#define ALGOPARENTISSIBLINGSSIBLINGBASE_H

//#include "AlgoMultipleParentsBase.h"
#include "AlgoBase.h"

namespace geotree{

  class AlgoParentIsSiblingsSiblingBase : public AlgoBase {

  public:
    
    AlgoParentIsSiblingsSiblingBase() { _name="ParentIsSiblingsSiblingBase"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoParentIsSiblingsSiblingBase(NodeCollection* coll);

    virtual void ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling);

  };
}

#endif
/** @} */ // end of doxygen group 
