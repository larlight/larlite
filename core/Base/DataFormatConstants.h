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
    const size_t         kINVALID_SIZE   = std::numeric_limits<size_t>::max();

    const double kINVALID_DOUBLE = std::numeric_limits<double>::max();
    const float  kINVALID_FLOAT  = std::numeric_limits<float>::max();

    /// Category of data types
    enum DataTypeCategory_t {
      kEventData,
      kRunData,
      kSubRunData,
      kDATA_TYPE_CATEGORY_MAX
    };
    
    /**
       Define event-wise data product type identifiers.\n
       What appears here must have a corresponding entry in larlite::data::kDATA_TREE_NAME.
    */
    enum DataType_t {
      // Event-data types
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
      kDAQHeaderTimeUBooNE,///< raw::DAQHeaderTimeUBooNE
      kWire,               ///< recob::Wire
      kHit,                ///< recob::Hit
      kCRTHit,             ///< crt::CRTHit
      kCRTTrack,           ///< crt::CRTTrack
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
      kMCTrack,            ///< sim::MCTrack
      kMCTree,             ///< sim::MCTree
      kMinos,              ///< t962::Minos
      kFIFO,               ///< from larlight::fifo
      kAssociation,        ///< Association data product
      kPCAxis,             ///< recob::PCAxis
      kFlashMatch,         ///< anab::FlashMatch
      kTrackMomentum,      ///< track momentum
      kOpDetWaveform,      ///< optical detector waveform
      kSimPhotons,         ///< sim::SimPhotons
      kMuCSData,           ///< MuCS::MuCSData
      kMuCSReco,           ///< MuCS::MuCSReco
      kROI,
      kAuxDetSimChannel,   ///< sim::AuxDetSimChannel
      kChStatus,           ///< larlite::chstatus
      kT0,                 ///< larlite::t0
      kMCEventWeight,      ///< evwgh::MCEventWeight
      kSWTrigger,          ///< larlite::swtrigger
      kLArFlow3DHit,       ///< larflow::LArFlow3DHit   (larflow addition)
      kLArFlowCluster,     ///< larflow::LArFlowCluster (larflow addition)
      kPixelMask,          ///< larlite::pixelmask (larflow addition)
      kDATA_TYPE_MAX       ///< Event-wise enum boundary
    };

    enum RunDataType_t {
      // Run-data types
      kRUNDATA_Undefined,
      kRUNDATA_TYPE_MAX   ///< Run-data type enum boundary
    };

    enum SubRunDataType_t {
      // SubRun-data types
      kSUBRUNDATA_Undefined,
      kPOTSummary,         ///< sumdata::POTSummary 
      kSUBRUNDATA_TYPE_MAX ///< SubRun-data type enum boundary
    };

    #ifndef __CINT__
    #ifndef __CLING__
    const std::string kDATA_TREE_NAME[kDATA_TYPE_MAX] = {
      //const std::vector<std::string> kDATA_TREE_NAME = {
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
      "daqheadertimeuboone",
      "wire",
      "hit",
      "crthit",
      "crttrack",
      "cosmictag",
      "ophit",
      "opflash",
      "cluster",
      "seed",
      "sps",
      "track",
      "shower",
      "vertex",
      "endpoint2d",
      "calo",
      "partid",
      "pfpart",
      "user",
      "trigger",
      "mctrack",
      "mctree",
      "minos",
      "fifo",
      "ass",
      "pcaxis",
      "fmatch",
      "trackmom",
      "opdigit",
      "simphotons",
      "mucsdata",
      "mucsreco",
      "ROI",
      "auxsimch",
      "chstatus",
      "t0",
      "mceventweight",
      "swtrigger",
      "larflow3dhit",
      "larflowcluster",
      "pixelmask"
    };

    const std::string kRUNDATA_TREE_NAME[kRUNDATA_TYPE_MAX] = {
      //    const std::vector<std::string> kRUNDATA_TREE_NAME = {
      "undefined"
    };
    
    const std::string kSUBRUNDATA_TREE_NAME[kSUBRUNDATA_TYPE_MAX] = {
      //    const std::vector<std::string> kSUBRUNDATA_TREE_NAME = {
      "undefined",
      "potsummary"
    };
    #endif
    #endif
    static const std::string kEVENT_ID_TREE("larlite_id_tree");
  }
  
}
#endif
/** @} */ // end of doxygen group
