#ifndef LARLITE_RAWCONSTANTS_H
#define LARLITE_RAWCONSTANTS_H

namespace larlite {
  namespace raw {
    
    typedef enum _compress {
      kNone,       ///< no compression
      kHuffman,    ///< Huffman Encoding
      kZeroSuppression,  ///< Zero Suppression algorithm
      kZeroHuffman,  ///< Zero Suppression followed by Huffman Encoding
      kDynamicDec  ///< Dynamic decimation
    } Compress_t;
  }
}

#endif

