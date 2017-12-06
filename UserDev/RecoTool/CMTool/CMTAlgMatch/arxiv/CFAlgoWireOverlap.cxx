#ifndef RECOTOOL_CFALGOWIREOVERLAP_CXX
#define RECOTOOL_CFALGOWIREOVERLAP_CXX

#include "CFAlgoWireOverlap.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"
#include <algorithm>

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoWireOverlap::CFAlgoWireOverlap() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _w2cm = larutil::GeometryHelper::GetME()->WireToCm();
    _t2cm = larutil::GeometryHelper::GetME()->TimeToCm();
    SetVerbose(false);
    SetDebug(false);
    SetUseAllPlanes(false); // Any plane combination OK
  }

  //-----------------------------
  void CFAlgoWireOverlap::Reset()
  //-----------------------------
  {
  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoWireOverlap::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {

    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if (_UseAllPlanes) { if(clusters.size()==2) return -1; }
    // Code-block by Kazu ends

    //This algorithm now works for 3 planes: find 3Dstart point from first 2 planes and find
    //How well that agrees with 3rd plane's start point location.

    //So first, make sure clusters vector has more than 1 element.
    //If not, return -1. Algorithm does not make sense
    if ( clusters.size() == 1 )
      return -1;

    if (_verbose) { std::cout << "Number of clusters taken into account: " << clusters.size() << std::endl; }
    
    //Nomenclature:
    //Planes: U == 0; V == 1; Y == 2.

    //Get hits for all 3 clusters
    std::vector<std::vector<larutil::PxHit> > Hits(3, std::vector<larutil::PxHit>());

    for (size_t c=0; c < clusters.size(); c++)
      Hits.at( clusters.at(c)->plane_id.Plane ) = clusters.at(c)->hit_vector;

    //std::vector<larutil::PxHit> hits0 = clusters.at(0)->hit_vector;
    //std::vector<larutil::PxHit> hits1 = clusters.at(1)->hit_vector;
    //std::vector<larutil::PxHit> hits2 = clusters.at(2)->hit_vector;
    
    //Wire Range Vector. Entries 0,1,2 correspond to planes
    std::vector<int> StartWires;
    std::vector<int> EndWires;
    //loop over number of planes and pre-fill StartWires and EndWires
    for (int pl=0; pl < 3; pl++){
      StartWires.push_back(9999);
      EndWires.push_back(0);
    }

    for (size_t h=0; h < Hits.size(); h++){
      for ( auto& hit: Hits.at(h) ){
	if ( Hits.at(h).size() == 0 ){
	  std::cout << "Need to insert fake hit ranges...";
	}
	else{
	  if ( int(hit.w / _w2cm) < StartWires.at(h) ) { StartWires.at(h) = int(hit.w / _w2cm); }
	  if ( int(hit.w / _w2cm) > EndWires.at(h) )   { EndWires.at(h)   = int(hit.w / _w2cm); }
	}
      }//for all hits in range
    }//for all hit-lists (i.e. for all clusters)

    //if one of the plane's wire-range is still [9999, 0] then replace it
    //with min/max wire number from that plane
    //This allows for easy 2-Plane matching: if we are ignoring one plane
    //completely by giving the wire-range for the whole plane the intersection
    //will effectively be the intersection of the other two planes
    for (size_t h=0; h < Hits.size(); h++){
      if ( StartWires.at(h) == 9999 ) { StartWires.at(h) = 1; }
      if ( EndWires.at(h) == 0 ) { EndWires.at(h) = larutil::Geometry::GetME()->Nwires(h)-1; }
    }
    /*
    //Get Wire-Range for all 3 clusters
    int startWire0 = 9999, endWire0 = 0;
    int startWire1 = 9999, endWire1 = 0;
    int startWire2 = 9999, endWire2 = 0;
    for (auto& hit: hits0){
      if ( int(hit.w / _w2cm) < startWire0 ) { startWire0 = int(hit.w / _w2cm); }
      if ( int(hit.w / _w2cm) > endWire0 )   { endWire0   = int(hit.w / _w2cm); }
    }
    for (auto& hit: hits1){
      if ( int(hit.w / _w2cm) < startWire1 ) { startWire1 = int(hit.w / _w2cm); }
      if ( int(hit.w / _w2cm) > endWire1 )   { endWire1   = int(hit.w / _w2cm); }
    }
    for (auto& hit: hits2){
      if ( int(hit.w / _w2cm) < startWire2 ) { startWire2 = int(hit.w / _w2cm); }
      if ( int(hit.w / _w2cm) > endWire2 )   { endWire2   = int(hit.w / _w2cm); }
    }
    */
    //Now get start & end points of all these wires
    Double_t xyzStart0WireStart[3] = {0., 0., 0.}; //xyz array info of start point for smallest wire number on plane 0
    Double_t xyzStart0WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzEnd0WireStart[3] = {0., 0., 0.}; 
    Double_t xyzEnd0WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzStart1WireStart[3] = {0., 0., 0.}; //xyz array info of start point for smallest wire number on plane 1
    Double_t xyzStart1WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzEnd1WireStart[3] = {0., 0., 0.}; 
    Double_t xyzEnd1WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzStart2WireStart[3] = {0., 0., 0.}; //xyz array info of start point for smallest wire number on plane 2
    Double_t xyzStart2WireEnd[3]   = {0., 0., 0.}; 
    Double_t xyzEnd2WireStart[3] = {0., 0., 0.}; 
    Double_t xyzEnd2WireEnd[3]   = {0., 0., 0.}; 

    if (_debug) {
      std::cout << "Wire Ranges:" << std::endl;
      std::cout << "U-Plane: [ " << StartWires.at(0) << ", " << EndWires.at(0) << "]" << std::endl;
      std::cout << "V-Plane: [ " << StartWires.at(1) << ", " << EndWires.at(1) << "]" << std::endl;
      std::cout << "Y-Plane: [ " << StartWires.at(2) << ", " << EndWires.at(2) << "]" << std::endl;
      std::cout << std::endl;
    }

    larutil::Geometry::GetME()->WireEndPoints(0, StartWires.at(0), xyzStart0WireStart, xyzStart0WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(0, EndWires.at(0), xyzEnd0WireStart, xyzEnd0WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(1, StartWires.at(1), xyzStart1WireStart, xyzStart1WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(1, EndWires.at(1), xyzEnd1WireStart, xyzEnd1WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(2, StartWires.at(2), xyzStart2WireStart, xyzStart2WireEnd);
    larutil::Geometry::GetME()->WireEndPoints(2, EndWires.at(2), xyzEnd2WireStart, xyzEnd2WireEnd);
    //check that z-positions for plane2 are the same...if not error!
    //if ( xyzStart2WireStart[2] != xyzStart2WireEnd[2] ) { std::cout << "Y-wire does not have same Z start and end..." << std::endl; }
    double zMin = xyzStart2WireStart[2];
    //if ( xyzEnd2WireStart[2] != xyzEnd2WireEnd[2] ) { std::cout << "Y-wire does not have same Z start and end..." << std::endl; }
    double zMax = xyzEnd2WireStart[2];

    //Plane U == Plane 0
    //Plane V == Plane 1
    //Plane Y == Plane 2

    //Each line can be described by function y = s*z + b (x,y,z coordinates same as uBooNE coord.)
    //Slopes known: Pl0 is +60 clockwise from vertical. Pl1 is -60, counterclockwise from vertical. Looking at TPC with beam from left
    double slopeU = tan(30*3.14/180.);
    double slopeV = tan(-30*3.14/180.);

    //Find intercepts:
    double bUup = xyzStart0WireStart[1] - xyzStart0WireStart[2] * slopeU; 
    double bUdn   = xyzEnd0WireStart[1] - xyzEnd0WireStart[2] * slopeU; 
    double bVdn = xyzStart1WireStart[1] - xyzStart1WireStart[2] * slopeV; 
    double bVup   = xyzEnd1WireStart[1] - xyzEnd1WireStart[2] * slopeV; 
    //make sure we know which line is above which
    if ( bUup < bUdn ) { std::cout << "Uup and Udn are mixed up!" << std::endl; }
    if ( bVdn > bVup ) { std::cout << "Vup and Vdn are mixed up!" << std::endl; }
    //Pl2 lines are vertical...slope is infinite and no intercept.

    //Find intercepts of U wire-ranges with Y plane (easy since vertical)
    //For now assume wire-ranges go to infinity, worry about TPC constraints later

    //Plug in Y-plane zMin and zMax coordinates into equations for U/V wire lines
    double VdnZmin = slopeV * zMin + bVdn; 
    double VdnZmax = slopeV * zMax + bVdn; 
    double VupZmin = slopeV * zMin + bVup; 
    double VupZmax = slopeV * zMax + bVup; 
    double UdnZmin = slopeU * zMin + bUdn; 
    double UdnZmax = slopeU * zMax + bUdn; 
    double UupZmin = slopeU * zMin + bUup; 
    double UupZmax = slopeU * zMax + bUup;

    if (_debug){
      std::cout << "Y-Plane and U-Plane points [Z,Y]:" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << UdnZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << UupZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << UupZmax << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << UdnZmax << "]" << std::endl;
      std::cout << "Y-Plane and V-Plane points [Z,Y]:" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << VdnZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMin << ", " << VupZmin << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << VupZmax << "]" << std::endl;
      std::cout << "\t\t[ " << zMax << ", " << VdnZmax << "]" << std::endl;
    }
    //We now have Two polygons:
    //One is the intersection of Y-Plane wires with U-Plane wires
    //The other the intersection of planes Y and V.
    //The intersection points of these two polygons is the 
    //overall intersection Area of the 3 clusters on the Y-Z plane.

    //Go through all segment intersections. If one is found add to 
    //list of points.
    //Create a list of points for polygon
    std::vector< std::pair<float,float> > WireIntersection;
    double zInt; // temporary holder for z-intersection point of oblique sides
    //Check: Vup and Uup, Vup and Uright, Vup and Uleft, Vup and Udn.
    //Intersection between Vup and Uup: if within zMin, zMax then ok!
    zInt = (bUup-bVup)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVup ) );
    //Intersection between Vup and Uright:
    if ( (VupZmax < UupZmax) and (VupZmax > UdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, VupZmax ) );
    //Intersection between Vup and Uleft:
    if ( (VupZmin < UupZmin) and ( VupZmin > UdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, VupZmin ) );
    //Intersection between Vup and Udn:
    zInt = (bUdn-bVup)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVup ) );
    
    //Check: Vdn and Uup, Uright, Uleft, Udn:
    //Intersection between Vdn and Uup:
    zInt = (bUup-bVdn)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVdn ) );
    //Intersection between Vdn and Uright:
    if ( (VdnZmax < UupZmax) and (VdnZmax > UdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, VdnZmax ) );
    //Intersection between Vdn and Uleft:
    if ( (VdnZmin < UupZmin) and ( VdnZmin > UdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, VdnZmin ) );
    //Intersection between Vdn and Udn:
    zInt = (bUdn-bVdn)/(slopeV-slopeU);
    if ( (zInt > zMin) and (zInt < zMax) )
      WireIntersection.push_back( std::make_pair( zInt, slopeV*zInt+bVdn ) );
      
    //Check: Vright and Uup, Udn:
    //Intersection between Vright and Uup:
    if ( (UupZmax < VupZmax) and ( UupZmax > VdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, UupZmax ) );
    //Intersection between Vright and Udn:
    if ( (UdnZmax < VupZmax) and ( UdnZmax > VdnZmax) )
      WireIntersection.push_back( std::make_pair( zMax, UdnZmax ) );

    //Check Vleft and Uup, Udn:
    //Intersection between Vleft and Uup:
    if ( (UupZmin < VupZmin) and ( UupZmin > VdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, UupZmin ) );
    //Intersection between Vleft and Udn:
    if ( (UdnZmin < VupZmin) and ( UdnZmin > VdnZmin) )
      WireIntersection.push_back( std::make_pair( zMin, UdnZmin ) );

    //If length is 0 then no intersection...return -1
    if ( WireIntersection.size() == 0 ){
      if (_verbose) { std::cout << "No intersection..." << std::endl << std::endl; }
      return -1;
    }

    //Now our polygon is complete...
    //need to disentangle in case points added in incorrect order
    //then calculate area
    Polygon2D WirePolygon(WireIntersection);    
    //Variable to hold final output Area
    double PolyArea = -1;
    //Check order
    WirePolygon.UntanglePolygon();

    //Create a Polygon for the Y-Z TPC Plane
    std::vector< std::pair<float,float> > TPCCorners;
    TPCCorners.push_back( std::make_pair(0., -larutil::Geometry::GetME()->DetHalfHeight()) );
    TPCCorners.push_back( std::make_pair(larutil::Geometry::GetME()->DetLength(), -larutil::Geometry::GetME()->DetHalfHeight()) );
    TPCCorners.push_back( std::make_pair(larutil::Geometry::GetME()->DetLength(), larutil::Geometry::GetME()->DetHalfHeight()) );
    TPCCorners.push_back( std::make_pair(0., larutil::Geometry::GetME()->DetHalfHeight()) );
    Polygon2D TPCPolygon(TPCCorners);
    if (TPCPolygon.Contained(WirePolygon) ){
      if (_verbose) {
	std::cout << "Wire Overlap contained in TPC" << std::endl;
	std::cout << "Intersection Polygon Coordinates [Z,Y]: " << std::endl;
	for (unsigned int s=0; s < WirePolygon.Size(); s++)
	  std::cout << "\t\t[ " << WirePolygon.Point(s).first << ", " << WirePolygon.Point(s).second << "]" << std::endl;
	std::cout << std::endl;
      }
      PolyArea = WirePolygon.Area();
    }
    else {
      if (_verbose) {
	std::cout << "Wire overlap not fully contained in TPC" << std::endl;
	std::cout << "Intersection Polygon Coordinates [Z,Y]: " << std::endl;
	for (unsigned int s=0; s < WirePolygon.Size(); s++)
	  std::cout << "\t\t[ " << WirePolygon.Point(s).first << ", " << WirePolygon.Point(s).second << "]" << std::endl;
	std::cout << std::endl;
      }
      //product polygon should be the intersection of WirePolygon and TPCPolygon
	Polygon2D IntersectionPolygon(TPCPolygon, WirePolygon);
	PolyArea = IntersectionPolygon.Area();
    }

    //return polygon area -> larger = better!
    if (_verbose) { std::cout << "Intersection area: " << PolyArea << std::endl << std::endl; }    
    return  PolyArea;
  }

  //------------------------------
  void CFAlgoWireOverlap::Report()
  //------------------------------
  {

  }
    
}
#endif
