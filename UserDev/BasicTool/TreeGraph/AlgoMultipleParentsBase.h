/**
 * \file AlgoMultipleParentsBase.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoMultipleParentsBase
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    
    @{*/
#ifndef ALGOMULTIPLEPARENTSBASE_H
#define ALGOMULTIPLEPARENTSBASE_H

#include "AlgoBase.h"

namespace geotree{

  class AlgoMultipleParentsBase : public AlgoBase {

  public:
    
    AlgoMultipleParentsBase() { _name="MultipleParents"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoMultipleParentsBase(NodeCollection* coll) { _name="MultipleParents"; _coll = coll; _verbose = false; }

    virtual void FindBestParent(const NodeID_t& id, const std::vector<NodeID_t>& parents) {}

  };
}

#endif
/** @} */ // end of doxygen group 
