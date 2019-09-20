/**
 * \class CRTHit
 *
 * \ingroup larlite
 *
 * \brief CRT Hit Info
 *
 * \author $Author: David Lorca $
 *
 */


#ifndef LARLITE_CRTHIT_H
#define LARLITE_CRTHIT_H

#include "data_base.h"
#include <cstdint>
#include <vector>
#include <map>

namespace larlite {

  class crthit : public data_base {
  public:

    /// Default constructor
    crthit();

    /// Default destructor
    virtual ~crthit(){}

    void clear_data();

    //struct CRTHit{
    std::vector<uint8_t> feb_id;
    std::map< uint8_t, std::vector<std::pair<int,float> > > pesmap;
    float peshit;

    // CRT GPS clock in s     
    uint32_t ts0_s;
    int8_t ts0_s_corr;
      
    // CRT GPS clock in ns
    uint32_t ts0_ns;
    int32_t ts0_ns_corr;

    // CRT time w.r.t. BNB trigger
    int32_t ts1_ns;

    // CRT hit time in TPC time frame 
    // can be directly compared to TPC and optical object timing 
    float ts2_ns;
      
    int plane;
    float x_pos;
    float x_err;
    float y_pos;
    float y_err;
    float z_pos;
    float z_err;
      
    //uint16_t event_flag;
    //std::map< uint8_t, uint16_t > lostcpu_map;
    //std::map< uint8_t, uint16_t > lostfpga_map;
    //uint16_t pollms;

    //CRTHit() {}

  };
  /**
     \class event_crthit
     A collection storage class of multiple hits.
  */
  class event_crthit : public std::vector<larlite::crthit>, 
    public event_base {
    
  public:
    
      /// Default constructor
  event_crthit(std::string name="noname") : event_base(data::kCRTHit,name) { clear_data(); }
    
      /// Default copy constructor
  event_crthit(const event_crthit& original) : std::vector<larlite::crthit>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_crthit(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
    };


}

#endif
