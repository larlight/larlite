#ifndef RECOTOOL_CBALGOOUTOFCONESEPARATE_CXX
#define RECOTOOL_CBALGOOUTOFCONESEPARATE_CXX

#include "CBAlgoOutOfConeSeparate.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoOutOfConeSeparate::CBAlgoOutOfConeSeparate() : CBoolAlgoBase()
  //----------------------------------------
  {

    SetDebug(false);
    SetMaxAngleSep(20.);
    SetMinLength(15.);
    SetMinHits(20);
    SetStartAngleFalloff(2800); // in cm^2

  }

  //--------------------------------------------------------
  bool CBAlgoOutOfConeSeparate::Bool(const ::cluster::cluster_params &cluster1,
                                     const ::cluster::cluster_params &cluster2)
  //--------------------------------------------------------
  {

    double angle1 = cluster1.angle_2d;
    double angle2 = cluster2.angle_2d;

    double w_start1 = cluster1.start_point.w;
    double t_start1 = cluster1.start_point.t;
    double w_start2 = cluster2.start_point.w;
    double t_start2 = cluster2.start_point.t;

    double len1 = cluster1.length;
    double len2 = cluster2.length;
    
    size_t hits1 = cluster1.hit_vector.size();
    size_t hits2 = cluster2.hit_vector.size();

    double startseparation = (w_start2-w_start1)*(w_start2-w_start1) + (t_start2-t_start1)*(t_start2-t_start1);
    //convert sepration to be instead of just angle -> angle/distance^n (n=1 for now)
    SetMaxAngleFar(_MaxAngle*(_FallOff/startseparation)); //distance^2 of 400 cm^2 taken as "standard" 
    if ( _MaxAngleFar > 90. )
      _MaxAngleFar = 90.;

    //if either cluster has less than _minHits don't even try...
    if ( (hits1 < _minHits) or (hits2 < _minHits)
         or (angle1 < -360) or (angle2 < -360) )
      return false;
    
    if (_debug){
      std::cout << "Cluster 1:" << std::endl;
      std::cout << "\tStart: ( " << w_start1 << ", " << t_start1 << " )" << std::endl;
      std::cout << "\tAngle: " << angle1 << std::endl;
      std::cout << "\tLength: " << len1 << std::endl;
      std::cout << "\tN Hits: " << hits1 << std::endl;

      std::cout << "Cluster 2:" << std::endl;
      std::cout << "\tStart: ( " << w_start2 << ", " << t_start2 << " )" << std::endl;
      std::cout << "\tAngle: " << angle2 << std::endl;
      std::cout << "\tLength: " << len2 << std::endl;
      std::cout << "\tN Hits: " << hits2 << std::endl;

      std::cout << "Start Point Separation: " << startseparation << std::endl;
    }

    //cluster 1 needs to be long enough (i.e. good) and cluster 2 must have minimum number of hits
    double angle;
    double separation;

    if ( startseparation == 0 ) //do not prohibit merging if start point identical
      return false;

    //first calculate angle of line in 2D plane connectng the two start points
    if ( (t_start2-t_start1) == 0 )
      angle = 0.;
    else {
      double slope = (t_start2-t_start1)/(w_start2-w_start1);
      angle = atan(slope)*180./3.14;
    }

    separation = abs(angle-angle1);

    if (_debug){
      std::cout << "Angle S1--S2: " << angle << std::endl;
      std::cout << "Angle1--S2: " << separation << std::endl;
    }
    if ( ( ( ( (separation > _MaxAngle) and (separation < 180-_MaxAngle) ) or
             ( (separation > 180+_MaxAngle) and (separation< 360-_MaxAngle) ) )
           or ( ( (separation > _MaxAngleFar) and (separation < 180-_MaxAngleFar) ) or
              ( (separation > 180+_MaxAngleFar) and (separation< 360-_MaxAngleFar) ) ) )
         and (hits2 > _minHits)
         and (len1 > _MinLen)      ){
      if (_verbose) { std::cout << "Separate! cluster 1 BIG" << std::endl << std::endl; }
      return true;
    }

    //now change direction of angle so that it points from cluster 2 (big) to cluster 1 (small)
    angle += 180.;
    angle = (int)(angle) % 360;

    separation = abs(angle-angle2);
    //separation *= (400./startseparation); //distance^2 of 400 cm^2 taken as "standard"
    if (_debug){
      std::cout << "Angle S2--S1: " << angle << std::endl;
      std::cout << "Angle2--S1: " << separation << std::endl;
    }
    if ( ( ( ( (separation > _MaxAngle) and (separation < 180-_MaxAngle) ) or
             ( (separation > 180+_MaxAngle) and (separation< 360-_MaxAngle) ) )
           or ( ( (separation > _MaxAngleFar) and (separation < 180-_MaxAngleFar) ) or
              ( (separation > 180+_MaxAngleFar) and (separation< 360-_MaxAngleFar) ) ) )
         and (hits1 > _minHits)
         and (len2 > _MinLen)    ){
      if (_verbose) { std::cout << "Separate! cluster 2 BIG" << std::endl << std::endl; }
      return true;
    }

    return false;
  }

}

#endif
