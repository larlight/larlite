/**
 * \file tpcfifo.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for base FEM data container
 *
 * @author David Caratelli - 02/2014
 */

/** \addtogroup DataFormat

    @{*/

#ifndef TPCFIFO_H
#define TPCFIFO_H

#include "data_base.h"
#include "fifo.h"
#include "Base/GeoConstants.h"
#include "Base/GeoTypes.h"

namespace larlite {

  /**
     \class fifo 
     Channel-wise data member class to hold a collection of ADC samples.
  */
  class tpcfifo : public fifo {
    
  public:

    /// Default constructor
    tpcfifo();
    
    /// Default copy constructor
    tpcfifo(const tpcfifo& original)
      : fifo::fifo(original)
    { _plane = original._plane ; _signal = original._signal; }

    /// Fast vector copy constructor
    tpcfifo(UShort_t ch,
	    UInt_t   frame,
	    UInt_t   sample,
	    UChar_t  module_address,
	    UChar_t  module_id,
	    larlite::geo::View_t plane,
	    larlite::geo::SigType_t signaltype,
	    //DATA::DATA_TYPE type,
	    std::vector<UShort_t> wf) : fifo::fifo(ch,
						   frame,
						   sample,
						   module_address,
						   module_id,
						   fem::kDISC_MAX,
						   //type,
						   wf)
    { _plane = plane;  _signal = signaltype; }
    
    
    /// plane info setter
    void set_plane ( larlite::geo::View_t plane ) { _plane=plane;}
    
    /// signal type setter
    void set_signal ( larlite::geo::SigType_t signal ) { _signal=signal;}

    /// plane info getter
    larlite::geo::View_t plane() const { return _plane; }
    
    /// signal type getter
    larlite::geo::SigType_t signal() const {return _signal; }

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data();
    
    /// Default destructor
    ~tpcfifo(){}
    
    
  protected:
    
    /// Internal function to reset variables.
    virtual void init_vars();

    larlite::geo::View_t      _plane;    ///< Plane Projection
    larlite::geo::SigType_t   _signal;   ///< Signal Type
    
    ////////////////////////
    //ClassDef(tpcfifo,6)
    ////////////////////////
  };
  
  
  /**
     \class event_fifo 
     Event-wise data member class to hold a collection of ch-wise data members
  */
  class event_tpcfifo : public std::vector<larlite::tpcfifo>, public event_base {
    
  public:
    
    /// Default constructor ... provide an option to set the length of ch-wise data
    event_tpcfifo(data::DataType_t type=data::kFIFO);
    
    /// Default copy constructor needed to avoid memory leak in ROOT streamer
    event_tpcfifo(const event_tpcfifo& original)
      : std::vector<larlite::tpcfifo>(original),
	event_base(original),
	_event_number(original._event_number),
	_event_frame_number(original._event_frame_number),
	_module_address(original._module_address),
	_module_id(original._module_id),
	_fem_trig_frame_number(original._fem_trig_frame_number),
	_fem_trig_sample_number(original._fem_trig_sample_number),
	_checksum(original._checksum),
	_nwords(original._nwords)
    {}
    
    /// Default destructor
    ~event_tpcfifo(){}

    /// Setter for the evnet number
    void set_event_number(UInt_t n) { _event_number = n; }
    
    /// Setter for the event frame number
    void set_event_frame_number(UInt_t n) {_event_frame_number=n;}
    
    /// Setter for the module address from which this event is read-out
    void set_module_address(UChar_t n) {_module_address=n;}
    
    /// Setter for the module ID
    void set_module_id(UChar_t id)      {_module_id=id;}
    
    /// Setter for the checksum
    void set_checksum(UInt_t sum)      {_checksum=sum;}
    
    /// Setter for the # words read out
    void set_nwords(UInt_t n)          {_nwords=n;}
    
    /// Setter for the trigger frame id
    void set_fem_trig_frame_number(UInt_t n) {_fem_trig_frame_number=n;}
    
    /// Setter for the trigger timeslice
    void set_fem_trig_sample_number(UInt_t n) {_fem_trig_sample_number=n;}

    /// Getter for the event number
    UInt_t event_number() const { return _event_number;}
    
    /// Getter for the frame ID
    UInt_t event_frame_number() const {return _event_frame_number;}
    
    /// Getter for the module address
    UChar_t module_address() const {return _module_address;}
    
    /// Getter for the module ID
    UChar_t module_id()      const {return _module_id;}
    
    /// Getter for the checksum
    UInt_t checksum()       const {return _checksum;} 
    
    /// Getter for the number of read words
    UInt_t nwords()         const {return _nwords;}
    
    /// Getter for the trigger frame number
    UInt_t fem_trig_frame_number() const {return _fem_trig_frame_number;}

    /// Getter for the fem trigger sample number RAW
    inline UInt_t fem_trig_sample_number_RAW() const {return _fem_trig_sample_number;}
    
    /// Getter for the fem trigger sample number in 2 MHz
    inline UInt_t fem_trig_sample_number_2MHz() const {return _fem_trig_sample_number;}    

    /// Getter for the fem trigger sample number in 16 MHz
    inline UInt_t fem_trig_sample_number_16MHz() const {return _fem_trig_sample_number*8;}

    /// Getter for the fem trigger sample number in 64 MHz
    inline UInt_t fem_trig_sample_number_64MHz() const {return _fem_trig_sample_number*32;}
    
    /// A function to reset data member variables
    virtual void clear_data();
    
    
  protected:
    
    virtual void init_vars();
    /// Actual implementation function of resetting variables
    UInt_t  _event_number;              ///< event number counter
    UInt_t  _event_frame_number;        ///< event frame ID number
    UChar_t _module_address;            ///< module address number
    UChar_t _module_id;                 ///< module ID number
    
    UInt_t _fem_trig_frame_number;     ///< trigger frame id
    UInt_t _fem_trig_sample_number;    ///< trigger time slice
    
    UInt_t _checksum;                  ///< checksum of readout events
    UInt_t _nwords;                    ///< # of event words readout
    
    ///////////////////////////
    //ClassDef(event_tpcfifo,7)
    //////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 

