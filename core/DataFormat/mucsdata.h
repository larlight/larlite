
////////////////////////////////////////////////////////////////////////
//
//    trivia : Class to hold the Muon Counter System (MuCS) data, 
//             a.k.a. the best class there's ever been, May 2015 
//    author : Odysseas Kanenas
//    e-mail : kalousis@vt.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_MUCSDATA_H
#define LARLITE_MUCSDATA_H

#include <vector>
#include "data_base.h"
namespace larlite {
 
  class mucsdata : public data_base {
    
  public:
    mucsdata();
    virtual ~mucsdata();  
    
    mucsdata( float t0, float adc1[24], float adc2[24], float adc3[24], float adc7[24], 
	      const std::vector<int>& hits1, const std::vector<int>& hits2, const std::vector<int>& hits3,
	      const std::vector<int>& hits7 ); 
    
    float T0() const;
    
    const std::vector<float> ADC1() const; 
    const std::vector<float> ADC2() const; 
    const std::vector<float> ADC3() const; 
    const std::vector<float> ADC7() const; 
    
    const std::vector<int>& Hits1() const;
    const std::vector<int>& Hits2() const;
    const std::vector<int>& Hits3() const;
    const std::vector<int>& Hits7() const;
    
  private:
    
    float ft0;
    
    float fadc1[24];
    float fadc2[24];
    float fadc3[24];
    float fadc7[24];
    
    std::vector<int> fhits1;
    std::vector<int> fhits2;
    std::vector<int> fhits3;
    std::vector<int> fhits7;
    
  };

  /**
     \class event_mucsdata
     A collection storage class of multiple mucsdata.
  */
  class event_mucsdata : public std::vector<larlite::mucsdata>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_mucsdata(std::string name="noname") : event_base(data::kMuCSData,name) { clear_data(); }
    
    /// Default copy constructor
    event_mucsdata(const event_mucsdata& original) : std::vector<larlite::mucsdata>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_mucsdata(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
  
}

#endif 

////////////////////////////////////////////////////////////////////////
//
//    The end !
//
////////////////////////////////////////////////////////////////////////
