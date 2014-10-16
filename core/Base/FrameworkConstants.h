/**
 * \file FrameworkConstants.h
 *
 * \ingroup Base
 * 
 * \brief defines basic constants used in this framework
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_FRAMEWORKCONSTANTS_H
#define LARLITE_FRAMEWORKCONSTANTS_H
#include <string>

/// Namespace of everything in this framework
namespace larlite{
  
  /// Defines constants for Message utility
  namespace msg{
    
    /// Defines message level
    enum Level{
      kDEBUG=0,      ///< Message level ... useful to debug a crash
      kINFO,         ///< Debug info but not the lowest level
      kNORMAL,       ///< Normal stdout
      kWARNING,      ///< notify a user in the standard operation mode for an important finding.
      kERROR,        ///< notify a user when something is clearly wrong
      kMSG_TYPE_MAX
    };
    
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
/** @} */ // end of doxygen group
