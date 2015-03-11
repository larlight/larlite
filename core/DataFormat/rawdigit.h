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
    inline rawdigit()
      : data_base(data::kRawDigit)
      , fChannel(0) 
      , fSamples(0) 
      , fPedestal(0.) 
      , fSigma(0.)
      , fADC(0)
      , fCompression(raw::kNone)
    {clear_data();}
    
    inline rawdigit(raw::ChannelID_t          channel,
		    unsigned short            samples,
		    const std::vector<short>& adclist,
		    raw::Compress_t           compression
		    )
      : data_base(data::kRawDigit)
      , fChannel(channel) 
      , fSamples(samples)
      , fPedestal(0.) 
      , fSigma(0.)
      , fADC(adclist) 
      , fCompression(compression)
    {clear_data();}
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

    // Set Methods
    void             SetPedestal(float ped);

    inline size_t           NADC()        const { return fADC.size();  }
    short                   ADC(int i)    const;
    inline raw::ChannelID_t Channel()     const { return fChannel;     }
    inline unsigned short   Samples()     const { return fSamples;     }
    inline float            GetPedestal() const { return fPedestal;    } 
    inline float            GetSigma()    const { return fSigma;       } 
    inline raw::Compress_t  Compression() const { return fCompression; }

  protected:

    raw::ChannelID_t fChannel;     ///< channel in the readout
    unsigned short   fSamples;     ///< number of ticks of the clock
    float            fPedestal;    ///< pedestal for this channel
    float            fSigma;       ///< sigma of the pedestal counts for this channel
    ADCvector_t fADC;         ///< ADC readout per tick, before pedestal subtraction 
    raw::Compress_t  fCompression; ///< compression scheme used for the ADC vector

  private:
    
    ////////////////////////
    ClassDef(rawdigit,2)
    ////////////////////////
      
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
    
    ////////////////////////
    ClassDef(event_rawdigit,2)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
