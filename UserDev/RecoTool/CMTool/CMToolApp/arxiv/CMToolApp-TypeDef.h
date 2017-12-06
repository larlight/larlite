//
// type-def file for ROOT
//

#ifndef RECOTOOL_CMTOOLAPP_TYPEDEF_H
#define RECOTOOL_CMTOOLAPP_TYPEDEF_H

// Header files included here

#include "MatchViewer.h"
#include "ViewerException.h"
#include "ClusterViewerAlgo.h"
#include "ClusterViewer.h"
#include "ClusterMerger.h"
#include "FuzzyClusterShower.h"
#include "ClusterMatcher.h"
#include "MergeViewer.h"
#include "CMergeHelper.h"
//ADD_NEW_HEADER ... do not change this comment line

// Class forward declaration here
namespace cluster {
  class ViewerException;
  class ClusterViewerAlgo;
}

namespace larlite {
  class ClusterMerger;
  class ClusterMergerArray;
  class FuzzyClusterShower;
  class ClusterMatcher;
  class ClusterViewer;
  class MergeViewer;
  class MatchViewer;
//ADD_ANA_CLASS ... do not change this comment line
}

namespace cmtool {
  class CMergeHelper;
}
//ADD_EMPTY_CLASS ... do not change this comment line
#endif
