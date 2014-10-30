#ifndef RECOTOOL_CBALGOSTARTINPOLY_CXX
#define RECOTOOL_CBALGOSTARTINPOLY_CXX

#include "CBAlgoStartInPoly.h"

namespace cmtool {

  CBAlgoStartInPoly::CBAlgoStartInPoly()
  {
    // Nothing to be done in the base class
    this->reconfigure();

    _wire_2_cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _time_2_cm = larutil::GeometryUtilities::GetME()->TimeToCm();

    SetDebug(true);
    SetMinHitsCut(40);

  }


  void CBAlgoStartInPoly::reconfigure(){

    //not sure what needs to be reset/reconfigured for this algo
    
  }//end reconfigure function

  
  bool CBAlgoStartInPoly::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			       const ::cluster::ClusterParamsAlg &cluster2)
  {

    //skip if both polygons do not have minimum number of hits
    if ( ( (cluster1.GetHitVector().size() > _MinHits) and
	   (cluster2.GetHitVector().size() > _MinHits) ) or
	 ( (cluster1.GetParams().PolyObject.Size() < 2) or
	   (cluster2.GetParams().PolyObject.Size() < 2) ) )
      return false;


    //If start point of one cluster is inside the other's polygon
    //boundary --> then merge!
    
    float w_start1 = cluster1.GetParams().start_point.w;// * _wire_2_cm;
    float t_start1 = cluster1.GetParams().start_point.t;// * _time_2_cm;

    float w_start2 = cluster2.GetParams().start_point.w;// * _wire_2_cm;
    float t_start2 = cluster2.GetParams().start_point.t;// * _time_2_cm;


    //make Point() for the two start points
    std::pair<float,float> start1;
    std::pair<float,float> start2;
    start1 = std::make_pair( w_start1, t_start1);
    start2 = std::make_pair( w_start2, t_start2);


    if (_debug){
      std::cout << "\tStart1: (" << w_start1 << ", " << t_start1 << " )" << std::endl;
      std::cout << "\tN Sides2:" << cluster2.GetParams().PolyObject.Size() << std::endl;
      for (unsigned int n=0; n < cluster2.GetParams().PolyObject.Size(); n++)
	std::cout << "\t\t\t(" << cluster2.GetParams().PolyObject.Point(n).first << ", "
		  << cluster2.GetParams().PolyObject.Point(n).first << " )" << std::endl;
      std::cout << "\tStart2: (" << w_start2 << ", " << t_start2 << " )" << std::endl;
      std::cout << "\tN Sides2:" << cluster1.GetParams().PolyObject.Size() << std::endl;
      for (unsigned int n=0; n < cluster1.GetParams().PolyObject.Size(); n++)
	std::cout << "\t\t\t(" << cluster1.GetParams().PolyObject.Point(n).first << ", "
		  << cluster1.GetParams().PolyObject.Point(n).first << " )" << std::endl;
    }
    
    
    //check if start point for cluster1 is in Polygon of cluster2
    if ( ( cluster2.GetParams().PolyObject.PointInside( start1) ) and (cluster1.GetHitVector().size() > _MinHits) ){
      if (_verbose) { 
	std::cout << "Start point of Cluster 1 in Polygon of Cluster 2!" << std::endl;
      }
      return true;
    }
    //check if start point for cluster1 is in Polygon of cluster2
    if ( ( cluster1.GetParams().PolyObject.PointInside( start2) ) and (cluster1.GetHitVector().size() > _MinHits) ){
      if (_verbose) { 
	std::cout << "Start point of Cluster 2 in Polygon of Cluster 1!" << std::endl; 
      }
      return true;
    }

    return false;
  }


}

#endif
