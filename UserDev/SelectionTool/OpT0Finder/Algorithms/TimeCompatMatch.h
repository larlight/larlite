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
    TimeCompatMatch(const std::string name="TimeCompatMatch");
    
    /// Default destructor
    ~TimeCompatMatch(){}

    void Configure(const ::fcllite::PSet &pset);

    bool MatchCompatible(const QCluster_t& clus, const Flash_t& flash);

  private:

    /// Buffer time to allow some uncertainty [us]
    double _time_buffer;

  };
}
#endif
/** @} */ // end of doxygen group 

