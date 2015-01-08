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

    // select showers that do not originate from another track/shower
    
    // do this by checking that:
    // 1) shower does not point backwards to the end of a track/shower
    // 2) shower does not start near another track
    
    double minDist = 1036.; // detector length

    for (auto const& t : data._tracks) {
    
      geoalgo::LineSegment_t track(t[0],t.at(t.size()-1)); // segment pointing from track start to track end
      geoalgo::Vector_t trackDir(t.at(t.size()-1)-t[0]);   // direction vector of track (normalized)
      geoalgo::Point_t trackEnd(t.at(t.size()-1));         // track end point
      geoalgo::HalfLine_t shower(shr.pos(),shr.mom()*(-1)); // line pointing backwards from shower start
      geoalgo::Vector_t shrDir(shr.mom()/shr.mom().Length());     // direction vector of track  (momentum but normalized)
      // Case 1)
      // shower & track/shower direction must agree to some extent and
      // distance between shower start and shr/trk end should not be large
      if ((180/3.14)*acos(trackDir.Dot(shrDir)) < 5.){
	// shower and track directions are within 5 degrees
	double d = trackEnd.Dist(shr.pos());
	if ( d < minDist ) { minDist = d; }
      }
      // Case 2)
      double distNext = _geoAlgo.SqDist(shr.pos(),track);
      if (distNext < minDist) { minDist = distNext; }
    }

    std::cout << "min Dist between shower and other stuff: " << minDist << std::endl;

    return -minDist;
  }

  SPArticleSet SPAlgoLonelyE::Reconstruct(const SPAData &data)
  { 

    //Get a list of single (start point isolated) electron showers
    //from the SPAlgoSingleE instance
    SPArticleSet single_es = _alg_singleE.Reconstruct(data);
    
    // create empty SPArticleSet for the products
    SPArticleSet lonely_es;
    
    // loop over showers
    for(auto const& s : single_es) {

      if (LL(s,data) > 10){ // if more than 10 cm from anything else
	lonely_es.push_back(s);
      }

    }// for all input showers from SPAlgoSingleE
    return lonely_es; 
  }

}

#endif
