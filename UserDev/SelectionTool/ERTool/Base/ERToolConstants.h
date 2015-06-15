#ifndef ERTOOL_CONSTANTS_H
#define ERTOOL_CONSTANTS_H

#include <utility>
#include <string>
#include <climits>
#include <limits>
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoAlgoConstants.h"
#include "ERToolTypes.h"
namespace ertool {

  /// Default parameter set storage file name
  const std::string kDefaultConfigFileName = std::string(getenv("LARLITE_USERDEVDIR")) + std::string("/SelectionTool/ERTool/dat/ertool_default.cfg");

  /// Utility: maximum value for double 
  const double kDOUBLE_MAX = std::numeric_limits<double>::max();

  /// Utility: minimum value for double
  const double kDOUBLE_MIN = std::numeric_limits<double>::min();

  /// Utility: maximum value for int
  const int    kINT_MAX    = std::numeric_limits<int>::max();

  /// Utility: maximum value for size_t
  const size_t kSIZE_MAX   = std::numeric_limits<size_t>::max();
  
  /// Default generation for ertool::Particle
  const Generation_t kDefaultGeneration = -1;

  /// Default (invalid) node id
  const NodeID_t  kINVALID_NODE_ID = kSIZE_MAX;
  
  /// Default (invalid) reco object id
  const RecoID_t  kINVALID_RECO_ID = kSIZE_MAX;

  /// Default (invalid) value for an integer
  const int       kINVALID_INT = kINT_MAX;

  /// Default (invalid) value for a size_t
  const size_t    kINVALID_SIZE = kSIZE_MAX;

  /// Default (invalid) valud for double
  const double    kINVALID_DOUBLE = kDOUBLE_MAX;

  /// Default particle reco/relationship score
  const float     kDEFAULT_SCORE = -1;

  /// Default (invalid) reco input ID
  const RecoInputID_t kINVALID_RECO_INPUT_ID(kINVALID_SIZE,"");
  
  /// Default (invalid) vertex for ertool::Particle
  const ::geoalgo::Vector kINVALID_VERTEX(kINVALID_DOUBLE,kINVALID_DOUBLE,kINVALID_DOUBLE);

  /// Default (invalid) momentum for ertool::Particle
  const ::geoalgo::Vector kINVALID_MOMENTUM(kINVALID_DOUBLE,kINVALID_DOUBLE,kINVALID_DOUBLE);

  namespace io{
    /// EventData identifier
    const std::string kEVENT_DATA_NAME("edata");
    /// ParticleGraph identifier
    const std::string kGRAPH_DATA_NAME("pgraph");

    /// MC EventData identifier
    const std::string kMC_EVENT_DATA_NAME("mc_edata");
    /// MC ParticleGraph identifier
    const std::string kMC_GRAPH_DATA_NAME("mc_pgraph");
  }

  namespace msg{
    const std::string ColorPrefix[kMSG_TYPE_MAX] =
      {
	"\033[94m", ///< blue ... DEBUG
	"\033[92m", ///< green ... INFO
	"\033[95m", ///< magenta ... NORMAL
	"\033[93m", ///< yellow ... WARNING
	"\033[91m"  ///< red ... ERROR
      };
    ///< Color coding of message
    
    const std::string StringPrefix[kMSG_TYPE_MAX] =
      {
	"     [DEBUG]  ", ///< DEBUG message prefix
	"      [INFO]  ", ///< INFO message prefix
	"    [NORMAL]  ", ///< NORMAL message prefix
	"   [WARNING]  ", ///< WARNING message prefix
	"     [ERROR]  "  ///< ERROR message prefix
      };
    ///< Prefix of message
  }
} 

#endif
