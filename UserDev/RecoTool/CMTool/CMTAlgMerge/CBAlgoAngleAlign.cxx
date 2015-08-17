#ifndef RECOTOOL_CBALGOANGLEALIGN_CXX
#define RECOTOOL_CBALGOANGLEALIGN_CXX

#include "CBAlgoAngleAlign.h"

namespace cmtool {

  CBAlgoAngleAlign::CBAlgoAngleAlign() : CBoolAlgoBase() {

    //this just sets default values
    SetDebug(true);
    SetAngleCut(10.); // in degrees
    SetAllow180Ambig(false);
    SetMinNHits(30);


  } //end constructor

  bool CBAlgoAngleAlign::Bool(const ::cluster::cluster_params &cluster1,
                              const ::cluster::cluster_params &cluster2)
  {
    
    double angle1 = cluster1.angle_2d;
    double angle2 = cluster2.angle_2d;
    size_t hits1     = cluster1.hit_vector.size();
    size_t hits2     = cluster2.hit_vector.size();

    //if don't make hit cut return aflse
    if ( (hits1 < _MinNHits) or (hits2 < _MinNHits) )
      return false;

    if (_debug){
      std::cout << "Cluster1:" << std::endl;
      std::cout << "\tAngle: " << angle1 << std::endl;
      std::cout << "\t Start: (" << cluster1.start_point.w << ", " << cluster1.start_point.t << ")" << std::endl; 
      std::cout << "Cluster2:" << std::endl;
      std::cout << "\tAngle: " << angle2 << std::endl;
      std::cout << "\t Start: (" << cluster2.start_point.w << ", " << cluster2.start_point.t << ")" << std::endl; 
      std::cout << std::endl;

    }
  
    //for some reason angles are frequently -999.99.
    //if either angle is this, clearly the cluster 2d angle is not well defined
    //and this algorithm does not apply
    if(angle1 < -998 || angle2 < -998)
      return false;

    bool compatible = false;
    
    //if you don't care if clusters have been reconstructed backwards
    if(_allow_180_ambig)
      compatible = ( abs(angle1-angle2)     < _MaxAngleSep ||
                     abs(angle1-angle2-180) < _MaxAngleSep ||
                     abs(angle1-angle2+180) < _MaxAngleSep   );
    else
      compatible = ( abs(angle1-angle2) < _MaxAngleSep );
    
  
    if(_verbose) {
          if(compatible) std::cout<<"These two clusters are compatible in angle."<<std::endl;
    }
    
    return compatible;
    
  } // end Merge function 
  

}//end namespace cmtool
#endif
