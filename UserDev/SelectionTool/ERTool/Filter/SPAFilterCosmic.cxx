#ifndef SELECTIONTOOL_SPAFILTERCOSMIC_CXX
#define SELECTIONTOOL_SPAFILTERCOSMIC_CXX

#include "SPAFilterCosmic.h"

namespace sptool {

  SPAFilterCosmic::SPAFilterCosmic() : SPAFilterBase()
  {
    _name = "SPAFilterCosmic";
	SetCosmicScoreCut(0.); 
  }

  SPAOrder SPAFilterCosmic::Select(const SPAData &data)
  { 
	
    //auto track_tag    = storage->get_data<event_cosmictag>("mctrackmctag");
    //auto shower_tag   = storage->get_data<event_cosmictag>("mcshowermctag");
	
	SPAOrder rm ;

	for(size_t s=0; s<data._showers.size(); ++s){
		if(data._showers.at(s)._cosmogenic < _score )
			rm._rm_showers.insert(s) ;
		}
	
    //for(size_t i=0; i<data._tracks.size(); ++i) if(data._tracks[i]._cos
    return rm; 
  }

}

#endif
