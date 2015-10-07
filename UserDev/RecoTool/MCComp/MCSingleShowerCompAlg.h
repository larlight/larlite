/**
 * \file MCSingleShowerCompAlg.h
 *
 * \ingroup MCComp
 *
 * \brief This is a class designed to only work on single particle files
 * (single MCShower, specifically... so, single e's and single gammas)
 * It computes similar things as MCMatchAlg and MCBTAlg, but WITHOUT the use of simchannel
 * (simchannels can be avoided because we only care about single-particle files)
 *
 * @author davidkaleko
 */

/** \addtogroup MCComp

    @{*/

#ifndef LARLITE_MCSINGLESHOWERCOMPALG_H
#define LARLITE_MCSINGLESHOWERCOMPALG_H

#include "DataFormat/storage_manager.h"
#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {
/**
   \class MCSingleShowerCompAlg
   User defined class made by kalekizzle
 */
class MCSingleShowerCompAlg {

public:

  /// Default constructor
  MCSingleShowerCompAlg();

  /// Default destructor
  virtual ~MCSingleShowerCompAlg() {}

  /// Function that must be called once per event to intialize this algorithm
  bool InitializeForEvent(storage_manager* storage, std::string fShowerProducer);

  /// Function to compute a shower's cluster's efficiency and purity 
  /// Return is std::vector<std::pair>. Outer vector has one entry per plane 
  /// (regardless if that plane contains a cluster associated w/ the shower)
  /// and the std::pair is (efficiency, purity) for the associated cluster in that plane.
  /// Note, purity is always == 1 for single-MCShower events! Just returned as a reminder...
  std::vector< std::pair<double, double> > ClusterEP(size_t shower_index);

protected:

  /// Outer map has key == shower index (size == number of reconstructed showers)
  /// Outer map has value == Inner map
  /// Inner map has key == plane (size == number of planes that have a cluster associated with that shower)
  /// Inner map has value == summed charge of the associated cluster on that plane
  std::map< int, std::map< int, double> > _shower_plane_hitcharges;

  /// Simple function to clear the _shower_plane_hitcharges variable and all its contents
  void ResetShowerClusterHitCharges();

  /// Map of plane number (cast as integer) ==> total summed charge of all hits on that plane
  std::map< int, double > _total_hit_Qs_per_plane;

};
}
#endif

/** @} */ // end of doxygen group
