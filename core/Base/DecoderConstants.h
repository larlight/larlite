/**
 * \file DecoderConstants.hh
 *
 * \ingroup Base
 * 
 * \brief defines Decoder related constants
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef DECODERCONSTANTS_HH
#define DECODERCONSTANTS_HH
#include <string>


/// Namespace of everything in this framework
namespace larlite{

  /// Defines file/data-word format type
  namespace FORMAT{
    
    /// Defines input file format for decoder (i.e. output of electronics)
    enum INPUT_FILE{
      /// Bin format for decoder (input) and coder (output)   
      BINARY=0,
      /// ASCII format for (de)coder
      ASCII,
      /// Undefined...
      UNDEFINED
    };
  }
}
#endif
/** @} */ // end of doxygen group
