/**
 * \file AlgoBase.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoBase
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    
    @{*/
#ifndef ALGOBASE_H
#define ALGOBASE_H

#include "Correlation.h"
#include "NodeCollection.h"
#include "Node.h"
#include <vector>
#include "string.h"

namespace geotree{

  class AlgoBase{

  public:
    
    AlgoBase() { _name=""; _verbose = false; }

    /// Constructor which syncs node collection for the algorithm
    AlgoBase(NodeCollection* coll) { _name=""; _coll = coll; _verbose = false; }

    virtual ~AlgoBase(){}

    /// Getter for the correlations erased/modified/added by algo
    virtual std::map< std::pair<NodeID_t, NodeID_t>, geotree::Correlation> GetCorrelations() { return _corr_v; }

    /// clear correlations. To be called every time aglorithm is applied to a node
    virtual void ClearCorrelations() { _corr_v.clear(); }

    /// set verbosity
    void SetVerbose(bool on) { _verbose = on; }

  protected:

    // verbosity flag
    bool _verbose;
    
    // Algo's name
    std::string _name;

    // map for correlations
    // connects a pair of nodes to a correlation type
    std::map< std::pair<NodeID_t, NodeID_t>, geotree::Correlation> _corr_v;

    // pointer to collection reference
    NodeCollection* _coll;

  };
}

#endif
/** @} */ // end of doxygen group 
