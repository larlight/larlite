
#ifndef LARLITE_RAWDIGIT_CXX
#define LARLITE_RAWDIGIT_CXX

#include "rawdigit.h"

namespace larlite {

  rawdigit::rawdigit()
    : data_base(data::kRawDigit)
    , fChannel(0) 
    , fSamples(0) 
    , fPedestal(0.) 
    , fSigma(0.)
    , fADC(0)
    , fCompression(raw::kNone)
  {clear_data();}
  
  rawdigit::rawdigit(raw::ChannelID_t          channel,
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

  rawdigit::rawdigit(raw::ChannelID_t     channel,
		     unsigned short       samples,
		     std::vector<short>&& adclist,
		     raw::Compress_t      compression
		     )
    : data_base(data::kRawDigit)
    , fChannel(channel) 
    , fSamples(samples)
    , fPedestal(0.) 
    , fSigma(0.)
    , fADC(std::move(adclist) )
    , fCompression(compression)
  {clear_data();}

  
  //--------------------------------------------------
  short rawdigit::ADC(int i) const
  {
    unsigned int j = i;
    if(i < 0 || (j>fADC.size()) ) {
      std::cerr << "\033[93m" << __PRETTY_FUNCTION__ <<"\033[00m"
		<< "asked for illegal ADC index: " << i << "\n";
      throw std::exception();
    }

    return fADC[j];
  }


  //----------------------------------------------------------------------
  void rawdigit::SetPedestal(float ped)
  {
    fPedestal = ped;
    fSigma = 1.;
  }

  const std::vector<short>&  rawdigit::ADCs()        const { return fADC;         }
  size_t                     rawdigit::NADC()        const { return fADC.size();  }
  raw::ChannelID_t           rawdigit::Channel()     const { return fChannel;     }
  unsigned short             rawdigit::Samples()     const { return fSamples;     }
  float                      rawdigit::GetPedestal() const { return fPedestal;    } 
  float                      rawdigit::GetSigma()    const { return fSigma;       } 
  raw::Compress_t            rawdigit::Compression() const { return fCompression; }

}
#endif
