/**
 * \file CPAlgoQSum.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CPAlgoQSum
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CPALGOQSUM_H
#define RECOTOOL_CPALGOQSUM_H

#include "CMTool/CMToolBase/CPriorityAlgoBase.h"

namespace cmtool {
  /**
     \class CPAlgoQSum
     Simple algorithm to determine priority based on charge sum
     If charge sum < set cut value by a user, returns -1.     
  */
  class CPAlgoQSum : public CPriorityAlgoBase {
    
  public:
    
    /// Default constructor
    CPAlgoQSum();
    
    /// Default destructor
    virtual ~CPAlgoQSum(){};

    /**
       Core function: given the CPAN input, return a float which indicates 
       the user-defined priority for analysis.
    */
    virtual float Priority(const ::cluster::cluster_params &cluster);

    /// Setter for minimum charge
    void SetMinQ(double v) { _qsum_cut = v; }

  protected:

    double _qsum_cut;

  };
}
#endif
/** @} */ // end of doxygen group 

