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
#include "OpT0Finder/Base/FlashProhibitFactory.h"

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

    bool MatchCompatible(const QCluster_t& clus, const Flash_t& flash);

  protected:

    void _Configure_(const Config_t &pset);

  private:

    /// Buffer time to allow some uncertainty [us]
    double _time_buffer;

  };

  /**
     \class flashana::TimeCompatMatchFactory
  */
  class TimeCompatMatchFactory : public FlashProhibitFactoryBase {
  public:
    /// ctor
    TimeCompatMatchFactory() { FlashProhibitFactory::get().add_factory("TimeCompatMatch",this); }
    /// dtor
    ~TimeCompatMatchFactory() {}
    /// creation method
    BaseProhibitAlgo* create(const std::string instance_name) { return new TimeCompatMatch(instance_name); }
  };
  
}
#endif
/** @} */ // end of doxygen group 

