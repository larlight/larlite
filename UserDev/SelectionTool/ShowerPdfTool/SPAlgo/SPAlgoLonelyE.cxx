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


  void SPAlgoLonelyE::ProcessBegin()
  {

    _alg_singleE.ProcessBegin();
    
    return;
  }

  void SPAlgoLonelyE::LoadParams(std::string fname, size_t version){

    _alg_singleE.LoadParams(fname,version);
    return;
  }

  double SPAlgoLonelyE::LL(const SPArticle shr, const SPAData &data){

    // calculate score that this shower does not have anything nearby
    
    // do this by checking that:
    // 1) shower & other tracks+showers don't point back to the "same" point
    // 2) shower does not start near another shower+track
    
    double alone = 1.;

    for (auto const& t : data._tracks) {
    
      geoalgo::Point_t int1(3);
      geoalgo::Point_t int2(3);
      geoalgo::HalfLine_t track(t[0],t[0]-t[1]); // line pointing backwards from track start
      geoalgo::HalfLine_t shower(shr.pos(),shr.mom()*(-1)); // line pointing backwards from shower start
      double distBack = _geoAlgo.SqDist(shower,track,int1,int2);
      // int1 & int2 are the PoCA points on the shr & track lines respectively
      std::cout << "distance between track line & shower line is: " << distBack << std::endl;
      double distNext = _geoAlgo.SqDist(shr.pos(),track);
      std::cout << "distance between track and shower start: " << distNext << std::endl;
      
      
      return alone;
    }

    return -1.;
  }

  SPArticleSet SPAlgoLonelyE::Reconstruct(const SPAData &data)
  { 

    //Get a list of single (start point isolated) electron showers
    //from the SPAlgoSingleE instance
    SPArticleSet single_es = _alg_singleE.Reconstruct(data);

    // loop over showers
    for(auto const& s : single_es) {

      LL(s,data);

    }


    return SPArticleSet(); 
  }

}

#endif
