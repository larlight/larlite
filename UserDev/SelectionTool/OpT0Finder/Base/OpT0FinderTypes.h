#ifndef OPT0FINDER_OPT0FINDERTYPES_H
#define OPT0FINDER_OPT0FINDERTYPES_H

#include <vector>
#include "OpT0FinderConstants.h"
#include <string>
namespace flashana {

  /// Index used to identify Flash_t/QPointCollection_t uniquely in an event
  typedef size_t ID_t;
  /// Invalid ID
  const ID_t kINVALID_ID = kINVALID_SIZE;

  /// Enumerator for different types of algorithm
  enum Algorithm_t {
    kTPCFilter,       ///< Algorithm type to filter out TPC objects from matching candidate list
    kFlashFilter,     ///< Algorithm type to filter out flash from matching candidate list
    kFlashMatch,      ///< Algorithm type to match flash hypothesis and reconstructed flash
    kMatchProhibit,   ///< Algorithm type to prohibit a match between a flash and a cluster
    kFlashHypothesis, ///< Algorithm type to make QCluster_t => Flash_t hypothesis
    kAlgorithmTypeMax ///< enum flag for algorithm type count & invalid type
  };

  /// Struct to represent an optical flash
  struct Flash_t {
  public:

    std::vector<double> pe_v; ///< PE distribution over photo-detectors
    double x,y,z;             ///< Flash position 
    double x_err,y_err,z_err; ///< Flash timing, a candidate T0
    double time;
    ID_t idx;                 ///< index from original larlite vector
    /// Default ctor assigns invalid values
    Flash_t() {
      x = y = z = kINVALID_DOUBLE;
      x_err = y_err = z_err = kINVALID_DOUBLE;
      time = kINVALID_DOUBLE;
      idx = kINVALID_ID;
    }
  };

  /// Struct to represent an energy deposition point in 3D space
  struct QPoint_t{

    double x,y,z; ///< Spatial position in [cm]
    double q;     ///< Charge in an arbitrary unit
    /// Default ctor assigns invalid values
    QPoint_t()
      : x(kINVALID_DOUBLE)
      , y(kINVALID_DOUBLE)
      , z(kINVALID_DOUBLE)
      , q(kINVALID_DOUBLE)
    {}
    /// Alternative ctor
    QPoint_t(double xvalue,
	     double yvalue,
	     double zvalue,
	     double qvalue)
      : x(xvalue)
      , y(yvalue)
      , z(zvalue)
      , q(qvalue)
    {}
  };

  /// Collection of charge deposition 3D point (cluster)
  struct QCluster_t : public std::vector<QPoint_t>{
    ID_t idx;                 ///< index from original larlite vector
    /// Default constructor
    QCluster_t()
      : idx(kINVALID_ID)
    {}

  };
  /// Collection of 3D point clusters (one use case is TPC object representation for track(s) and shower(s))
  typedef std::vector<flashana::QCluster_t> QClusterArray_t;
  /// Collection of Flash objects
  typedef std::vector<flashana::Flash_t> FlashArray_t;

  /// Index collection
  typedef std::vector<flashana::ID_t> IDArray_t;



  /// Flash-TPC match info
  struct FlashMatch_t {
    ID_t tpc_id;   ///< matched TPC object ID
    ID_t flash_id; ///< matched Flash ID
    double score;  ///< floating point representing the "goodness" (algorithm dependent) 
    QPoint_t tpc_point; ///< estimated & matched 3D flash hypothesis point from TPC information
    /// Default ctor assigns invalid values
    FlashMatch_t()
    { tpc_id = kINVALID_ID; flash_id = kINVALID_ID; score = -1; }
    /// Alternative ctor
    FlashMatch_t(const ID_t& tpc_id_value,
		 const ID_t& flash_id_value,
		 const double& score_value)
    { tpc_id = tpc_id_value; flash_id = flash_id_value; score = score_value; }
  };
  
  namespace msg {
    /// Verbosity message level
    enum MSGLevel_t {
      kDEBUG,
      kINFO,
      kNORMAL,
      kWARNING,
      kERROR,
      kEXCEPTION,
      kMSG_TYPE_MAX
    };
    
    const std::string kColorPrefix[kMSG_TYPE_MAX] =
      {
	"\033[94m", ///< blue ... DEBUG
	"\033[92m", ///< green ... INFO
	"\033[95m", ///< magenta ... NORMAL
	"\033[93m", ///< yellow ... WARNING
	"\033[91m", ///< red ... ERROR
	"\033[5;1;33;41m" ///< red with yellow background ... CRITICAL
      };
    ///< Color coding of message
    
    const std::string kStringPrefix[kMSG_TYPE_MAX] =
      {
	"     [DEBUG]  ", ///< DEBUG message prefix
	"      [INFO]  ", ///< INFO message prefix
	"    [NORMAL]  ", ///< NORMAL message prefix
	"   [WARNING]  ", ///< WARNING message prefix
	"     [ERROR]  ", ///< ERROR message prefix
	" [EXCEPTION]  "  ///< CRITICAL message prefix
      };
    ///< Prefix of message
  }
}
#endif
