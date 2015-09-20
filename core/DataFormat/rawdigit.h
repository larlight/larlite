/**
 * \file rawdigit.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for rawdigit data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_RAWDIGIT_H
#define LARLITE_RAWDIGIT_H

#include "data_base.h"
#include "Base/RawConstants.h"
#include "Base/GeoTypes.h"
#include <vector>
#include <exception>
namespace larlite{

  typedef std::vector<short> ADCvector_t;
  /**
     \class rawdigit
     LArSoft rawdigit class equivalent data container
  */
  class rawdigit : public data_base {
    
  public:

    /// Default constructor
    rawdigit();
    
    rawdigit(raw::ChannelID_t          channel,
	     unsigned short            samples,
	     const std::vector<short>& adclist,
	     raw::Compress_t           compression
	     );
#ifndef __CINT__
    rawdigit(raw::ChannelID_t     channel,
	     unsigned short       samples,
	     std::vector<short>&& adclist,
	     raw::Compress_t      compression
	     );
#endif
    /*
    inline rawdigit(unsigned int              channel,
		    const std::vector<short>& adclist,
		    raw::Compress_t           compression
		    )
      : rawdigit(channel, 0, adclist, compression)
    {clear_data();}
    */    
    /// Default destructor
    virtual ~rawdigit(){}

    void clear_data() {data_base::clear_data();}

    const std::vector<short>& ADCs()        const;
    size_t                    NADC()        const;
    short                     ADC(int i)    const;
    raw::ChannelID_t          Channel()     const;
    unsigned short            Samples()     const;
    float                     GetPedestal() const;
    float                     GetSigma()    const;
    raw::Compress_t           Compression() const;

    // Set Methods
    void             SetPedestal(float ped);
    
  protected:

    raw::ChannelID_t fChannel;     ///< channel in the readout
    unsigned short   fSamples;     ///< number of ticks of the clock
    float            fPedestal;    ///< pedestal for this channel
    float            fSigma;       ///< sigma of the pedestal counts for this channel
    ADCvector_t fADC;         ///< ADC readout per tick, before pedestal subtraction 
    raw::Compress_t  fCompression; ///< compression scheme used for the ADC vector
    
  };
  
  /**
     \class event_rawdigit
     A collection storage class of multiple rawdigits.
  */
  class event_rawdigit : public std::vector<larlite::rawdigit>, 
		    public event_base {
    
  public:
    
    /// Default constructor
    event_rawdigit(std::string name="noname") : event_base(data::kRawDigit,name) { clear_data(); }
    
    /// Default copy constructor
    event_rawdigit(const event_rawdigit& original) : std::vector<larlite::rawdigit>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_rawdigit(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
