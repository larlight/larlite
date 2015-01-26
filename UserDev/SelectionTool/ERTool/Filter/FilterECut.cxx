#ifndef ERTOOL_FILTERECUT_CXX
#define ERTOOL_FILTERECUT_CXX

#include "FilterECut.h"

namespace ertool {

  FilterECut::FilterECut() : FilterBase()
  {
    _name = "FilterECut";
    SetECut(0.);
  }

  void FilterECut::Filter(EventData& data) {
    
    // loop over showers
    for (size_t s=0; s < data.AllShower().size(); ++s) {
      if (data.AllShower()[s]._energy < _ECut) 
	data.FilterShower(s);
    }
    
    // loop over tracks
    for (size_t t=0; t < data.AllTrack().size(); t++) {
      if (data.AllTrack()[t]._energy < _ECut)
	data.FilterTrack(t);
    }	
   
    return;
  }

}

#endif
