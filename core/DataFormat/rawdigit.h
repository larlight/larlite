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
#include <vector>
#include <exception>
namespace larlite{
  /**
     \class rawdigit
     LArSoft rawdigit class equivalent data container
  */
  class rawdigit : public data_base {
    
  public:

    /// How interesting: public adc vector. Awesome design LArSoft :D
    std::vector<short> fADC;

    /// Default constructor
    inline rawdigit()
      : data_base(data::kRawDigit)
      , fADC(0)
      , fChannel(0) 
      , fSamples(0) 
      , fPedestal(0.) 
      , fSigma(0.)
      , fCompression(raw::kNone)
    {clear_data();}
    
    inline rawdigit(unsigned int              channel,
		    unsigned short            samples,
		    const std::vector<short>& adclist,
		    raw::Compress_t           compression
		    )
      : data_base(data::kRawDigit)
      , fADC(adclist) 
      , fChannel(channel) 
      , fSamples(samples)
      , fPedestal(0.) 
      , fSigma(0.)
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
    void             SetPedestal(double ped);

    inline unsigned int    NADC()        const { return fADC.size();  }
    short                  ADC(int i)    const;
    inline unsigned int    Channel()     const { return fChannel;     }
    inline unsigned short  Samples()     const { return fSamples;     }
    inline double          GetPedestal() const { return fPedestal;    } 
    inline double          GetSigma()    const { return fSigma;       } 
    inline raw::Compress_t Compression() const { return fCompression; }
    
  protected:

    unsigned int    fChannel;     ///< channel in the readout
    unsigned short  fSamples;     ///< number of ticks of the clock
    
    double          fPedestal;    ///< pedestal for this channel
    double          fSigma;       ///< sigma of the pedestal counts for this channel

    raw::Compress_t fCompression; ///< compression scheme used for the ADC vector

  private:
    
    ////////////////////////
    ClassDef(rawdigit,1)
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
    ClassDef(event_rawdigit,1)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
