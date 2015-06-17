/**
 * \file AlgoParentIsSiblingsSibling.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoParentIsSiblingsSibling
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    This algorithm is called when a node is found
    to have a parent and a sibling that are siblings
    with each other.
    Actions taken:
    the logical connection with the highest score is
    kept:
    - A) (parent <-> node)    + (sibling <-> node) : score X
    - B) (parent <-> sibling) + (sibling <-> node) : score Y
    Largest score (X or Y) determines which correlation
    is broken.
    - if (X > Y) -> keep configuration A) and remove
    parent <-> sibling correlation
    - if (Y > X) -> keep configuration B) and remove
    parent <-> node correlation
    @{*/

#ifndef ALGOPARENTISSIBLINGSSIBLING_H
#define ALGOPARENTISSIBLINGSSIBLING_H

#include "AlgoParentIsSiblingsSiblingBase.h"
//#include "AlgoBase.h"

namespace geotree{

  class AlgoParentIsSiblingsSibling : public AlgoParentIsSiblingsSiblingBase {

  public:
    
    AlgoParentIsSiblingsSibling() { _name="ParentIsSiblingsSibling"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoParentIsSiblingsSibling(NodeCollection* coll);

    void ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling);

  };
}

#endif
/** @} */ // end of doxygen group 
