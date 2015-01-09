#ifndef SELECTIONTOOL_SPALGOLONELYE_CXX
#define SELECTIONTOOL_SPALGOLONELYE_CXX

#include "SPAlgoLonelyE.h"

namespace sptool {

  SPAlgoLonelyE::SPAlgoLonelyE() : SPAlgoBase()
  {
    _name     = "SPAlgoLonelyE";
    _debug    = false;
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
    // 1) shower does not point backwards to the end of a track
    // 2) shower does not start near another track
    // 3) shower does not point backwards to the trunk of another shower
    // 4) shower aligned with and after another shower
    
    double minDist = 1036.; // detector length

    geoalgo::HalfLine_t showerBackwards(shr.pos(),shr.mom()*(-1)); // line pointing backwards from shower start
    geoalgo::Vector_t shrDir(shr.mom()/shr.mom().Length());     // direction vector of track  (momentum but normalized)
    
    for (auto const& t : data._tracks) {
    
      geoalgo::LineSegment_t track(t[0],t.at(t.size()-1)); // segment pointing from track start to track end
      // Case 1)
      // loop over all track segments.
      // for each segment, check if the shower pointing back gets close to the end of that segment
      for (size_t i=1; i < t.size()-1; i++){
	double dist = _geoAlgo.SqDist(t[i],showerBackwards);
	if ( dist < minDist ) { minDist = dist; }
      }
      // Case 2)
      // check how far the shower start point is
      // from the end of the track
      double distNext =shr.pos().Dist(track.End());
      if (distNext < minDist) { minDist = distNext; }
    }

    for (auto const& s : data._showers) {

      // Case 4)
      // first make sure we are not looping over the same shower we are studying in this function
      if (s.Start() == shr.pos()) { continue; }
      //check that the two showers are within some angle of each other
      if ( (180/3.14)*acos(shrDir.Dot(s.Dir())) < 10 ){ // if within 10 degrees
	// check that the shower start is not "downstream" and too far away from the other shower's start point
	// do this by checking distance between shower start and other shower's forward half line segment
	double distShrs = _geoAlgo.SqDist(shr.pos(),s);
	if ( distShrs < minDist ) { minDist = distShrs; }
      }// if the showers are aligned
    } // for all showers

    if (_debug) { std::cout << "min Dist between shower and other stuff: " << minDist << std::endl; }

    return -minDist;
  }

  SPArticleSet SPAlgoLonelyE::Reconstruct(const SPAData &data)
  { 

    // What is in this event?
    if (_debug){
      std::cout << "There are " << data._showers.size() << " showers." << std::endl;
      std::cout << "There are " << data._tracks.size() << " tracks." << std::endl;
    }

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

    if (_debug) { std::cout << std::endl; }

    return lonely_es; 
  }

}

#endif
