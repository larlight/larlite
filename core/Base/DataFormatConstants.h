/**
 * \file DataFormatConstants.h
 *
 * \ingroup DataFormat
 * 
 * \brief defines constants used in DataFormat
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_DATAFORMATCONSTANTS_H
#define LARLITE_DATAFORMATCONSTANTS_H

#include <string>
#include <limits>
#include <climits>
#include <TROOT.h>

/// Namespace of everything in this framework
namespace larlite{
  
  /// Defines constants for data structure definition (system utility)
  namespace data{

    const unsigned char  kINVALID_UCHAR  = std::numeric_limits<unsigned char>::max();
    const char           kINVALID_CHAR   = std::numeric_limits<char>::max();
    const unsigned short kINVALID_USHORT = std::numeric_limits<unsigned short>::max();
    const short          kINVALID_SHORT  = std::numeric_limits<short>::max();
    const unsigned int   kINVALID_UINT   = std::numeric_limits<unsigned int>::max();
    const int            kINVALID_INT    = std::numeric_limits<int>::max();

    const double kINVALID_DOUBLE = std::numeric_limits<double>::max();
    const float  kINVALID_FLOAT  = std::numeric_limits<float>::max();
    
    /// Define data product type identifiers
    /// Defines a type of data product class
    enum DataType_t {
      kUndefined=0,
      kEvent,
      kGTruth,             ///< simb::GTruth
      kMCTruth,            ///< simb::MCTruth 
      kMCParticle,         ///< simb::MCParticle
      kMCFlux,             ///< simb::MCFlux
      kMCTrajectory,       ///< simb::MCTrajectory
      kMCNeutrino,         ///< simb::MCNeutrino
      kSimChannel,         ///< sim::SimChannel
      kMCShower,           ///< sim::MCShower
      kRawDigit,           ///< raw::RawDigit
      kWire,               ///< recob::Wire
      kHit,                ///< recob::Hit
      kCosmicTag,          ///< anab::CosmicTag 
      kOpHit,              ///< opdet::OpHit
      kOpFlash,            ///< opdet::OpFlash
      kCluster,            ///< recob::Cluster
      kSeed,               ///< recob::Seed
      kSpacePoint,         ///< recob::Spacepoints
      kTrack,              ///< recob::Track
      kShower,             ///< recob::Shower
      kVertex,             ///< recob::Vertex
      kEndPoint2D,         ///< recob::EndPoint2D
      kCalorimetry,        ///< anab::Calorimetry
      kParticleID,         ///< anab::ParticleID
      kPFParticle,         ///< recob::PFParticle
      kUserInfo,           ///< dynamic data container (LArLite original)
      kTrigger,            ///< Trigger data (LArLite original)
      kPOTSummary,         ///< sumdata::POTSummary 
      kMCTrack,            ///< sim::MCTrack
      kMCTree,             ///< sim::MCTree
      kMinos,              ///< t962::Minos
      kFIFO,               ///< from larlight::fifo
      kDATA_TYPE_MAX
    };

    const std::string kDATA_TREE_NAME[kDATA_TYPE_MAX] = {
      "unknown",
      "event",
      "gtruth",
      "mctruth",
      "mcpart",
      "mcflux",
      "mctrajectory",
      "mcnu",
      "simch",
      "mcshower",
      "rawdigit",
      "wire",
      "hit",
      "cosmictag",
      "ophit",
      "opflash",
      "cluster",
      "seed",
      "sps",
      "track",
      "shower",
      "vertex",
      "end2d",
      "calo",
      "partid",
      "pfpart",
      "user",
      "trigger",
      "potsummary",
      "mctrack",
      "mctree",
      "minos",
      "fifo"
    };
    
  }
  
}
#endif
/** @} */ // end of doxygen group
