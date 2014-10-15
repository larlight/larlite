/**
 * \file wire.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for wire data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef WIRE_H
#define WIRE_H

#include "data_base.h"

namespace larlite {
  /**
     \class wire
     LArSoft Wire class equivalent data container
  */
  class wire : public data_base {
    
  public:
    
    /// Default constructor
    wire() : data_base(data::kWire) {clear_data();}

    /// Alternative constructor
    wire(const std::vector<Float_t> wf,
	 const UInt_t ch,
	 const geo::View_t view,
	 const geo::SigType_t sig) 
      : data_base(data::kWire),
	fSignal(wf),
	fView(view),
	fSignalType(sig),
	fChannel(ch)
    {};
    
    /// Copy constructor
    wire(const wire& original) : data_base(original),
				 fSignal(original.fSignal),
				 fView(original.fView),
				 fSignalType(original.fSignalType),
				 fChannel(original.fChannel)
    {};
    
    /// Default destructor
    virtual ~wire(){};
    
    void clear_data();
    
    /// Setter for an waveform
    void set_signal(const std::vector<Float_t> wf) { fSignal=wf; }
    /// Setter for a channel number
    void set_channel(UInt_t    ch)   { fChannel = ch;   }
    /// Setter for view type
    void set_view   (geo::View_t v)    { fView = v;       }
    /// Setter for signal type
    void set_sigtype(geo::SigType_t s) { fSignalType = s; }
    
    // Get Methods
    /// Getter for waveform
    const std::vector<Float_t>&  Signal()     const { return fSignal;        }
    /// Getter for view type
    geo::View_t                View()       const { return fView;          }
    /// Getter for signal type
    geo::SigType_t             SignalType() const { return fSignalType;    }
    /// Getter for channel number
    UInt_t                   Channel()    const { return fChannel;       }
    
  protected:
    
    std::vector<Float_t> fSignal;     ///< the calibrated signal waveform
    geo::View_t        fView;       ///< view corresponding to the plane of this wire
    geo::SigType_t     fSignalType; ///< signal type of the plane for this wire
    UInt_t             fChannel;    ///< channel number
    
  private:
    
    ////////////////////////
    ClassDef(wire,1)
    ////////////////////////
      
  };

  /**
     \class event_wire
     A collection storage class of multiple wires.
  */
  class event_wire : public std::vector<larlite::wire>,
		     public event_base {
    
  public:
    
    /// Default constructor
    event_wire(std::string name="noname") : event_base(data::kWire,name) {clear_data();}
    
    /// Default copy constructor
    event_wire(const event_wire& original) : std::vector<larlite::wire>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_wire(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_wire,2)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
