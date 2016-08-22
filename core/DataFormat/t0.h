/**
 * \file t0.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for t0 data container
 *
 * @author David Caratelli - July 2016
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef LARLITE_T0_H
#define LARLITE_T0_H

#include "data_base.h"

namespace larlite {
  /**
     \class t0
     T0 Data container
  */
  class t0 : public data_base {
    
  public:

    /// Default constructor
    t0();

    /// Alternative contructor
    t0(double Time, unsigned int TriggerType, int TriggerBits, int ID=-1, double TriggerConfidence=-1);

    /// Copy constructor
    t0(const t0& origin);

    /// Default destructor
    virtual ~t0(){}

    friend std::ostream& operator << (std::ostream &o, t0 const& a);

    ///--- Getters ---///
    double          Time()              const;
    unsigned int    TriggerType()       const;
    int             TriggerBits()       const;
    int             ID()                const;
    double          TriggerConfidence() const;
    
    ///--- Setters ---///

    /// Clear method
    virtual void clear_data();


  private:

    double       fTime;
    unsigned int fTriggerType;
    int          fTriggerBits;
    int          fID;
    double       fTriggerConfidence; 

  };


  class event_t0 : public std::vector<larlite::t0>,
    public event_base {
      
  public:
      
      /// default ctor
  event_t0(std::string name="noname") : event_base(data::kT0,name) {clear_data();}
      
      /// Default copy constructor
  event_t0(const event_t0& original) : std::vector<larlite::t0>(original),
      event_base(original)
      {}
    
    ~event_t0() {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    };
  
}
#endif

/** @} */ // end of doxygen group 
