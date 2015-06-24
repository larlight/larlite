/**
 * \file AlgoGenericConflictFindHighestScore.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoGenericConflictFindHighestScore
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    This algorithm is called when a "generic conflict" is found
    A "generic conflict" means: the node has both a parent and 
    a sibling.
    Actions taken:
    The node's correlation's score with the sibling and parent
    is found. Whichever correlation has the lowest score is
    always removed. No check on other correlations with other
    nodes are performed.
    @{*/

#ifndef ALGOGENERICCONFLICTFINDHIGHESTSCORE_H
#define ALGOGENERICCONFLICTFINDHIGHESTSCORE_H

#include "AlgoGenericConflictBase.h"

namespace geotree{

  class AlgoGenericConflictFindHighestScore : public AlgoGenericConflictBase {

  public:
    
    AlgoGenericConflictFindHighestScore() { _name="GenericConflictFindHighestScore"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoGenericConflictFindHighestScore(NodeCollection* coll);

    void ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling);

  };
}

#endif
/** @} */ // end of doxygen group 
