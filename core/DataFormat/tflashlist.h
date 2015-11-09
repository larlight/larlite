#ifndef __FLASHLIST__
#define __FLASHLIST__

#ifndef __FORLARLITE__
// ---------------------------------------------
// LARSOFT
#ifdef __BUILD_ROOT_DICT__
#include "TObject.h"
#endif
#include <vector>

#include "Flash.hh"

namespace subevent {

  typedef std::vector< Flash >::iterator FlashListIter;

#ifdef __BUILD_ROOT_DICT__
  class FlashList : public TObject { 
#else
  class FlashList { 
#endif

// ---------------------------------------------
#else

#include "tflash.h"

namespace larlite {

  typedef std::vector< Flash >::iterator FlashListIter;

  class FlashList {

#endif

  public:
    FlashList();
    ~FlashList();
    
#ifndef __CINT__ // hide from rootcint
#ifndef __GCCXML__
    int add( Flash&& opflash );
#endif
#endif
    Flash& get( int i );
    FlashListIter begin();
    FlashListIter end();
    void sortByTime();
    void sortByCharge();
    void sortByAmp();
    int size() { return fFlashes.size(); };
    void clear() { fFlashes.clear(); fFlashes.reserve(20); };
    bool sortedByTime() { if (sortMethod==kByTime) return true; else return false; }; 
    bool sortedByCharge() { if (sortMethod==kByCharge) return true; else return false; }; 
    bool sortedByAmp() { if (sortMethod==kByAmp) return true; else return false; }; 
    void transferFlash( Flash& flash ); // wrapper around add for cython

  protected:
    std::vector< Flash > fFlashes;

    typedef enum { kUnsorted=-1, kByTime, kByCharge, kByAmp } SortMethod_t;
    SortMethod_t sortMethod;

    static bool compareTime( Flash& t1, Flash& t2 ) {
      if (t1.tstart<t2.tstart )
	return true;
      else
	return false;
    };
    static bool compareArea( Flash& q1, Flash& q2 ) {
      if ( q1.area<q2.area ) return true;
      else return false;
    };
    static bool compareAmp( Flash& amp1, Flash& amp2 ) {
      if ( amp1.maxamp<amp2.maxamp ) return true;
      else return false;
    };

    
  public:
#ifdef __BUILD_ROOT_DICT__
    ClassDef( FlashList, 1 )
#endif
  };

}

#endif
