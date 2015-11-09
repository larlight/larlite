#ifndef __FORLARLITE__

#include "SubEvent.hh"

#ifdef __BUILD_ROOT_DICT__
ClassImp( subevent::SubEvent )
#endif

namespace subevent {

#else

#include "subevent.h"

namespace larlite {

#endif

  SubEvent::SubEvent() {
    tstart_sample = -1;
    tend_sample = -1;
    totpe = 0.0;
    sumflash30 = sumfcomp_gausintegral = 0.0;
    maxamp = 0.0;
  }
  
  SubEvent::~SubEvent() {}

  void SubEvent::transferFlashes( FlashList& flashes ) {
    for ( FlashListIter it=flashes.begin(); it!=flashes.end(); it++ ) {
      flashes.add( std::move( *it ) );
    }
  }
  
}
