#ifndef ERTOOL_FILTERCOSMIC_CXX
#define ERTOOL_FILTERCOSMIC_CXX

#include "FilterCosmic.h"

namespace ertool {

  FilterCosmic::FilterCosmic() : FilterBase()
  {
    _name = "FilterCosmic";
    SetCosmicScoreCut(0.); 
  }

  void FilterCosmic::Filter(EventData& data)
  {

    for(size_t i=0; i<data.AllShower().size(); ++i) {

      auto const& obj = data.AllShower()[i];
      if(obj._cosmogenic < _score) data.FilterShower(i);

    }

    for(size_t i=0; i<data.AllTrack().size(); ++i) {

      auto const& obj = data.AllTrack()[i];
      if(obj._cosmogenic < _score) data.FilterTrack(i);

    }

  }

}

#endif
