#ifndef __FLASH_HH__
#define __FLASH_HH__

#ifndef __FORLARLITE__
// ---- LARSOFT ---
#ifdef __BUILD_ROOT_DICT__
#include "TObject.h"
#endif
#include <vector>

namespace subevent {
#ifdef __BUILD_ROOT_DICT__
  class Flash : public TObject {
#else
  class Flash {
// ---------------
#endif
#else 
// ---- LARLITE ---
#include "data_base.h"

namespace larlite {

  class Flash : public data_base {
// ---------------
#endif

  public:
    
    Flash();
    Flash( int ch, int tstart, int tend, int tmax, float maxamp, std::vector< double >& expectation, std::vector< double >& waveform );
    Flash( const Flash& orig ); // copy constructor
    ~Flash();
    
    void storeWaveform( const std::vector< double >& waveform );
    void storeExpectation( const std::vector< double >& expectation );

    int ch;
    int tstart;
    int tend;
    int tmax;
    double maxamp;
    double area;
    double area30;
    double fcomp_gausintegral;
    bool claimed;
    std::vector< double > expectation;
    std::vector< double > waveform;
#ifdef __BUILD_ROOT_DICT__
    ClassDef( Flash, 1 );
#endif
  };


#ifdef __FORLARLITE__
  // ---------------------------------------------------------------------------
  // LARLITE
  class event_Flash : public std::vector<larlite::Flash>,
			public event_base {
    
  public:

    /// Default ctor
    event_Flash(std::string name="noname") : event_base(data::kFlash,name) {clear_data();}

    /// Default copy constructor
    event_Flash(const event_Flash& original) : std::vector<larlite::Flash>(original),
						   event_base(original)
    {}

    ~event_Flash(){}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
  // ---------------------------------------------------------------------------
#endif
}


#endif
