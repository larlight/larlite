#ifndef LARLITE_TRIGGERDATA_H
#define LARLITE_TRIGGERDATA_H

// C++ includes
#include <vector>
#include <limits>
#include <stdexcept>
#include <iostream>
#include "data_base.h"
#include "Base/FEMConstants.h"

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

	_trig_sample_number = 0xffff;
	_trig_frame_number  = fem::kINVALID_WORD;

	//_trig_number        = fem::kINVALID_WORD;
	
	_pmt_data       = 0xffff;
	_trig_pc        = false;
	_trig_ext       = false;
	_active         = false;
	_gate1_in       = false;
	_gate2_in       = false;
	_veto_in        = false;
	_calib          = false;
	_remainder_16MHz = 0xffff;
	_remainder_64MHz = 0xffff;
	
      }
    
  trigger(const trigger&rhs) : event_base(rhs)
      , fTriggerNumber(rhs.fTriggerNumber)
      , fTriggerTime(rhs.fTriggerTime)
      , fBeamGateTime(rhs.fBeamGateTime)
      , fTriggerBits(rhs.fTriggerBits)
      {}
    
  protected:
    
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
      : event_base(),
      fTriggerNumber       ( counter           ),
      fTriggerTime         ( trigger_time      ),
      fBeamGateTime        ( beamgate_time     ),
      fTriggerBits         ( bits              )
    {}

    /// Trigger number
    void TriggerNumber(unsigned int n)  { fTriggerNumber = n; }
    /// Trigger time w.r.t. electronics clock T0 in us
    void TriggerTime  (double t)        { fTriggerTime = t;   }
    /// BeamGate time w.r.t. electronics clock T0 in us
    void BeamGateTime (double t)        { fBeamGateTime = t;  }
    /// Trigger Bits
    void TriggerBits  (unsigned int b)  { fTriggerBits = b;   }

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

    /// Setter for 16 MHz remainder
    void set_remainder_16MHz(UShort_t remin) {_remainder_16MHz=remin;}
    
    /// Setter for 64 MHz remainder
    void set_remainder_64MHz(UShort_t remin) {_remainder_64MHz=remin;}
    
    /// Setter for frame number
    void set_trig_frame_number(UInt_t n) {_trig_frame_number=n;}
    
    /// Setter for sample number
    void set_trig_sample_number(UInt_t t) {_trig_sample_number=t;}
    
    /// Setter for trigger ID
    void set_trig_number(unsigned int n)     { fTriggerNumber=n; }
    
    /// Setter for PMT trigger data
    void set_pmt_data(UShort_t data)      { _pmt_data=data; }
    
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
    unsigned int trig_number() const        {return fTriggerNumber;}
    
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

    //UInt_t    _trig_number;

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
    
  };
}

#endif 
