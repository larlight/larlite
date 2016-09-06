/**
 * raw/TPCDetWaveform.h
 *
 */

#ifndef LARLITE_TPCDETWAVEFORM_H
#define LARLITE_TPCDETWAVEFORM_H

#include "data_base.h"
#include <vector>
#include <limits>
#include <iosfwd>

namespace larlite {
  
  // Define the types used
  typedef short    ADC_Count_t;
  typedef double   TimeStamp_t;  ///< us since 1970, based on TimeService
  typedef unsigned Channel_t;
  typedef unsigned Event_t;
  typedef unsigned Frame_t;

  class tpcdetwaveform : public std::vector< larlite::ADC_Count_t >,
			 public data_base {
    
  public:
    
    tpcdetwaveform();
    
    virtual ~tpcdetwaveform(){}
    
    // Functions included for backwards compatability with previous data types
    std::vector<ADC_Count_t>& Waveform() { return *this;  }
    
    Channel_t ChannelNumber() const { return fChannel; }
    TimeStamp_t TimeStamp() const { return fTimeStamp; }
    Frame_t Frame() const { return fFrame; }
    
    void SetChannelNumber(Channel_t chan) { fChannel = chan; }
    void SetTimeStamp(TimeStamp_t time)   { fTimeStamp = time; }
    void SetFrame(Frame_t frame) { fFrame = frame; }

  private:

    Channel_t fChannel;
    TimeStamp_t fTimeStamp;
    Frame_t fFrame;
  };
  

  class event_tpcdetwaveform : public std::vector<larlite::tpcdetwaveform>,
			       public event_base {
  public:

    /// default ctor
    event_tpcdetwaveform(std::string name="noname") : event_base(data::kTPCDetWaveform,name) {clear_data();}

    /// Default copy constructor
    event_tpcdetwaveform(const event_tpcdetwaveform& original) : std::vector<larlite::tpcdetwaveform>(original),
      event_base(original)
    {}
    
    ~event_tpcdetwaveform() {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

    const Event_t Event() const { return fEvent; }
    const Frame_t Frame() const { return fFrame; }
    
    void SetEvent(Event_t event) { fEvent = event; }
    void SetFrame(Frame_t frame) { fFrame = frame; }

  private:
    
    Event_t fEvent;
    Frame_t fFrame;
    
  };

}
#endif

