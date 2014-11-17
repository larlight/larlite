/**
 * \file TimeService.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class TimeService
 *
 * @author kterao
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARLITE_TIMESERVICE_H
#define LARLITE_TIMESERVICE_H

#include <iostream>
#include "ElecClock.h"
#include "LArUtilBase.h"
namespace larutil {
  /**
     \class TimeService
     This class provides electronics various electronics clocks. Currently supports
     three types of clocks: TPC, Optical, and Trigger in order to support MicroBooNE experiments.
  */
  class TimeService : public LArUtilBase{
    
  protected:
    
    /// Default constructor
    TimeService(bool default_load=false);
    
    /// Default destructor
    ~TimeService(){};

    /// Electronics clock counting start time in G4 time frame [us]
    double fG4RefTime;

    /// Frame period
    double fFramePeriod;

    /// TPC clock
    ::larutil::ElecClock fTPCClock;

    /// Optical clock
    ::larutil::ElecClock fOpticalClock;

    /// Trigger clock
    ::larutil::ElecClock fTriggerClock;

    /// Time offset from trigger to TPC readout start
    double fTriggerOffsetTPC;

    /// Trigger time in [us]
    double fTriggerTime;

    /// BeamGate time in [us]
    double fBeamGateTime;

    /// self pointer as a singleton
    static TimeService* _me;

  protected:

    virtual bool ReadTree();

    virtual void ClearData();

    /// Setter for trigger times
    virtual void SetTriggerTime(double trig_time, double beam_time)
    { 
      fTriggerTime  = trig_time;
      fBeamGateTime = beam_time;
      fTPCClock.SetTime(trig_time);
      fOpticalClock.SetTime(trig_time);
      fTriggerClock.SetTime(trig_time);
    }

  public:

    static const TimeService* GetME(bool default_load=false)
    {
      if(!_me) _me = new TimeService(default_load);
      return _me;
    }

    /// Given Geant4 time [ns], returns relative time [us] w.r.t. electronics time T0 
    double G4ToElecTime(double g4_time) const {return g4_time * 1.e-3 - fG4RefTime; }

    /// TPC readout start time offset from trigger
    virtual double TriggerOffsetTPC() const { return fTriggerOffsetTPC; }

    /// Trigger electronics clock time in [us]
    double TriggerTime() const { return fTriggerTime; }

    /// Beam gate electronics clock time in [us]
    double BeamGateTime() const { return fBeamGateTime; }

    //
    // Getters of TPC ElecClock
    //
    /// Borrow a const TPC clock with time set to Trigger time [us]
    const ElecClock& TPCClock() const
    { return fTPCClock; }

    /// Create a TPC clock for a given time [us] from clock counting start
    ElecClock TPCClock(double time) const 
    { return ElecClock(time,fTPCClock.FramePeriod(),fTPCClock.Frequency());}

    /// Create a TPC clock for a given sample/frame number in TPC clock frequency
    ElecClock TPCClock(unsigned int sample,
		       unsigned int frame) const
    { ElecClock clock = TPCClock(); clock.SetTime(sample,frame); return clock; }

    //
    // Getters of Optical ElecClock
    //
    /// Borrow a const Optical clock with time set to Trigger time [us]
    const ElecClock& OpticalClock() const
    { return fOpticalClock; }

    /// Create a Optical clock for a given time [us] from clock counting start
    ElecClock OpticalClock(double time) const 
    { return ElecClock(time,fOpticalClock.FramePeriod(),fOpticalClock.Frequency());}

    /// Create a Optical clock for a given sample/frame number in Optical clock frequency
    ElecClock OpticalClock(unsigned int sample,
			   unsigned int frame) const
    { ElecClock clock = OpticalClock(); clock.SetTime(sample,frame); return clock; }

    //
    // Getters of Trigger ElecClock
    //
    /// Borrow a const Trigger clock with time set to Trigger time [us]
    const ElecClock& TriggerClock() const
    { return fTriggerClock; }

    /// Create a Trigger clock for a given time [us] from clock counting start    
    ElecClock TriggerClock(double time) const 
    { return ElecClock(time,fTriggerClock.FramePeriod(),fTriggerClock.Frequency());}

    /// Create a Trigger clock for a given sample/frame number in Trigger clock frequency
    ElecClock TriggerClock(unsigned int sample,
			   unsigned int frame) const
    { ElecClock clock = TriggerClock(); clock.SetTime(sample,frame); return clock; }

    //
    // Getters for time [us] w.r.t. trigger given information from waveform
    //

    /// Given TPC time-tick (waveform index), returns time [us] w.r.t. trigger time stamp
    double TPCTick2TrigTime(double tick) const
    { return fTPCClock.TickPeriod() * tick + TriggerOffsetTPC(); }
    /// Given TPC time-tick (waveform index), returns time [us] w.r.t. beam gate time
    double TPCTick2BeamTime(double tick) const
    { return fTPCClock.TickPeriod() * tick + TriggerOffsetTPC() + TriggerTime() - BeamGateTime(); }
    /// Given Optical time-tick (waveform index), sample and frame number, returns time [us] w.r.t. trigger time stamp
    double OpticalTick2TrigTime(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.TickPeriod() * tick + fOpticalClock.Time(sample,frame) - TriggerTime();  }
    /// Given Optical time-tick (waveform index), sample and frame number, returns time [us] w.r.t. beam gate time stamp
    double OpticalTick2BeamTime(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.TickPeriod() * tick + fOpticalClock.Time(sample,frame) - BeamGateTime(); }


    //
    // Getters for time [tdc] (electronics clock counting ... in double precision) 
    //

    /// Given TPC time-tick (waveform index), returns electronics clock count [tdc]
    double TPCTick2TDC(double tick) const
    { return ( (TriggerTime() + TriggerOffsetTPC()) / fTPCClock.TickPeriod() + tick ); }
    /// Given G4 time [ns], returns corresponding TPC electronics clock count [tdc]
    double TPCG4Time2TDC(double g4time) const
    { return G4ToElecTime(g4time) / fTPCClock.TickPeriod(); }
    /// Given Optical time-tick (waveform index), sample and frame number, returns time electronics clock count [tdc]
    double OpticalTick2TDC(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.Ticks(sample,frame) + tick; }
    /// Given G4 time [ns], returns corresponding Optical electronics clock count [tdc]  
    double OpticalG4Time2TDC(double g4time) const
    { return G4ToElecTime(g4time) / fOpticalClock.TickPeriod(); }

    //
    // Getters for time [us] (electronics clock counting ... in double precision)
    //
    /// Given TPC time-tick (waveform index), returns electronics clock [us]
    double TPCTick2Time(double tick) const
    { return TriggerTime() + TriggerOffsetTPC() + tick * fTPCClock.TickPeriod(); }
    /// Given Optical time-tick (waveform index), sample and frame number, returns electronics clock [us]
    double OpticalTick2Time(double tick, size_t sample, size_t frame) const
    { return fOpticalClock.Time(sample,frame) + tick * fOpticalClock.TickPeriod(); }

    //
    // Getters for time [ticks] (waveform index number)
    //

    /// Given electronics clock count [tdc] returns TPC time-tick
    double TPCTDC2Tick(double tdc) const
    { return ( tdc - (TriggerTime() + TriggerOffsetTPC()) / fTPCClock.TickPeriod() ); }
    /// Given G4 time returns electronics clock count [tdc]
    double TPCG4Time2Tick(double g4time) const
    { return (G4ToElecTime(g4time) - (TriggerTime() + TriggerOffsetTPC())) / fTPCClock.TickPeriod(); }


  };
  
}
#endif
/** @} */ // end of doxygen group 

