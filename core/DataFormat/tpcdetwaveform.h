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
  typedef short        ADC_Count_t;
  typedef double       TimeStamp_t;  ///< us since 1970, based on TimeService
  typedef unsigned int Channel_t;
  typedef unsigned int Event_t;
  typedef unsigned int Frame_t;

  
  class tpcdetwaveform : public std::vector< larlite::ADC_Count_t >,
			 public data_base {
    
  public:
    
    tpcdetwaveform();
    
    virtual ~tpcdetwaveform(){}
    
    // Functions included for backwards compatability with previous data types
    std::vector<ADC_Count_t>& Waveform()         { return *this;  }
    
    Channel_t   ChannelNumber() const            { return fChannel; }
    TimeStamp_t TimeStamp() const                { return fTimeStamp; }
    Event_t   Event() const { return fTimeStamp; }
    Frame_t   Frame() const { return fTimeStamp; }
    
    void SetChannelNumber(Channel_t chan) { fChannel = chan; }
    void SetTimeStamp(TimeStamp_t time)   { fTimeStamp = time; }
    void SetEvent(Channel_t chan)         { fChannel = chan; }
    void SetFrame(TimeStamp_t time)       { fTimeStamp = time; }

    inline bool operator< (const tpcdetwaveform& rhs) const
    {
      // Sort by channel, then time
      if ( fChannel < rhs.ChannelNumber() ) return true;
      if ( fChannel > rhs.ChannelNumber() ) return false;
      
      return ( fTimeStamp < rhs.TimeStamp() );
    }
    
  private:
    
    Channel_t   fChannel;
    TimeStamp_t fTimeStamp;
    Event_t fEvent;
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

  private:

  };

}
#endif

