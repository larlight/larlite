#ifndef ERTOOL_ERFILTERTOYCOSMICREMOVER_CXX
#define ERTOOL_ERFILTERTOYCOSMICREMOVER_CXX

#include "ERFilterToyCosmicRemover.h"

namespace ertool {

  ERFilterToyCosmicRemover::ERFilterToyCosmicRemover() : FilterBase()
						       , fTPC(6.,-110.,5.,250.,110.,1054.)
  {
    _name = "ERFilterToyCosmicRemover";
  }

  void ERFilterToyCosmicRemover::Filter(EventData& data)
  { 
    // Search for cosmic-like penetrating tracks
    for(auto const& trk : data.Track()) {

      if(trk->size()<2) data.FilterTrack(trk->ID());

      auto const& start = trk->front();
      auto const& end   = trk->back();

      if(!fTPC.Contain(start) || !fTPC.Contain(end)) {
	data.FilterTrack(trk->ID());
	_sec_filter.RegisterSeed(trk->ID());
      }
    }

    _sec_filter.Filter(data);

  }

}

#endif
