/**
 * \file TimeCompatMatch.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class TimeCompatMatch
 *
 * @author david caratelli
 */

/** \addtogroup Algorithms

    @{*/
#ifndef OPT0FINDER_TIMECOMPATMATCH_H
#define OPT0FINDER_TIMECOMPATMATCH_H

#include "OpT0Finder/Base/BaseProhibitAlgo.h"

namespace flashana {
  
  /**
     \class TimeCompatMatch
     Simple flash matching algorithm. Based on absolute time of flash and track
     w.r.t. trigger time, if the two objects are incompatible (because the time-difference
     is larger than a full drift window) the match is not allowed
  */
  class TimeCompatMatch : public BaseProhibitAlgo {
    
  public:
    
    /// Default constructor
    TimeCompatMatch();
    
    /// Default destructor
    ~TimeCompatMatch(){}

    bool MatchCompatible(const QCluster_t& clus, const Flash_t& flash);

    /// set the time it takes to drift across the TPC
    void SetFrameDriftTime(double t) { _frame_drift_time = t; }

  private:

    /// time it takes for charge to drift the entire drift-length
    double _frame_drift_time;

  };
}
#endif
/** @} */ // end of doxygen group 

