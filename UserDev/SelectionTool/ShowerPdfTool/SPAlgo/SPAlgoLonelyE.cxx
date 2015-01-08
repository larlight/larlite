#ifndef SELECTIONTOOL_SPALGOLONELYE_CXX
#define SELECTIONTOOL_SPALGOLONELYE_CXX

#include "SPAlgoLonelyE.h"

namespace sptool {

  SPAlgoLonelyE::SPAlgoLonelyE() : SPAlgoBase()
  {
    _name     = "SPAlgoLonelyE";
  }

  void SPAlgoLonelyE::Reset()
  {}

  double SPAlgoLonelyE::LL(const geoalgo::HalfLine_t shr, const SPAData &data){

    // calculate score that this shower does not have anything nearby
    
    // do this by checking that:
    // 1) shower & other tracks+showers don't point back to the "same" point
    // 2) shower does not start near another shower+track
    
    double alone = 1.;

    for (auto const& t : data._tracks) {
    
      geoalgo::Point_t int1(3);
      geoalgo::Point_t int2(3);
      geoalgo::HalfLine_t track(t[0],t[1]-t[0]);
      //double distBack = _geoAlgo.SqDist(shr,track);
      
      return alone;
    }

    return -1.;
  }

  SPArticleSet SPAlgoLonelyE::Reconstruct(const SPAData &data)
  { 

    // loop over showers
    for(auto const& s : data._showers) {

      LL(s,data);

    }


    return SPArticleSet(); 
  }

}

#endif
