/**
 * \file trigger.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for trigger
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/
#ifndef TRIGGER_HH
#define TRIGGER_HH

#include "data_base.hh"

namespace larlight {

  /**
     \class trigger
     A class to hold trigger module output data
  */
  class trigger : public event_base {
    
  public:
    
    /// Default constructor
    trigger(DATA::DATA_TYPE type=DATA::Trigger);
    
    /// Default copy constructor to avoid memory leak in ROOT streamer
    trigger(const trigger &original);
    
    /// Default destructor
    virtual ~trigger(){}
    
    /// Setter for 16 MHz remainder
    void set_remainder_16MHz(UShort_t remin) {_remainder_16MHz=remin;}
    
    /// Setter for 64 MHz remainder
    void set_remainder_64MHz(UShort_t remin) {_remainder_64MHz=remin;}
    
    /// Setter for frame number
    void set_trig_frame_number(UInt_t n) {_trig_frame_number=n;}
    
    /// Setter for sample number
    void set_trig_sample_number(UInt_t t) {_trig_sample_number=t;}
    
    /// Setter for trigger ID
    void set_trig_number(UInt_t n)     {_trig_number=n;}
    
    /// Setter for PMT trigger data
    void set_pmt_data(UShort_t data)      {_pmt_data=data;}
    
    /// Getter for 16 MHz remainder
    UShort_t remainder_16MHz() const  {return _remainder_16MHz;}

    /// Getter for 64 MHz remainder
    UShort_t remainder_64MHz() const  {return _remainder_64MHz;}
    
    /// Getter for frame number
    UInt_t trig_frame_number() const  {return _trig_frame_number;}

    /// Getter for a RAW readout sample number
    UInt_t trig_sample_number_RAW() const { return _trig_sample_number; }
    
    /// Getter for sample number in 2MHz
    UInt_t trig_sample_number_2MHz() const {return _trig_sample_number;}

    /// Getter for sample number in 16MHz
    inline UInt_t trig_sample_number_16MHz() const {return _trig_sample_number*8 + _remainder_16MHz;}

    /// Getter for sample number in 64MHz
    inline UInt_t trig_sample_number_64MHz() const {return _trig_sample_number*32 + _remainder_16MHz*4 + _remainder_64MHz;}
    
    /// Getter for trigger ID
    UInt_t trig_number() const        {return _trig_number;}
    
    /// Getter for PMT trigger data
    UShort_t pmt_data() const         {return _pmt_data;}
    
    /// Setter for trigger bits
    void set_trig_bits(Bool_t trig_pc,  Bool_t trig_ext,
		       Bool_t active,   Bool_t gate1_in,
		       Bool_t gate2_in, Bool_t veto_in,
		       Bool_t calib)
    { _trig_pc  = trig_pc;   _trig_ext = trig_ext;
      _active   = active;    _gate1_in = gate1_in;
      _gate2_in = gate2_in; _veto_in  = veto_in;
      _calib    = calib;
    }
    
    /// Getter for trigger bit
    Bool_t trig_pc() const  {return _trig_pc;}
    
    /// Getter for trigger bit
    Bool_t trig_ext() const {return _trig_ext;}
    
    /// Getter for trigger bit
    Bool_t active() const   {return _active;}
     
    /// Getter for trigger bit
    Bool_t gate1_in() const {return _gate1_in;}
    
    /// Getter for trigger bit
    Bool_t gate2_in() const {return _gate2_in;}
    
    /// Getter for trigger bit
    Bool_t veto_in() const  {return _veto_in;}
    
    /// Getter for trigger big
    Bool_t calib() const    {return _calib;}
    
    /// Clear method override
    virtual void clear_data();
    
  protected:

    void init_vars();
    
    UShort_t  _trig_sample_number; // 2MHz
    UInt_t    _trig_frame_number;
    UInt_t    _trig_number;
    UShort_t  _pmt_data;
    Bool_t    _trig_pc;
    Bool_t    _trig_ext;
    Bool_t    _active;
    Bool_t    _gate1_in;
    Bool_t    _gate2_in;
    Bool_t    _veto_in;
    Bool_t    _calib;
    UShort_t  _remainder_64MHz;
    UShort_t  _remainder_16MHz;
    
    ////////////////////////
    ClassDef(trigger,4)
    ////////////////////////
    
  };
}

#endif
/** @} */ // end of doxygen group 
