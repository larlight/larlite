#ifndef RECOTOOL_CBALGOCENTEROFMASSSMALL_CXX
#define RECOTOOL_CBALGOCENTEROFMASSSMALL_CXX

#include "CBAlgoCenterOfMassSmall.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoCenterOfMassSmall::CBAlgoCenterOfMassSmall() : CBoolAlgoBase()
  //----------------------------------------
  {

    SetDebug(false);
    SetMaxHitsSmallClus(30);
    SetMaxDistance(20.);
    SetMaxCOMDistance(25.);
    UseCOMInPoly(true);
    UseCOMClose(true);
    UseCOMNearClus(true);

  }

  //---------------------------------------------------------------------------
  bool CBAlgoCenterOfMassSmall::Bool(const ::cluster::ClusterParamsAlg &cluster1,
				     const ::cluster::ClusterParamsAlg &cluster2)
  //---------------------------------------------------------------------------
  {

    int Nhits1 = 0;
    int Nhits2 = 0;

    //Both clusters should have less hits than some threshold
    if ( (cluster1.GetHitVector().size() > _maxHits) or (cluster2.GetHitVector().size() > _maxHits) )
      return false;
    
    //Define COM values on w & t
    double COM_t_1  = 0;
    double COM_w_1  = 0;
    double Q_1      = 0;
    double start_w_1;
    double start_t_1;
    double end_w_1;
    double end_t_1;
    Polygon2D poly1;
    double COM_t_2  = 0;
    double COM_w_2  = 0;
    double Q_2      = 0;
    double start_w_2;
    double start_t_2;
    double end_w_2;
    double end_t_2;
    Polygon2D poly2;

    //Get Hit vector for cluster 1
    std::vector<larutil::PxHit> hits1;
    hits1   = cluster1.GetHitVector();
    Nhits1  = hits1.size();
    poly1   = cluster1.GetParams().PolyObject;
    start_w_1 = cluster1.GetParams().start_point.w;
    start_t_1 = cluster1.GetParams().start_point.t;
    end_w_1   = cluster1.GetParams().end_point.w;
    end_t_1   = cluster1.GetParams().end_point.t;
    //Get Hit vector for cluster 2
    std::vector<larutil::PxHit> hits2;
    hits2   = cluster2.GetHitVector();
    Nhits2 = hits2.size();
    poly2   = cluster2.GetParams().PolyObject;
    start_w_2 = cluster2.GetParams().start_point.w;
    start_t_2 = cluster2.GetParams().start_point.t;
    end_w_2   = cluster2.GetParams().end_point.w;
    end_t_2   = cluster2.GetParams().end_point.t;

    //Find COM for cluster 1
    for (auto& hit: hits1){
      COM_t_1 += hit.t * hit.charge;
      COM_w_1 += hit.w * hit.charge;
      Q_1     += hit.charge;
    }
    COM_t_1 /= Q_1;
    COM_w_1 /= Q_1;

    //Find COM for cluster 2
    for (auto& hit: hits2){
      COM_t_2 += hit.t * hit.charge;
      COM_w_2 += hit.w * hit.charge;
      Q_2     += hit.charge;
    }
    COM_t_2 /= Q_2;
    COM_w_2 /= Q_2;

    if (_debug) {
      std::cout << "Cluster 1: " << std::endl;
      std::cout << "N Hits: " << Nhits1 << std::endl;
      std::cout << "COM: (w,t) -> (" << COM_w_1 << ", " << COM_t_1 << ")" << std::endl;
      std::cout << "Cluster 2: " << std::endl;
      std::cout << "N Hits: " << Nhits2 << std::endl;
      std::cout << "COM: (w,t) -> (" << COM_w_2 << ", " << COM_t_2 << ")" << std::endl;
      std::cout << std::endl;
    }

    //Get COM
    std::pair<float,float> COM_1;
    COM_1 = std::make_pair( COM_w_1, COM_t_1 );
    std::pair<float,float> COM_2;
    COM_2 = std::make_pair( COM_w_2, COM_t_2 );

    //look for polygon overlap
    if ( ( ( poly2.PointInside(COM_1) ) and  _COMinPolyAlg ) or
	 ( ( poly1.PointInside(COM_2) ) and  _COMinPolyAlg ) ){
      if (_verbose) { std::cout << "Polygon Overlap -> Merge!" << std::endl << std::endl;}
      return true;
    }

    //look for COM of 1 close to COM of 2
    double distCOMs = ( COM_w_1-COM_w_2 )*( COM_w_1-COM_w_2 ) +
      ( COM_t_1-COM_t_2 )*( COM_t_1-COM_t_2 ); 
    if ( _COMsClose and ( distCOMs < _MaxCOMDistSquared ) ){
      if (_verbose) { std::cout << "COMs close to each other -> Merge!" << std::endl << std::endl;}
      return true;
    }

    //look for COM close to start-end of other cluster
    if ( _COMNearClus and
	 ( ( ShortestDistanceSquared( COM_w_1, COM_t_1, start_w_2, start_t_2, end_w_2, end_t_2 ) < _MaxDist ) or
	   ( ShortestDistanceSquared( COM_w_2, COM_t_2, start_w_1, start_t_1, end_w_1, end_t_1 ) < _MaxDist ) ) ) {
      if (_verbose) { std::cout << "COM close to start-end -> Merge!" << std::endl; }
      return true;
    }

    return false;
  }

  //-----------------------
  void CBAlgoCenterOfMassSmall::Report()
  //-----------------------
  {

  }

  double CBAlgoCenterOfMassSmall::ShortestDistanceSquared(double point_x, double point_y, 
							  double start_x, double start_y,
							  double end_x,   double end_y  ) const {
    
    //This code finds the shortest distance between a point and a line segment.    
    //code based off sample from 
    //http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
    //note to self: rewrite this with TVector2 and compare time differences... 
    //TVector2 code might be more understandable
    
    double distance_squared = -1;
    
    // Line segment: from ("V") = (start_x, start_y) to ("W")=(end_x, end_y)
    double length_squared = pow((end_x - start_x), 2) + pow((end_y - start_y), 2);
    
    // Treat the case start & end point overlaps
    if(length_squared < 0.1) {
      if(_verbose){
	std::cout << std::endl;
	std::cout << Form(" Provided very short line segment: (%g,%g) => (%g,%g)",
			  start_x,start_y,end_x,end_y) << std::endl;
	std::cout << " Likely this means one of two clusters have start & end point identical." << std::endl;
	std::cout << " Check the cluster output!" << std::endl;
	std::cout << std::endl;
	std::cout << Form(" At this time, the algorithm uses a point (%g,%g)",start_x,start_y) << std::endl;
	std::cout << " to represent this cluster's location." << std::endl;
	std::cout << std::endl;
      }
      
      return (pow((point_x - start_x),2) + pow((point_y - start_y),2));
    }
    
    //Find shortest distance between point ("P")=(point_x,point_y) to this line segment
    double t = ( (point_x - start_x)*(end_x - start_x) + (point_y - start_y)*(end_y - start_y) ) / length_squared;
    
    if(t<0.0) distance_squared = pow((point_x - start_x), 2) + pow((point_y - start_y), 2);
    
    else if (t>1.0) distance_squared = pow((point_x - end_x), 2) + pow(point_y - end_y, 2);
    
    else distance_squared = pow((point_x - (start_x + t*(end_x - start_x))), 2) + pow((point_y - (start_y + t*(end_y - start_y))),2);
    
    return distance_squared;
    
  }//end ShortestDistanceSquared function



  
}

#endif
