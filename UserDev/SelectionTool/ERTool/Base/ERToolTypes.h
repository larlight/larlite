#ifndef ERTOOL_TYPES_H
#define ERTOOL_TYPES_H

#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include <string>
namespace ertool {

  typedef size_t RecoID_t;     ///< A unique reconstructed object (input) identifier variable type
  typedef size_t FlashID_t;    ///< A unique flash object (input) identifier variable type
  typedef size_t NodeID_t;     ///< A unique particle identifier variable type
  typedef int    Generation_t; ///< A layer number in particle graph (0 = Primary, kDefaultLayer = not yet assessed )

  /// Defines an input shower/track original product information
  typedef std::pair<size_t,std::string> RecoInputID_t;

  /// Defines a process information container
  typedef std::map<std::string,std::string> ProcInfo_t;

  /// Defines a type for a combinatory index expression
  typedef std::vector<NodeID_t> Combination_t;

  /// Defines a set of combinations
  typedef std::vector<Combination_t> CombinationSet_t;

  /// Reconstructed object type associated with each particle
  enum RecoType_t {
    kInvisible, ///< No reconstructed information
    kShower,    ///< Associated with a shower object
    kTrack,     ///< Associated with a track object
    kINVALID_RECO_TYPE
  };

  /// Relation Type
  enum RelationType_t {
    kChild,
    kParent,
    kSibling,
    kUnrelated,
    kINVALID_RELATION_TYPE
  };

  /// "Process Type" that created this particle
  enum ProcessType_t {
    kUnknown,
    /// Particles coming out of the neutrino interaction
    kNuCCQE,         //1001
    kNuNC,           //1002
    kNuCC1Pi,        //1003,5,10,12
    kNuCC1PiZero,    //1004,11
    kNuNC1Pi,        //1007,9,14,16
    kNuNC1PiZero,    //1008,15
    kNuCCDeltaRes,   //1017-21,28-32
    kNuNCDeltaRes,   //1022-27,33-38
    kNuDIS,          //1091-1092
    kNuOther,        //every other types of neutrino interactions
    /// Other particles
    kMichel,         //michel electrons "muMinusCaptureAtRest"
    kMuIoni,         //particles (electrons?) from muon ionization "muIoni"
    kCosmic,         //Cosmics
    kPiZeroDecay,    //daughters of pizero decays (gammas usually)
    kINVALID_PROCESS_TYPE
  };

  namespace io {
    enum Mode_t {
      kUNDEFINED, ///< undefined mode
      kREAD,  ///< read mode
      kWRITE, ///< write mode
      kBOTH   ///< both read & write mode
    };

    enum StreamType_t {
      kEmptyStream,
      kROOT,
      kROOTTuple
    };
  }

  namespace msg {
    enum Level_t {
      kDEBUG=0,      ///< Message level ... useful to debug a crash
      kINFO,         ///< Debug info but not the lowest level
      kNORMAL,       ///< Normal stdout
      kWARNING,      ///< notify a user in the standard operation mode for an important finding.
      kERROR,        ///< notify a user when something is clearly wrong
      kEXCEPTION,    ///< Exception!
      kMSG_TYPE_MAX
    };
  }
} 

#endif
