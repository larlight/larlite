#ifndef RECOTOOL_CBALGOSTARTINCONE_CXX
#define RECOTOOL_CBALGOSTARTINCONE_CXX

#include "CBAlgoStartInCone.h"

namespace cmtool {

  CBAlgoStartInCone::CBAlgoStartInCone()
  {
    // Nothing to be done in the base class
    this->reconfigure();

    _wire_2_cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _time_2_cm = larutil::GeometryUtilities::GetME()->TimeToCm();

    SetMinHits(40);
    SetMinLen(10);
    SetDebug(false);
    SetAngleCompat(90.);
    SetLengthReach(1.0);

  }


  void CBAlgoStartInCone::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CBAlgoStartInCone::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			       const ::cluster::ClusterParamsAlg &cluster2)
  {

    //apply filter on hit number and length immediately
    if ( ( (cluster1.GetParams().length < _lenMin) and (cluster1.GetHitVector().size() < _NhitsMin) )
	 and ( (cluster2.GetParams().length < _lenMin) and (cluster2.GetHitVector().size() < _NhitsMin) ) )
      return false;

    //A cluster has an associated cone defined as the cone originating
    //at the start point of that cluster opening out in the direction
    //of the end point with an angle equal to the opening angle
    //If cluster A (B) has a start point within the volume defined
    //by the cone of cluster B (A), and if cluster B (A) is good enough
    // (enough hits, charge, length...) then the two are merged.

    double w_start1 = cluster1.GetParams().start_point.w;// * _wire_2_cm;
    double t_start1 = cluster1.GetParams().start_point.t;// * _time_2_cm;
    double angle1   = cluster1.GetParams().angle_2d;
    double opening1 = cluster1.GetParams().opening_angle * (180./3.14);
    double len1     = cluster1.GetParams().length;

    double w_start2 = cluster2.GetParams().start_point.w;// * _wire_2_cm;
    double t_start2 = cluster2.GetParams().start_point.t;// * _time_2_cm;
    double angle2   = cluster2.GetParams().angle_2d;
    double opening2 = cluster2.GetParams().opening_angle * (180./3.14);
    double len2     = cluster2.GetParams().length;

    //check for angle compatibility: the direction of the two clusters must be within
    //this range of each other
    if ( abs(angle2-angle1) > _angleCompat ){
      if (_debug) { std::cout << "Directions too different....do not merge" << std::endl; }
      return false;
    }
    
    if (_debug){
      std::cout << "Cluster 1:" << std::endl;
      std::cout << "\tStart: ( " << w_start1 << ", " << t_start1 << " )" << std::endl;
      std::cout << "\tAngle: " << angle1 << std::endl;
      std::cout << "\tOpening Angle: " << opening1 << std::endl;
      std::cout << "\tLength: " << len1 << std::endl;
      std::cout << "Cluster 2:" << std::endl;
      std::cout << "\tStart: ( " << w_start2 << ", " << t_start2 << " )" << std::endl;
      std::cout << "\tAngle: " << angle2 << std::endl;
      std::cout << "\tOpening Angle: " << opening2 << std::endl;
      std::cout << "\tLength: " << len2 << std::endl;
      std::cout << std::endl;
    }
    
    //find if start point of A (B) in cone of B (A)
    //do this by translating point A (B) such that
    //start point of B (A) is at origin and +  axis
    //towards end point of B (A)
    double w_start1_transl = w_start1 - w_start2;
    double t_start1_transl = t_start1 - t_start2;
    double w_start2_transl = w_start2 - w_start1;
    double t_start2_transl = t_start2 - t_start1;

    double w_start1_rot =   w_start1_transl*cos(angle2*3.14/180.) + t_start1_transl*sin(angle2*3.14/180.);
    double t_start1_rot = - w_start1_transl*sin(angle2*3.14/180.) + t_start1_transl*cos(angle2*3.14/180.);
    double w_start2_rot =   w_start2_transl*cos(angle1*3.14/180.) + t_start2_transl*sin(angle1*3.14/180.);
    double t_start2_rot = - w_start2_transl*sin(angle1*3.14/180.) + t_start2_transl*cos(angle1*3.14/180.);

    if (_debug){
      std::cout << "\tStart Rot 1: ( " << w_start1_rot << ", " << t_start1_rot << " )" << std::endl;
      std::cout << "\tStart Rot 2: ( " << w_start2_rot << ", " << t_start2_rot << " )" << std::endl;
    }

    if ( (w_start1_rot < len2*_lengthReach ) and (w_start1_rot > 0) and
	 ( abs(t_start1_rot) < (abs(w_start1_rot*sin(opening2*3.14/180.))) ) and
	 (len2 > _lenMin) and 
	 (cluster2.GetHitVector().size() > _NhitsMin) ){
      if (_verbose) { std::cout << "Start point of Cluster 1 in cone of Cluster 2!" << std::endl; }
      return true;
    }
    if ( (w_start2_rot < len1*_lengthReach ) and (w_start2_rot > 0) and
	 ( abs(t_start2_rot) < (abs(w_start2_rot*sin(opening1*3.14/180.))) ) and
	 (len1 > _lenMin) and 
	 (cluster1.GetHitVector().size() > _NhitsMin) ){
      if (_verbose) { std::cout << "Start point of Cluster 2 in cone of Cluster 1!" << std::endl; }
      return true;
    }

    return false;
  }


}


#endif
