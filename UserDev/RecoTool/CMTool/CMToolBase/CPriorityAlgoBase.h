/**
 * \file CPriorityAlgoBase.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CPriorityAlgoBase
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CPRIORITYALGOBASE_H
#define RECOTOOL_CPRIORITYALGOBASE_H

#include "CMAlgoBase.h"

namespace cmtool {

  /**
     \class CPriorityAlgoBase
     An abstract base class for CMatchManager and CMergeManager to determine 
     cluster "priority" for matching and merging action respectively.
  */
  class CPriorityAlgoBase : public CMAlgoBase {
    
  public:
    
    /// Default constructor
    CPriorityAlgoBase(){}
    
    /// Default destructor
    virtual ~CPriorityAlgoBase(){}

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual float Priority(const ::cluster::Cluster &cluster) = 0;

  };

}

#endif
/** @} */ // end of doxygen group 

