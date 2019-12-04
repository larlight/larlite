/**
 * \file AlgoSibHasDiffParent.h
 *
 * \ingroup GeoTree
 * 
 * \brief Class def header for a class geotree::AlgoSibHasDiffParent
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

#ifndef ALGOSIBHASDIFFPARENT_H
#define ALGOSIBHASDIFFPARENT_H

//#include "AlgoMultipleParentsBase.h"
#include "AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"

namespace geotree{

  class AlgoSibHasDiffParent : public AlgoBase {

  public:
    
    AlgoSibHasDiffParent() { _name="SibHasDiffParent"; }

    /// Constructor which syncs node collection for the algorithm
    AlgoSibHasDiffParent(NodeCollection* coll);

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
