#ifndef ERTOOL_CONSTANTS_H
#define ERTOOL_CONSTANTS_H

#include <string>
#include <climits>
#include <limits>
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoAlgoConstants.h"
#include "ERToolTypes.h"
namespace ertool {

  /// Default parameter set storage file name
  const std::string kDataFileName = "larlite_spt_data.root";

  /// Utility: maximum value for double 
  const double kDOUBLE_MAX = std::numeric_limits<double>::max();

  /// Utility: minimum value for double
  const double kDOUBLE_MIN = std::numeric_limits<double>::min();

  /// Utility: maximum value for int
  const int    kINT_MAX    = std::numeric_limits<int>::max();

  /// Utility: maximum value for size_t
  const size_t kSIZE_MAX   = std::numeric_limits<size_t>::max();
  
  /// Default layer id for ertool::Particle
  const LayerID_t kDefaultLayer = -1;

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
  
  /// Default (invalid) vertex for ertool::Particle
  const ::geoalgo::Vector kINVALID_VERTEX(kINVALID_DOUBLE,kINVALID_DOUBLE,kINVALID_DOUBLE);

  /// Default (invalid) momentum for ertool::Particle
  const ::geoalgo::Vector kINVALID_MOMENTUM(kINVALID_DOUBLE,kINVALID_DOUBLE,kINVALID_DOUBLE);
} 

#endif
