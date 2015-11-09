#ifndef __SUBEVENT__
#define __SUBEVENT__

#ifndef __FORLARLITE__
// LARSOFT
#ifdef __BUILD_ROOT_DICT__
#include "TObject.h"
#endif

#include "Flash.hh"
#include "FlashList.hh"

namespace subevent {

#ifdef __BUILD_ROOT_DICT__
  class SubEvent : public TObject {
#else
  class SubEvent {
#endif

#else

#include "tflash.h"
#include "tflashlist.h"

namespace larlite {
  
  class SubEvent : public data_base {
      
#endif
    
  public:
    
    SubEvent();
    ~SubEvent();

    int tstart_sample;
    int tend_sample;
    int tmax_sample;

    double tstart_ns;
    double tend_ns;
    double tmax_ns;
    
    double maxamp;
    double totpe;
    double pe30;
    double totpe_1; // first pass
    double pe30_1;  // first pass
    double sumflash30;
    double sumfcomp_gausintegral;

    // you made me do this art
    int runid;
    int subrunid;
    int eventid;
    
    FlashList flashes;        // first pass flashes
    FlashList flashes_pass2;  // second pass flashes
    void transferFlashes( FlashList& flashes );

#ifndef __FORLARLITE__
    
#ifdef __BUILD_ROOT_DICT__
    ClassDef( SubEvent, 1 );
#endif

#endif
    
  };

#ifdef __FORLARLITE__
  // ---------------------------------------------------------------------------
  // LARLITE
  class event_SubEvent : public std::vector<larlite::SubEvent>,
			public event_base {
    
  public:

    /// Default ctor
    event_SubEvent(std::string name="noname") : event_base(data::kSubEvent,name) {clear_data();}

    /// Default copy constructor
    event_SubEvent(const event_SubEvent& original) : std::vector<larlite::SubEvent>(original),
						   event_base(original)
    {}

    ~event_SubEvent(){}

    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
  // ---------------------------------------------------------------------------
#endif

  

}

#endif
