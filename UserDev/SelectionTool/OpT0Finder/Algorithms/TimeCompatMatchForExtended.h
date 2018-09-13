/**
 * \file TimeCompatMatchForExtended.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class TimeCompatMatchForExtended
 *
 * @author david caratelli
 */

/** \addtogroup Algorithms

    @{*/
#ifndef OPT0FINDER_TIMECOMPATMATCH_FOREXTENDED_H
#define OPT0FINDER_TIMECOMPATMATCH_FOREXTENDED_H

#include "OpT0Finder/Base/BaseProhibitAlgo.h"
#include "OpT0Finder/Base/FlashProhibitFactory.h"

namespace flashana {
  
  /**
     \class TimeCompatMatchForExtended
     Simple flash matching algorithm. Based on absolute time of flash and track
     w.r.t. trigger time, if the two objects are incompatible (because the time-difference
     is larger than a full drift window) the match is not allowed.
  */
  class TimeCompatMatchForExtended : public BaseProhibitAlgo {
    
  public:
    
    /// Default constructor
    TimeCompatMatchForExtended(const std::string name="TimeCompatMatchForExtended");
    
    /// Default destructor
    ~TimeCompatMatchForExtended(){}

    bool MatchCompatible(const QCluster_t& clus, const Flash_t& flash);

  protected:

    void _Configure_(const Config_t &pset);

  private:

    /// Buffer time to allow some uncertainty [us]
    double _time_buffer;
    int _verbosity; //< kazu convention: 0=most verbose, 2=quiet
    
  };

  /**
     \class flashana::TimeCompatMatchForExtendedFactory
  */
  class TimeCompatMatchForExtendedFactory : public FlashProhibitFactoryBase {
  public:
    /// ctor
    TimeCompatMatchForExtendedFactory() { FlashProhibitFactory::get().add_factory("TimeCompatMatchForExtended",this); }
    /// dtor
    ~TimeCompatMatchForExtendedFactory() {}
    /// creation method
    BaseProhibitAlgo* create(const std::string instance_name) { return new TimeCompatMatchForExtended(instance_name); }
  };
  
}
#endif
/** @} */ // end of doxygen group 

