#ifndef RECOTOOL_CBALGOTRACKSEPARATE_CXX
#define RECOTOOL_CBALGOTRACKSEPARATE_CXX

#include "CBAlgoTrackSeparate.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoTrackSeparate::CBAlgoTrackSeparate() : CBoolAlgoBase()
  //----------------------------------------
  {

    //this just sets default values    
    SetVerbose(true);
    SetDebug(true);
    
    //1e9 is huge; everything will be merged
    SetMinNumHits(30);
    SetMinAngleDiff(15.); //in degrees
    SetMaxOpeningAngle(12.0); //in deg (parameter in rad!!)
    SetMinLength(10.);
    SetMinPolyHitDensity(10.);
    SetMaxWidth(10.);
    

    //NOTE! Using this flag means all of the other crap
    //(minNumHits, anglediff, openingangle, blah blah)
    //is totally irrelevant. if we stick with this flag as the algo,
    //we probably want to delete all of the old method
    SetUseEP(true);
    SetEPCutoff(0.99000);

    _wire_2_cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _time_2_cm = larutil::GeometryUtilities::GetME()->TimeToCm();

  }

  //--------------------------------------------------------
  bool CBAlgoTrackSeparate::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			const ::cluster::ClusterParamsAlg &cluster2)
  //--------------------------------------------------------
  {
    //if you are using EP method for this algo:
    if(_use_EP){
      if(cluster1.GetParams().eigenvalue_principal > _ep_cut &&
	 cluster2.GetParams().eigenvalue_principal > _ep_cut)
	return true;
    }
    //if you are using the original method for this algo:
    else{
      
      //two clusters are considered un-mergable if:
      //1) both have more than _MinNumHits
      //2) opening angle for both < _MAxOpeningAngle
      //3) diff. in direction of both < _MinAngleDiff
      
      size_t N_Hits1 = cluster1.GetHitVector().size();
      size_t N_Hits2 = cluster2.GetHitVector().size();
      larutil::PxPoint start_point1 = cluster1.GetParams().start_point;
      larutil::PxPoint start_point2 = cluster2.GetParams().start_point;
      double angle_2d1 = cluster1.GetParams().angle_2d;
      double angle_2d2 = cluster2.GetParams().angle_2d;
      double opening_angle1 = cluster1.GetParams().opening_angle;
      double opening_angle2 = cluster2.GetParams().opening_angle;
      Polygon2D PolyObject1 = cluster1.GetParams().PolyObject;
      Polygon2D PolyObject2 = cluster2.GetParams().PolyObject;
      double length1 = cluster1.GetParams().length;
      double length2 = cluster2.GetParams().length;
      double width1 = cluster1.GetParams().width;
      double width2 = cluster2.GetParams().width;
      
      //first filter out low hits clusters
      if ( (N_Hits1 > _MinNumHits) and
	   (N_Hits2 > _MinNumHits) ) {
	if (_debug) {
	  std::cout << "Cluster1 Num Hits: " << N_Hits1 << std::endl;
	  std::cout << "\t Start: (" << start_point1.w << " " << start_point1.t << " )" << std::endl;
	  std::cout << "\t Opening ANgle " << opening_angle1*(360/(2*3.14)) << std::endl;
	  std::cout << "\t Angle2D: " << angle_2d1 << std::endl;
	  std::cout << "\t Length: " << length1 << std::endl;
	  std::cout << "\t Width: " << width1 << std::endl;
	  std::cout << "Cluster2 Num Hits: " << N_Hits2 << std::endl;
	  std::cout << "\t Start: (" << start_point2.w  << " " << start_point2.t << " )" << std::endl;
	  std::cout << "\t Opening ANgle " << opening_angle2*(360/(2*3.14)) << std::endl;
	  std::cout << "\t Angle2D: " << angle_2d2 << std::endl;
	  std::cout << "\t Length: " << length2 << std::endl;
	  std::cout << "\t Width: " << width2 << std::endl;
	}
	if ( (N_Hits1 > _MinNumHits) and
	     (N_Hits2 > _MinNumHits) and
	     ( abs(angle_2d1 - angle_2d2) > _MinAngleDiff ) and
	     //( PolyObject1.Area()/N_Hits1 > _MinDensity ) and
	     //( PolyObject2.Area()/N_Hits2 > _MinDensity ) and
	     (opening_angle1 < _MaxOpeningAngle/(360/(2*3.14))) and
	     (opening_angle2 < _MaxOpeningAngle/(360/(2*3.14))) and
	     (width1 < _MaxWidth) and
	     (width2 < _MaxWidth) and
	     (length1 > _MinLength) and
	     (length2 > _MinLength) ){
	  if (_verbose) { std::cout << "*****************************************Separate with TrackSeparate!" << std::endl; }
	  return true;
	}
      }
    }
    
    return false;

  }

  //-----------------------
  void CBAlgoTrackSeparate::Report()
  //-----------------------
  {
  }

}

#endif
