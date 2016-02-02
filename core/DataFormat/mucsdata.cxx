
////////////////////////////////////////////////////////////////////////
//
//    trivia : Class to hold the Muon Counter System (MuCS) data, 
//             a.k.a. the best class there's ever been, May 2015 
//    author : Odysseas Kanenas
//    e-mail : kalousis@vt.edu
//
////////////////////////////////////////////////////////////////////////

#include "mucsdata.h"

namespace larlite {

  mucsdata::mucsdata() : data_base(data::kMuCSData)
  {}
  
  mucsdata::~mucsdata()
  {}
  
  mucsdata::mucsdata( float t0, float adc1[24], float adc2[24], float adc3[24], float adc7[24],
		      const std::vector<int>& hits1,
		      const std::vector<int>& hits2,
		      const std::vector<int>& hits3,
		      const std::vector<int>& hits7 ) 
    : data_base(data::kMuCSData)
    , fhits1(hits1)
    , fhits2(hits2)
    , fhits3(hits3)
    , fhits7(hits7)
  {
    ft0 = t0;
    
    for ( int i=0; i<24; i++ ) 
      { 
	fadc1[i]=adc1[i]; fadc2[i]=adc2[i];
	fadc3[i]=adc3[i]; fadc7[i]=adc7[i];
	
      }
  }
  
  float mucsdata::T0() const
  { return ft0; }
  
  const std::vector<float> mucsdata::ADC1() const
  {
    std::vector<float> fadc(24,0.);
    memcpy(&fadc[0],fadc1,sizeof(float)*fadc.size());
    return fadc;
  }

  const std::vector<float> mucsdata::ADC2() const
  {
    std::vector<float> fadc(24,0.);
    memcpy(&fadc[0],fadc2,sizeof(float)*fadc.size());
    return fadc;
  }
  
  const std::vector<float> mucsdata::ADC3() const
  {
    std::vector<float> fadc(24,0.);
    memcpy(&fadc[0],fadc3,sizeof(float)*fadc.size());
    return fadc;
  }
  
  const std::vector<float> mucsdata::ADC7() const
  {
    std::vector<float> fadc(24,0.);
    memcpy(&fadc[0],fadc7,sizeof(float)*fadc.size());
    return fadc;
  }

  const std::vector<int>& mucsdata::Hits1() const
  { return fhits1; }

  const std::vector<int>& mucsdata::Hits2() const
  { return fhits2; }
  
  const std::vector<int>& mucsdata::Hits3() const
  { return fhits3; }
  
  const std::vector<int>& mucsdata::Hits7() const
  { return fhits7; }

}

////////////////////////////////////////////////////////////////////////
//
//    The end !
//
////////////////////////////////////////////////////////////////////////
