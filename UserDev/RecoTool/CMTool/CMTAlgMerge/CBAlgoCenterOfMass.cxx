#ifndef RECOTOOL_CBALGOCENTEROFMASS_CXX
#define RECOTOOL_CBALGOCENTEROFMASS_CXX

#include "CBAlgoCenterOfMass.h"

namespace cmtool {

  //----------------------------------------
  CBAlgoCenterOfMass::CBAlgoCenterOfMass() : CBoolAlgoBase()
  //----------------------------------------
  {

    SetDebug(false);
    SetMaxHitsSmallClus(10);
    SetMinHitsBigClus(40);
    SetMaxDistance(20.);
    SetLengthReach(3.0);
    UseCOMInPoly(true);
    UseCOMInCone(true);
    UseCOMNearClus(true);

  }

  //---------------------------------------------------------------------------
  bool CBAlgoCenterOfMass::Bool(const ::cluster::ClusterParamsAlg &cluster1,
				const ::cluster::ClusterParamsAlg &cluster2)
  //---------------------------------------------------------------------------
  {

    int small = 0;
    int hitssmall = 0;

    //determine if clusters ok and if so which is big and which is small
    if ( (cluster1.GetHitVector().size() > _minHits) and (cluster2.GetHitVector().size() < _maxHits) )
      small = 2;
    else if ( (cluster2.GetHitVector().size() > _minHits) and (cluster1.GetHitVector().size() < _maxHits) )
      small = 1;
    else
      return false;
    
    //Define COM values on w & t
    double COM_t_s = 0;
    double COM_w_s = 0;
    double Q_s     = 0;
    //Define cone parameters
    double opening_angle;
    double direc_angle;
    double length;
    double start_w;
    double start_t;
    double end_w;
    double end_t;
    Polygon2D poly;

    //Get Hit vector for small cluster
    std::vector<larutil::PxHit> hitss;
    if ( small == 1 ){
      hitss = cluster1.GetHitVector();
      hitssmall = hitss.size();
      direc_angle   = cluster2.GetParams().angle_2d;
      opening_angle = cluster2.GetParams().opening_angle * (180./3.14);
      length  = cluster2.GetParams().length;
      start_w = cluster2.GetParams().start_point.w;
      start_t = cluster2.GetParams().start_point.t;
      end_w = cluster2.GetParams().end_point.w;
      end_t = cluster2.GetParams().end_point.t;
      poly  = cluster2.GetParams().PolyObject;
    }      
    if ( small == 2 ){
      hitss = cluster2.GetHitVector();
      hitssmall = hitss.size();
      direc_angle   = cluster1.GetParams().angle_2d;
      opening_angle = cluster1.GetParams().opening_angle * (180./3.14);
      length  = cluster1.GetParams().length;
      start_w = cluster1.GetParams().start_point.w;
      start_t = cluster1.GetParams().start_point.t;
      end_w = cluster1.GetParams().end_point.w;
      end_t = cluster1.GetParams().end_point.t;
      poly  = cluster1.GetParams().PolyObject;
    }

    if (_debug){
      std::cout << "Big Cluster:" << std::endl;
      std::cout << "\tOpening Angle: " << opening_angle << std::endl;
      std::cout << "\tLength: " << length << std::endl;
      std::cout << "\tStart Point: (" << start_w << ", " << end_w << ")" << std::endl;
      std::cout << "\tDirection Angle: " << direc_angle << std::endl;
      std::cout << std::endl;
    }

    for (auto& hit: hitss){
      COM_t_s += hit.t * hit.charge;
      COM_w_s += hit.w * hit.charge;
      Q_s     += hit.charge;
    }
    COM_t_s /= Q_s;
    COM_w_s /= Q_s;

    if (_debug) {
      std::cout << "Small Cluster: " << std::endl;
      std::cout << "N Hits: " << hitssmall << std::endl;
      std::cout << "COM: (w,t) -> (" << COM_w_s << ", " << COM_t_s << ")" << std::endl;
      std::cout << std::endl;
    }

    //Get COM
    std::pair<float,float> COM_s;
    COM_s = std::make_pair( COM_w_s, COM_t_s );

    //Get rotate COM to see if in cone
    double COM_w_rot = (COM_w_s - start_w)*cos(direc_angle*3.14/180.) + (COM_t_s - start_t)*sin(direc_angle*3.14/180.);
    double COM_t_rot = - (COM_w_s - start_w)*sin(direc_angle*3.14/180.) + (COM_t_s - start_t)*cos(direc_angle*3.14/180.);

    //look for polygon overlap
    if ( ( poly.PointInside(COM_s) ) and  _COMinPolyAlg ){
      if (_verbose) { std::cout << "Polygon Overlap -> Merge!" << std::endl << std::endl;}
      return true;
    }
    //look for COM in cone
    if ( _COMinConeAlg and
	 (COM_w_rot < length*_lengthReach ) and ( COM_w_rot > 0 ) and
	 ( abs(COM_t_rot) < abs(COM_w_rot*sin(opening_angle*3.14/180.)) ) ){
      if (_verbose) { std::cout << "COM in Cone -> Merge! " << std::endl; }
      return true;
    }
    //look for COM close to start-end of other cluster
    if ( _COMNearClus and
	 ShortestDistanceSquared( COM_w_s, COM_t_s, start_w, start_t, end_w, end_t ) < _MaxDist ) {
      if (_verbose) { std::cout << "COM close to start-end -> Merge!" << std::endl; }
      return true;
    }

    return false;
  }

  //-----------------------
  void CBAlgoCenterOfMass::Report()
  //-----------------------
  {

  }

  double CBAlgoCenterOfMass::ShortestDistanceSquared(double point_x, double point_y, 
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
