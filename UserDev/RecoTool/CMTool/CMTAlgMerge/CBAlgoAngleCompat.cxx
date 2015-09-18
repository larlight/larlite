#ifndef RECOTOOL_CBALGOANGLECOMPAT_CXX
#define RECOTOOL_CBALGOANGLECOMPAT_CXX

#include "CBAlgoAngleCompat.h"

namespace cmtool {

  CBAlgoAngleCompat::CBAlgoAngleCompat() : CBoolAlgoBase() {

    //this just sets default values

    SetDebug(true);

    SetAngleCut(30.); // in degrees

    SetMinHits(0);
    
    SetAllow180Ambig(false);

    SetUseOpeningAngle(false);

    //    angle_dist_histo = 0;

    //    angle_dist_histo = new TH1F("angle_dist_histo","Cluster Angle Differences",100,-360,360);

  } //end constructor

  bool CBAlgoAngleCompat::Bool(const ::cluster::cluster_params &cluster1,
                               const ::cluster::cluster_params &cluster2)
  {
    
    //if number of hits not large enough skip
    if ( _minHits and ((cluster1.hit_vector.size() < _minHits) or (cluster2.hit_vector.size() < _minHits)) ) {
      return false;
    }

    //pretty sure we don't need conversion factors here. 
    //already in cm/cm units, degrees? need to check that
    double angle1 = cluster1.angle_2d;// * _time_2_cm / _wire_2_cm;
    double angle2 = cluster2.angle_2d;// * _time_2_cm / _wire_2_cm;

    double w_start1 = cluster1.start_point.w;// * _wire_2_cm;
    double t_start1 = cluster1.start_point.t;// * _time_2_cm;
    double w_start2 = cluster2.start_point.w;// * _wire_2_cm;
    double t_start2 = cluster2.start_point.t;// * _time_2_cm;

    if (_debug){

      std::cout << "Cluster 1:" << std::endl;
      std::cout << "\tAngle: " << angle1 << std::endl;
      std::cout << "\tStart: ( " << w_start1 << ", " << t_start1 << " )" << std::endl;
      std::cout << "Cluster 2:" << std::endl;
      std::cout << "\tAngle: " << angle2 << std::endl;
      std::cout << "\tStart: ( " << w_start2 << ", " << t_start2 << " )" << std::endl;

    }
  
    //for some reason angles are frequently -999.99.
    //if either angle is this, clearly the cluster 2d angle is not well defined
    //and this algorithm does not apply
    if(angle1 < -998 || angle2 < -998)
      return false;

    //    if(angle_dist_histo){
    //      angle_dist_histo->Fill(angle1-angle2);
    //    }
    //    else
    //      std::cout<<"\n\n\nSOMETHING WENT HORRIBLY WRONG IN CBALGOANGLECOMPAT\n\n\n\n\n\n\n"<<std::endl;

    bool compatible = false;
    
    double my_cut_value = _max_allowed_2D_angle_diff;
    //if using opening angle, have angle cutoff be the smaller of the two opening angles
    if(_use_opening_angle) 
      my_cut_value = std::min(cluster1.opening_angle, cluster2.opening_angle);
    
    //if you don't care if clusters have been reconstructed backwards
    if(_allow_180_ambig)
      compatible = ( abs(angle1-angle2)     < my_cut_value ||
                   abs(angle1-angle2-180) < my_cut_value ||
                   abs(angle1-angle2+180) < my_cut_value   );
    else
      compatible = ( abs(angle1-angle2)     < my_cut_value );
    
  
    if(_verbose) {
        if(compatible) std::cout<<"These two clusters are compatible in angle."<<std::endl;
        else std::cout<<"These two clusters are NOT compatible in angle."<<std::endl;
    }
    
    return compatible;
    
  } // end Merge function 
  

}//end namespace cmtool
#endif
