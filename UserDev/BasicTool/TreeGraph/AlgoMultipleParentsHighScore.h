/**
 * \file AlgoMultipleParentsHighScore.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoMultipleParentsHighScore
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    This algorithm is called when a node was found to
    have multiple parents.
    Actions taken:
    The correlation with the parent with the highest
    score is retained. All other parent correlations
    are removed. No check on other correlations with
    other nodes are performed.
    @{*/

#ifndef ALGOMULTIPLEPARENTSHIGHSCORE_H
#define ALGOMULTIPLEPARENTSHIGHSCORE_H

#include "AlgoMultipleParentsBase.h"
//#include "AlgoBase.h"

namespace geotree{

  class AlgoMultipleParentsHighScore : public AlgoMultipleParentsBase {

  public:

    
    AlgoMultipleParentsHighScore() { _name="MultipleParentsHighScore"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoMultipleParentsHighScore(NodeCollection* coll);

    void FindBestParent(const NodeID_t& id, const std::vector<NodeID_t>& parents);

  };
}

#endif
/** @} */ // end of doxygen group 
