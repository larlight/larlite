#ifndef RECOTOOL_CFALGOSTARTTIMECOMPAT_CXX
#define RECOTOOL_CFALGOSTARTTIMECOMPAT_CXX

#include "CFAlgoStartTimeCompat.h"
#include <algorithm>

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoStartTimeCompat::CFAlgoStartTimeCompat() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    SetVerbose(false);
    SetTimeDist(5.);//in cm
  }

  //-----------------------------
  void CFAlgoStartTimeCompat::Reset()
  //-----------------------------
  {
  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoStartTimeCompat::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
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

    //Get Start Wire [cm], Time [cm], Plane and Channel for each cluster in the 3 planes.
    double startWirecm0 = clusters.at(0)->GetParams().start_point.w;
    double startTimecm0 = clusters.at(0)->GetParams().start_point.t;
    int startWire0 = int( startWirecm0 / _w2cm );
    unsigned char Pl0 = clusters.at(0)->GetParams().start_point.plane;
    UInt_t startChan0 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl0, startWire0);

    double startWirecm1 = clusters.at(1)->GetParams().start_point.w;
    double startTimecm1 = clusters.at(1)->GetParams().start_point.t;
    int startWire1 = int( startWirecm1 / _w2cm );
    unsigned char Pl1 = clusters.at(1)->GetParams().start_point.plane;
    UInt_t startChan1 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl1, startWire1);

    double startWirecm2 = clusters.at(2)->GetParams().start_point.w;
    double startTimecm2 = clusters.at(2)->GetParams().start_point.t;
    int startWire2 = int( startWirecm2 / _w2cm );
    unsigned char Pl2 = clusters.at(2)->GetParams().start_point.plane;
    UInt_t startChan2 = larutil::Geometry::GetME()->PlaneWireToChannel(Pl2, startWire2);

    //Get Intersections in pairs:
    //y and z indicate detector coordinate and numbers indicate planes
    //used to generate that intersection point
    double yS01, zS01, yS02, zS02, yS12, zS12;

    //Check if Channels Intersect. If so get [Y,Z] intersection points
    bool WireIntersect01 = larutil::Geometry::GetME()->ChannelsIntersect( startChan0, startChan1, yS01, zS01);
    bool WireIntersect02 = larutil::Geometry::GetME()->ChannelsIntersect( startChan0, startChan2, yS02, zS02);
    bool WireIntersect12 = larutil::Geometry::GetME()->ChannelsIntersect( startChan1, startChan2, yS12, zS12);


    // Check for 2-Plane Start-Point compatibility:
    // If Start times within pre-defined range
    // AND wires intersect --> Start Point is Good
    // Measure of compatibility is time-distance
    double compat01 = 9999, compat02 = 9999, compat12 = 9999;
    if ( (abs(startTimecm0 - startTimecm1) < _timeDist)
	 and WireIntersect01 )
      compat01 = 0.01*abs(100*(startTimecm0-startTimecm1));
    if ( (abs(startTimecm1 - startTimecm2) < _timeDist)
	 and WireIntersect12 )
      compat12 = 0.01*abs(100*(startTimecm1-startTimecm2));
    if ( (abs(startTimecm0 - startTimecm2) < _timeDist)
	 and WireIntersect02 )
      compat02 = 0.01*abs(100*(startTimecm0-startTimecm2));

    // If no compatibility return -1
    if ( (compat01 > 1000) and (compat02 > 1000) and (compat12 > 1000) ){
      if ( _verbose ) { std::cout << "No compatibility among any 2 planes..." << std::endl; }
      return -1;
    }

    // If any of the planes were compatible, return "best"
    // This is the one where time-separation is smallest
    int bestcompat;
    double bestcompatVal;
    if ( (compat01 < compat02) and (compat01 < compat12) and (compat01 < 9999) ){
      bestcompat = 2;
      bestcompatVal = compat01;
    }
    if ( (compat02 < compat01) and (compat02 < compat12) and (compat02 < 9999) ){
      bestcompat = 1;
      bestcompatVal = compat02;
    }
    if ( (compat12 < compat02) and (compat12 < compat01) and (compat12 < 9999) ){
      bestcompat = 0;
      bestcompatVal = compat12;
    }

    //Now we know which two planes have a good match for start point (if any)
    //Check if the cluster on the 3rd plane is compatible with these two planes

    // To do this check that reconstructed start point from best
    // combination is inside cluster 3
    double minWire3rdClus = 9999, minTime3rdClus = 9999;
    double maxWire3rdClus = 0, maxTime3rdClus = 0;
    double hitWireTMP, hitTimeTMP;
    Double_t Start[3] = {-100., 0., 0.};
    double StartT3rdPlane = 0;
    double wireStart3rdPlane = 0;
    UInt_t Pl3rd = 0;
    std::vector<larutil::PxHit> hits;

    if ( bestcompat == 2 ){
      StartT3rdPlane = startTimecm2;//(startTimecm0+startTimecm1)/2.;
      Pl3rd = Pl2;
      Start[1] = yS01;
      Start[2] = zS01;
      hits = clusters.at(2)->GetHitVector();
    }
    if ( bestcompat == 0 ){
      StartT3rdPlane = startTimecm0;//(startTimecm1+startTimecm2)/2.;
      Pl3rd = Pl0;
      Start[1] = yS12;
      Start[2] = zS12;
      hits = clusters.at(0)->GetHitVector();
    }
    if ( bestcompat == 1 ){
      StartT3rdPlane = startTimecm1;//(startTimecm0+startTimecm2)/2.;
      Pl3rd = Pl1;
      Start[1] = yS02;
      Start[2] = zS02;
      hits = clusters.at(1)->GetHitVector();
    }

    for (auto& h: hits){
      hitWireTMP = h.w/_w2cm;
      hitTimeTMP = h.t;
      if ( hitWireTMP < minWire3rdClus ) { minWire3rdClus = hitWireTMP; }
      if ( hitWireTMP > maxWire3rdClus ) { maxWire3rdClus = hitWireTMP; }
      if ( hitTimeTMP < minTime3rdClus ) { minTime3rdClus = hitTimeTMP; }
      if ( hitTimeTMP > maxTime3rdClus ) { maxTime3rdClus = hitTimeTMP; }
    }
    try { wireStart3rdPlane = larutil::Geometry::GetME()->NearestWire( Start, Pl3rd); }
    catch ( ::larutil::LArUtilException &e) {
      std::cout << e.what() << std::endl;
      std::cout << "Exception caught!" << std::endl;
      wireStart3rdPlane = -1;
    }

    if (_verbose){
      std::cout << "Cluster Start Times:" << std::endl;
      std::cout << "Pl0: " << startTimecm0 << "\tPl1: " << startTimecm1 << "\tPl2: " << startTimecm2 << std::endl;
      std::cout << "Best Time Separation: " << bestcompatVal << std::endl;
      std::cout << "Third Plane: " << Pl3rd << std::endl;
      std::cout << "Time Range for 3rd plane: [ " << minTime3rdClus << ", " << maxTime3rdClus << " ]" << std::endl;
      std::cout << "Start Time for 3rd Plane: " << StartT3rdPlane << std::endl;
      std::cout << "Reco z,y start point: [ " << Start[2] << ", " << Start[1] << " ]" << std::endl;
      std::cout << "Nearest Wire on 3rd Plane: " << wireStart3rdPlane << std::endl;
      std::cout << "Wire range 3rd Plane: [ " << minWire3rdClus << ", " << maxWire3rdClus << " ]" << std::endl;
    }

    //Make sure start earest wire and start time in range of 3rd cluster
    if ( wireStart3rdPlane == -1 )
      return -1;

    if ( (wireStart3rdPlane <= maxWire3rdClus) and (wireStart3rdPlane >= minWire3rdClus) and
	 (StartT3rdPlane <= maxTime3rdClus) and (StartT3rdPlane >= minTime3rdClus) ){
      if (_verbose) { std::cout << "Match OK. Return: " << bestcompatVal << std::endl; }
      return 1./bestcompatVal;
    }

      if (_verbose) { std::cout << "\n\n***************************\n" << std::endl; }

    return -1;
  }

  //------------------------------
  void CFAlgoStartTimeCompat::Report()
  //------------------------------
  {

  }
    
}
#endif
