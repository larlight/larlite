/**
 * \class CRTTrack
 *
 * \ingroup larlite
 *
 * \brief CRT Track Info
 *
 *
 */


#ifndef LARLITE_CRTTRACK_H
#define LARLITE_CRTTRACK_H

#include "data_base.h"
#include <cstdint>
#include <vector>
#include <map>

namespace larlite {

  class crttrack : public data_base {
  public:

    /// Default constructor
    crttrack();

    /// Default destructor
    virtual ~crttrack(){}

    void clear_data();

    //struct CRTTrack{
    std::vector<uint8_t> feb_id;
    std::map< uint8_t, std::vector<std::pair<int,float> > > pesmap;
    float peshit;

    // CRT GPS clock in s     
    uint32_t ts0_s;
    uint16_t ts0_s_err;
    // CRT GPS clock in ns
    uint32_t ts0_ns;
    uint16_t ts0_ns_err;

    // CRT hit time w.r.t. BNB trigger
    int32_t ts1_ns;
    uint16_t ts1_ns_err;
    
    int plane1;
    int plane2;

    float x1_pos;
    float x1_err;
    float y1_pos;
    float y1_err;
    float z1_pos;
    float z1_err;
    float x2_pos;
    float x2_err;
    float y2_pos;
    float y2_err;
    float z2_pos;
    float z2_err;

    float length;
    float thetaxy;
    float phiz;

    // CRT GPS time of 1st hit 
    uint32_t ts0_ns_h1;
    uint16_t ts0_ns_err_h1;
    // CRT GPS time of 1nd hit
    uint32_t ts0_ns_h2;
    uint16_t ts0_ns_err_h2;      

    // time of 1st hit in TPC time frame
    float ts2_ns_h1;
    // time of 2nd hit in TPC time frame
    float ts2_ns_h2;


  };
  /**
     \class event_crttrack
     A collection storage class of multiple tracks.
  */
  class event_crttrack : public std::vector<larlite::crttrack>, 
    public event_base {
    
  public:
    
      /// Default constructor
  event_crttrack(std::string name="noname") : event_base(data::kCRTTrack,name) { clear_data(); }
    
      /// Default copy constructor
  event_crttrack(const event_crttrack& original) : std::vector<larlite::crttrack>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_crttrack(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
    };


}

#endif
