#ifndef RECOTOOL_CBALGOSTARTNEAREND_CXX
#define RECOTOOL_CBALGOSTARTNEAREND_CXX

#include "CBAlgoStartNearEnd.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoStartNearEnd::CBAlgoStartNearEnd() : CBoolAlgoBase()
  //----------------------------------------
  {
    SetMaxStartEndSeparation(10); //cm^2
    SetMinHits(40); //cm^2
    SetMaxAngle(20*(3.14/180)); //rad: max angle of end-point cluster
    // Nothing to be done in the base class
  }

  //--------------------------------------------------------
  bool CBAlgoStartNearEnd::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			const ::cluster::ClusterParamsAlg &cluster2)
  //--------------------------------------------------------
  {

    double start_w1 = cluster1.GetParams().start_point.w;
    double start_t1 = cluster1.GetParams().start_point.t;
    double end_w1 = cluster1.GetParams().end_point.w;
    double end_t1 = cluster1.GetParams().end_point.t;

    double start_w2 = cluster2.GetParams().start_point.w;
    double start_t2 = cluster2.GetParams().start_point.t;
    double end_w2 = cluster2.GetParams().end_point.w;
    double end_t2 = cluster2.GetParams().end_point.t;

    double angle_1 = cluster1.GetParams().opening_angle;
    double angle_2 = cluster2.GetParams().opening_angle;

    size_t hits_1 = cluster1.GetHitVector().size();
    size_t hits_2 = cluster2.GetHitVector().size();


    if ( (angle_1 < _maxopeningangle) and (hits_1 > _MinHits) and
	 ( ((start_w2-end_w1)*(start_w2-end_w1) +
	    (start_t2-end_t1)*(start_t2-end_t1)) < _separation) ){
      if (_verbose)
	std::cout << "Start in End!" << std::endl;
      return true;
    }

    if ( (angle_2 < _maxopeningangle) and (hits_2 > _MinHits) and
	 ( ((start_w1-end_w2)*(start_w1-end_w2) +
	    (start_t1-end_t2)*(start_t1-end_t2)) < _separation) ){
      if (_verbose)
	std::cout << "Start in End!" << std::endl;
      return true;
    }

    return false;

  }

  //-----------------------
  void CBAlgoStartNearEnd::Report()
  //-----------------------
  {
  }

}

#endif
