/**
 * \file AlgoGenericConflictComplex.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoGenericConflictComplex
 *
 * @author david caratelli
 */

/** \addtogroup GeoTree
    This algorithm is called when a "generic conflict" is found
    A "generic conflict" means: the node has both a parent and
    a sibling.
    Actions taken:
    either accept both parent and sibling (editing vertex)
    do this in case loose == true
    or pick the one with the highest score (loose == false)
    either way, modify all other correlations accordingly
    @{*/

#ifndef ALGOGENERICCONFLICTCOMPLEX_H
#define ALGOGENERICCONFLICTCOMPLEX_H

//#include "AlgoMultipleParentsBase.h"
#include "AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"

namespace geotree{

  class AlgoGenericConflictComplex : public AlgoBase {

  public:
    
    AlgoGenericConflictComplex() { _name="GenericConflictComplex"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoGenericConflictComplex(NodeCollection* coll);

    void ResolveConflict(const NodeID_t& id, const NodeID_t& parent, const NodeID_t& sibling);

    /// set the loose boolean
    void setLoose(bool on) { _loose = on; }

  private:

    /// loose flag: decide if to break or merge correlations
    bool _loose;

    larlite::geoalgo::GeoAlgo _geoAlgo;

  };
}

#endif
/** @} */ // end of doxygen group 
