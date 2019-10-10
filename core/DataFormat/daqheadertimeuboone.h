////////////////////////////////////////////////////////////////////////
// Name: DAQHeaderTimeUBooNE.h
//
// Purpose: Class to hold extended DAQ header information, in particular
//          GPS and NTP (host) event time stamp.
//
// Copied & modified from ubobj/RawData/DAQHeaderTimeUBooNE.h
//
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_DAQHEADERTIMEUBOONE_H
#define LARLITE_DAQHEADERTIMEUBOONE_H

#include <vector>
#include "data_base.h"
#include <time.h>

namespace larlite {

  class daqheadertimeuboone : public data_base {
  public:

    // Constructors.

    daqheadertimeuboone();
    virtual ~daqheadertimeuboone() {};

    void clear_data();

    //Set Methods

    void SetGPSTime(time_t t);
    void SetNTPTime(time_t t);
    void SetPPSTime(uint32_t sec, uint32_t micro, uint32_t nano);
    void SetTrigTime(uint32_t frame, uint16_t sample, uint16_t div);
    void SetTrigPPSTime(uint32_t frame, uint16_t sample, uint16_t div);
    void SetEventTimeStamp(time_t t);

    // Accessors.

    time_t gps_time() const {return fGPSTime;}
    time_t ntp_time() const {return fNTPTime;}

    uint32_t pps_sec() const {return fPPSsec;}
    uint32_t pps_micro() const {return fPPSmicro;}
    uint32_t pps_nano() const {return fPPSnano;}

    uint32_t trig_frame() const {return fTrigFrame;}
    uint16_t trig_sample() const {return fTrigSample;}
    uint16_t trig_div() const {return fTrigDiv;}

    uint32_t trig_pps_frame() const {return fTrigPPSFrame;}
    uint16_t trig_pps_sample() const {return fTrigPPSSample;}
    uint16_t trig_pps_div() const {return fTrigPPSDiv;}

    time_t evt_timestamp() const {return fTimeStamp;}

  private:

    // Data members.

    // Complete event times.

    time_t fGPSTime;    // (high, low)=(seconds, nanoseconds)
    time_t fNTPTime;    // (high, low)=(seconds, nanoseconds)

    // GPS PPS.

    uint32_t fPPSsec;   // PPS seconds.
    uint32_t fPPSmicro; // PPS microseconds.
    uint32_t fPPSnano;  // PPS nanoseconds.

    // Trigger event time.

    uint32_t fTrigFrame;  // Event frame (1.6 ms).
    uint16_t fTrigSample; // Event sample (2 MHz).
    uint16_t fTrigDiv;    // Event division (16 MHz).

    // Trigger PPS time.

    uint32_t fTrigPPSFrame;  // PPS frame (1.6 ms).
    uint16_t fTrigPPSSample; // PPS sample (2 MHz).
    uint16_t fTrigPPSDiv;    // PPS division (16 MHz).

    // Event time stamp (copied over from DAQHeader)
    time_t fTimeStamp;

  };

  /*
    Class event_daqheadertimeuboone
   */
  class event_daqheadertimeuboone : public std::vector<larlite::daqheadertimeuboone>,
    public event_base {
    
  public:
    
      /// Default constructor
  event_daqheadertimeuboone(std::string name="noname") : event_base(data::kDAQHeaderTimeUBooNE,name) { clear_data(); }
    
      /// Default copy constructor
  event_daqheadertimeuboone(const event_daqheadertimeuboone& original) : std::vector<larlite::daqheadertimeuboone>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_daqheadertimeuboone(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    };
}

inline void larlite::daqheadertimeuboone::SetGPSTime(time_t t)    { fGPSTime = t; }
inline void larlite::daqheadertimeuboone::SetNTPTime(time_t t)    { fNTPTime = t; }
inline void larlite::daqheadertimeuboone::SetPPSTime(uint32_t sec, uint32_t micro, uint32_t nano)
{
  fPPSsec=sec;
  fPPSmicro=micro;
  fPPSnano=nano;
}
inline void larlite::daqheadertimeuboone::SetTrigTime(uint32_t frame, uint16_t sample, uint16_t div)
{
  fTrigFrame=frame;
  fTrigSample=sample;
  fTrigDiv=div;
}
inline void larlite::daqheadertimeuboone::SetTrigPPSTime(uint32_t frame, uint16_t sample, uint16_t div)
{
  fTrigPPSFrame=frame;
  fTrigPPSSample=sample;
  fTrigPPSDiv=div;
}

inline void larlite::daqheadertimeuboone::SetEventTimeStamp(time_t t){ fTimeStamp = t;}

#endif // DAQHEADERTIMEUBOONE_H
