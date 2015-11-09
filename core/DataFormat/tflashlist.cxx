#include <algorithm>
#include <iostream>

#ifdef __FORLARLITE__
// ------------------------------------
// LARLITE

#include "tflashlist.h"

namespace larlite {

#else
// ------------------------------------
// LARSOFT

#include "FlashList.hh"

#ifdef __BUILD_ROOT_DICT__
ClassImp( subevent::FlashList )
#endif

namespace subevent {

#endif
// ------------------------------------

  FlashList::FlashList() {
    fFlashes.reserve(100);
  }
  FlashList::~FlashList() {
    fFlashes.clear();
  }

  int FlashList::add( Flash&& opflash ) {
    fFlashes.emplace_back( opflash );
    return fFlashes.size();
  }

  Flash& FlashList::get( int i ) {
    return fFlashes.at(i);
  }

  FlashListIter FlashList::begin() {
    return fFlashes.begin();
  }
  
  FlashListIter FlashList::end() {
    return fFlashes.end();
  }

  void FlashList::transferFlash( Flash& aflash ) {
    // it's mine now
    add( std::move(aflash) );
  }

  void FlashList::sortByTime() {
    std::sort( begin(), end(), FlashList::compareTime );
    sortMethod = kByTime;
  }

  void FlashList::sortByCharge() {
    std::sort( begin(), end(), FlashList::compareArea );
    sortMethod = kByCharge;
  }

  void FlashList::sortByAmp() {
    std::sort( begin(), end(), FlashList::compareAmp );
    sortMethod = kByAmp;
  }

}
