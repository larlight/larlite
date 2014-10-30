/**
 * \file CPAlgoNHits.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CPAlgoNHits
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CPALGONHITS_H
#define RECOTOOL_CPALGONHITS_H

#include "CMTool/CMToolBase/CPriorityAlgoBase.h"

namespace cmtool {
  /**
     \class CPAlgoNHits
     Simple algorithm to determine priority based on # of hits.
     If # hits < set cut value by a user, returns -1.
  */
  class CPAlgoNHits : public CPriorityAlgoBase {
    
  public:
    
    /// Default constructor
    CPAlgoNHits();
    
    /// Default destructor
    virtual ~CPAlgoNHits(){};

    /**
       Core function: given the CPAN input, return a float which indicates 
       the user-defined priority for analysis.
    */
    virtual float Priority(const ::cluster::ClusterParamsAlg &cluster);

    /// Setter for minimum # hits
    void SetMinHits(size_t n) { _min_hits = n; }

  protected:

    size_t _min_hits;

  };
}
#endif
/** @} */ // end of doxygen group 

