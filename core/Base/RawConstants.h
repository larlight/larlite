#ifndef LARLITE_RAW_TYPES_H
#define LARLITE_RAW_TYPES_H

//#include <stdint.h> // uint32_t
#include <limits> // std::numeric_limits<>

namespace larlite{
  namespace raw{
    
    typedef enum _compress {
      kNone,       ///< no compression
      kHuffman,    ///< Huffman Encoding
      kZeroSuppression,  ///< Zero Suppression algorithm
      kZeroHuffman,  ///< Zero Suppression followed by Huffman Encoding
      kDynamicDec  ///< Dynamic decimation
    } Compress_t;
    
    typedef enum _auxdettype {
      kUnknownAuxDet, ///< no idea
      kScintillator,  ///< Scintillator paddle
      kTimeOfFlight,  ///< Time of flight
      kCherenkov      ///< Cherenkov counter
    } AuxDetType_t;
    
    /// Type representing a TDC tick
    typedef int TDCtick_t;
    
    /// Type representing the ID of a readout channel
    typedef unsigned int ChannelID_t;
    
    /// ID of an invalid channel
    const ChannelID_t InvalidChannelID = std::numeric_limits<ChannelID_t>::max();
    
    /// Returns whether the specified channel ID is valid
    /// @note This does not mean that channel exists in the current geometry.
    inline bool isValidChannelID(raw::ChannelID_t channel)
    { return channel != InvalidChannelID; }
    
  } // namespace raw
}
#endif
