#ifndef ERTOOL_TYPES_H
#define ERTOOL_TYPES_H

#include <vector>
#include <cstddef>
namespace ertool {

  typedef size_t RecoID_t;  ///< A unique reconstructed object (input) identifier variable type
  typedef size_t NodeID_t;  ///< A unique particle identifier variable type
  typedef int    LayerID_t; ///< A layer number in particle graph (0 = Primary, kDefaultLayer = not yet assessed )

  /// Defines a type for a combinatory index expression
  typedef std::vector<NodeID_t> Combination_t;

  /// Defines a set of combinations
  typedef std::vector<Combination_t> CombinationSet_t;

  /// Reconstructed object type associated with each particle
  enum RecoType_t {
    kInvisible, ///< No reconstructed information
    kShower,    ///< Associated with a shower object
    kTrack,     ///< Associated with a track object
    kVertex,    ///< Associated with a vertex object
    kINVALID_RECO_TYPE
  };

  /// Relation Type
  enum RelationType_t {
    kChild,
    kParent,
    kSibling,
    kINVALID_RELATION_TYPE
  };
  
} 

#endif
