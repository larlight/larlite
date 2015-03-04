#ifndef ERTOOL_FILTERTRACKLENGTH_CXX
#define ERTOOL_FILTERTRACKLENGTH_CXX

#include "FilterTrackLength.h"

namespace ertool {

  FilterTrackLength::FilterTrackLength() : FilterBase()
  {
    _name = "FilterTrackLength";
    _lenCut = 0;
  }

  void FilterTrackLength::Filter(EventData& data) {
    
    // loop over tracks
    for (size_t t=0; t < data.AllTrack().size(); t++) {
      // if the track has less than 1 point -> remove!
      if ( (data.AllTrack()[t].size() < 2) or (data.AllTrack()[t].Length() < _lenCut) )
	data.FilterTrack(t);
    }	
   
    return;
  }

}

#endif
