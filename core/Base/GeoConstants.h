/**
 * \file GeoConstants.h
 *
 * \ingroup Base
 * 
 * \brief defines geometry related constants
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_GEOCONSTANTS_H
#define LARLITE_GEOCONSTANTS_H
#include <string>

/// Namespace of everything in this framework
namespace larlite{
  
  namespace geo {

    enum DetId_t {
      kBo,         ///< Bo id
      kArgoNeuT,   ///< ArgoNeuT id
      kMicroBooNE, ///< MicroBoone id
      kLBNE10kt,   ///< LBNE 10kt id
      kJP250L,     ///< JPARC 250 L id
      kLBNE35t,    ///< 35t prototype id
      kLBNE34kt,   ///< LBNE 34kt id
      kCSU40L,     ///< CSU 40 L id
      kLArIAT,     ///< LArIAT id
      kICARUS,     ///< ICARUS T600 id
      kDetIdMax 
    };
    
    /// Enumerate the possible plane projections
    enum View_t {
      kU,       ///< planes which measure U
      kV,       ///< planes which measure V
      kW,       ///< soon to be deprecated, planes which measure W (third view for Bo, MicroBooNE, etc)
      kZ=kW,    ///< planes which measure Z direction (ie wires are vertical)
      k3D,      ///< 3 dimensional objects, potentially hits, clusters, prongs, etc
      kUnknown  ///< unknown view
    };
    
    /// Enumerate the possible signal types
    enum SigType_t {
      kInduction,   ///< signal from induction planes
      kCollection,  ///< signal from collection planes
      kMysteryType  ///< who knows?
    };
    
  }
}
#endif
/** @} */ // end of doxygen group
