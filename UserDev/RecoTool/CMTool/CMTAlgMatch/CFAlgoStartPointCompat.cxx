#ifndef RECOTOOL_CFALGOSTARTPOINTCOMPAT_CXX
#define RECOTOOL_CFALGOSTARTPOINTCOMPAT_CXX

#include "CFAlgoStartPointCompat.h"
#include "LArUtil/GeometryUtilities.h"
#include <algorithm>

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoStartPointCompat::CFAlgoStartPointCompat() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    SetVerbose(false);
  }

  //-----------------------------
  void CFAlgoStartPointCompat::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoStartPointCompat::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if(clusters.size()==2) return -1;
    // Code-block by Kazu ends

    //This algorithm now works for 3 planes: find 3Dstart point from first 2 planes and find
    //How well that agrees with 3rd plane's start point location.

    //So first, make sure clusters vector has only 3 elements. If not return -1
    if ( clusters.size() != 3 )
      return -1;

    //Find 3D start point from start point on first 2 planes:
    //For now convert start point wire in cm back to wire number
    //Round to integer (sometimes output is double...why???)
    double startWirecm0 = clusters.at(0)->start_point.w;
    double startTimecm0 = clusters.at(0)->start_point.t;
    int startWire0 = int( startWirecm0 / _w2cm );
    unsigned char Pl0 = clusters.at(0)->start_point.plane;
    //int startChan1 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl0, startWire0);
    double startWirecm1 = clusters.at(1)->start_point.w;
    double startTimecm1 = clusters.at(1)->start_point.t;
    int startWire1 = int( startWirecm1 / _w2cm );
    unsigned char Pl1 = clusters.at(1)->start_point.plane;
    //int startChan2 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl1, startWire1);
    double startWirecm2 = clusters.at(2)->start_point.w;
    double startTimecm2 = clusters.at(2)->start_point.t;
    int startWire2 = int( startWirecm2 / _w2cm );
    unsigned char Pl2 = clusters.at(2)->start_point.plane;
    //int startChan3 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl2, startWire2);


    //Get Intersections in pairs:
    //y and z indicate detector coordinate and numbers indicate planes
    //used to generate that intersection point
    double yS01, zS01, yS02, zS02, yS12, zS12;

    larutil::Geometry::GetME()->IntersectionPoint( startWire0, startWire1,
						   Pl0, Pl1,
						   yS01, zS01);

    larutil::Geometry::GetME()->IntersectionPoint( startWire0, startWire2,
						   Pl0, Pl2,
						   yS02, zS02);

    larutil::Geometry::GetME()->IntersectionPoint( startWire1, startWire2,
						   Pl1, Pl2,
						   yS12, zS12);

    //assume X coordinate for these start-points is 0
    //i.e. only focus on projection onto wire-plane
    //then check if the start point reconstructe from
    //planes A and B falls within the wire-range of
    //the cluster on plane C
    //For compatibility also, check that the start-point-time
    //is within the time-range of the cluster on plane C
    Double_t Start2[3] = {-100., yS01, zS01};
    Double_t Start1[3] = {-100., yS02, zS02};
    Double_t Start0[3] = {-100., yS12, zS12};
    UInt_t WireStart0 = 0;
    UInt_t WireStart1 = 0;
    UInt_t WireStart2 = 0;
    try { WireStart0 = larutil::Geometry::GetME()->NearestWire( Start0, Pl0); }
    catch ( ::larutil::LArUtilException &e) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      WireStart0 = 9999;
      }
    try { WireStart1 = larutil::Geometry::GetME()->NearestWire( Start1, Pl1); }
    catch ( ::larutil::LArUtilException &e ) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      WireStart0 = 9999;
      }
    try { WireStart2 = larutil::Geometry::GetME()->NearestWire( Start2, Pl2); }
    catch ( ::larutil::LArUtilException &e) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      WireStart0 = 9999;
      }

    //Now Get Hit-Range for Clusters
    std::vector<larutil::PxHit> hits0 = clusters.at(0)->hit_vector;
    std::vector<larutil::PxHit> hits1 = clusters.at(1)->hit_vector;
    std::vector<larutil::PxHit> hits2 = clusters.at(2)->hit_vector;
    //define variables for min/max time/wire of each cluster
    double minWire0 = 9999;
    double minWire1 = 9999;
    double minWire2 = 9999;
    double maxWire0 = 0;
    double maxWire1 = 0;
    double maxWire2 = 0;
    double minTime0 = 9999;
    double minTime1 = 9999;
    double minTime2 = 9999;
    double maxTime0 = 0;
    double maxTime1 = 0;
    double maxTime2 = 0;
    int hitWireTMP = 0;
    double hitTimeTMP = 0;

    for (auto& h: hits0){
      hitWireTMP = int(h.w/_w2cm);
      hitTimeTMP = int(h.t/_t2cm);
      if ( hitWireTMP < minWire0 ) { minWire0 = hitWireTMP; }
      if ( hitWireTMP > maxWire0 ) { maxWire0 = hitWireTMP; }
      if ( hitTimeTMP < minTime0 ) { minTime0 = hitTimeTMP; }
      if ( hitTimeTMP > maxTime0 ) { maxTime0 = hitTimeTMP; }
    }
    for (auto& h: hits1){
      hitWireTMP = int(h.w/_w2cm);
      hitTimeTMP = int(h.t/_t2cm);
      if ( hitWireTMP < minWire1 ) { minWire1 = hitWireTMP; }
      if ( hitWireTMP > maxWire1 ) { maxWire1 = hitWireTMP; }
      if ( hitTimeTMP < minTime1 ) { minTime1 = hitTimeTMP; }
      if ( hitTimeTMP > maxTime1 ) { maxTime1 = hitTimeTMP; }
    }
    for (auto& h: hits2){
      hitWireTMP = int(h.w/_w2cm);
      hitTimeTMP = int(h.t/_t2cm);
      if ( hitWireTMP < minWire2 ) { minWire2 = hitWireTMP; }
      if ( hitWireTMP > maxWire2 ) { maxWire2 = hitWireTMP; }
      if ( hitTimeTMP < minTime2 ) { minTime2 = hitTimeTMP; }
      if ( hitTimeTMP > maxTime2 ) { maxTime2 = hitTimeTMP; }
    }

    if ( _verbose ){
      std::cout << "Start W,T on plane 0: " << "[ " << startWirecm0 << ", " << startTimecm0 << " ]" << std::endl;
      std::cout << "Start Wire, planne 0: " << startWire0 << std::endl;
      std::cout << "Start W,T on plane 1: " << "[ " << startWirecm1 << ", " << startTimecm1 << " ]" << std::endl;
      std::cout << "Start Wire, planne 1: " << startWire1 << std::endl;
      std::cout << "Start W,T on plane 2: " << "[ " << startWirecm2 << ", " << startTimecm2 << " ]" << std::endl;
      std::cout << "Start Wire, planne 2: " << startWire2 << std::endl;
      std::cout << std::endl;
      std::cout << "Start Reco Point from clusters on planes 1 and 2 (Z,Y):" << std::endl
		<< "[ " << zS12 << ", " << yS12 << " ]" << std::endl;
      std::cout << "Nearest wire on Pl0 to reco-start point from Pl1, Pl2:" << std::endl
		<<  WireStart0 << std::endl;
      std::cout << "Min-Max Wire for Hits from Cluster on Plane 0:" << std::endl
		<< "[ " << minWire0 << ", " << maxWire0 << " ]" << std::endl;
      std::cout << "Start Reco Point from clusters on planes 0 and 2 (Z,Y):" << std::endl
		<< "[ " << zS02 << ", " << yS02 << " ]" << std::endl;
      std::cout << "Nearest wire on Pl1 to reco-start point from Pl0, Pl2:" << std::endl
		<<  WireStart1 << std::endl;
      std::cout << "Min-Max Wire for Hits from Cluster on Plane 1:" << std::endl
		<< "[ " << minWire1 << ", " << maxWire1 << " ]" << std::endl;
      std::cout << "Start Reco Point from clusters on planes 0 and 1 (Z,Y):" << std::endl
		<< "[ " << zS01 << ", " << yS01 << " ]" << std::endl;
      std::cout << "Nearest wire on Pl2 to reco-start point from Pl0, Pl1:" << std::endl
		<<  WireStart2 << std::endl;
      std::cout << "Min-Max Wire for Hits from Cluster on Plane 2:" << std::endl
		<< "[ " << minWire2 << ", " << maxWire2 << " ]" << std::endl;
      std::cout << std::endl;
    }
    //Parameter used for evaluation is whether the start-point of any reconstructed start point from 2 planes
    //is in the start-end time-wire range of the other plane's cluster
    double compat = 9999;
    double retVal = -1;

    if ( (WireStart0 > minWire0) and (WireStart0 < maxWire0) ){
      if ( std::min( (WireStart0-minWire0) , (maxWire0-WireStart0) ) < compat ){
	compat = std::min( (WireStart0-minWire0) , (maxWire0-WireStart0) );
      }
    }
    if ( (WireStart1 > minWire1) and (WireStart1 < maxWire1) ){
      if ( std::min( (WireStart0-minWire0) , (maxWire0-WireStart0) ) < compat ){
	compat = std::min( (WireStart0-minWire0) , (maxWire0-WireStart0) );
      }
    }
    if ( (WireStart2 > minWire2) and (WireStart2 < maxWire2) ){
      if ( std::min( (WireStart0-minWire0) , (maxWire0-WireStart0) ) < compat ){
	compat = std::min( (WireStart0-minWire0) , (maxWire0-WireStart0) );
      }
    }

    if ( compat != 9999 )
      retVal = 1./compat;

    return retVal;

  }

  //------------------------------
  void CFAlgoStartPointCompat::Report()
  //------------------------------
  {

  }
    
}
#endif
