// TriggerData/TriggerData.h
#ifndef LARLITE_TRIGGERDATA_H
#define LARLITE_TRIGGERDATA_H

// C++ includes
#include <vector>
#include <limits>
#include <stdexcept>
#include <iostream>
#include "data_base.h"
namespace larlite {
  
  class trigger : public event_base {

  public:

    /// Default ctor
    trigger(std::string name="noname") : event_base(data::kTrigger,name)
    {
      fTriggerNumber       = data::kINVALID_UINT;
      
      fTriggerTime         = data::kINVALID_DOUBLE;
      fBeamGateTime        = data::kINVALID_DOUBLE;
      
      fTriggerBits         = 0x0;
    }
    
  private:
    
    unsigned int fTriggerNumber;       ///< Trigger counter
    double       fTriggerTime;         ///< Trigger time w.r.t. electronics clock T0
    double       fBeamGateTime;        ///< BeamGate time w.r.t. electronics clock T0
    unsigned int fTriggerBits;         ///< Trigger bits ... dedicated bit-by-bit function available

  public:

    /// Alternative constructor    
    trigger(unsigned int counter,
	    double       trigger_time,
	    double       beamgate_time,
	    uint32_t     bits)
      : fTriggerNumber       ( counter           ),
	fTriggerTime         ( trigger_time      ),
	fBeamGateTime        ( beamgate_time     ),
	fTriggerBits         ( bits              )
    {}

    /// Trigger number
    unsigned int TriggerNumber()          const { return fTriggerNumber;       }
    /// Trigger time w.r.t. electronics clock T0 in us
    double       TriggerTime  ()          const { return fTriggerTime;         }
    /// BeamGate time w.r.t. electronics clock T0 in us
    double       BeamGateTime ()          const { return fBeamGateTime;        }
    /// Trigger Bits
    unsigned int TriggerBits  ()          const { return fTriggerBits;         }
    /// Accessor to specific bit
    bool         Triggered(const unsigned char bit) const;

  };
}

#endif 
