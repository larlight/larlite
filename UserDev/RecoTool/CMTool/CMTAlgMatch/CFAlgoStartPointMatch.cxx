#ifndef RECOTOOL_CFALGOSTARTPOINTMATCH_CXX
#define RECOTOOL_CFALGOSTARTPOINTMATCH_CXX

#include "CFAlgoStartPointMatch.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoStartPointMatch::CFAlgoStartPointMatch() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

    _w2cm = larutil::GeometryUtilities::GetME()->WireToCm();
    _t2cm = larutil::GeometryUtilities::GetME()->TimeToCm();
    UseTime(true);
    SetMaxArea(100.);
    
  }

  //-----------------------------
  void CFAlgoStartPointMatch::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoStartPointMatch::Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters)
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
    int startWire1 = int( clusters.at(0)->GetParams().start_point.w / _w2cm );
    double startTime1 = clusters.at(0)->GetParams().start_point.t;
    unsigned char Pl1 = clusters.at(0)->GetParams().start_point.plane;
    int startWire2 = int( clusters.at(1)->GetParams().start_point.w / _w2cm );
    double startTime2 = clusters.at(1)->GetParams().start_point.t;
    unsigned char Pl2 = clusters.at(1)->GetParams().start_point.plane;
    int startWire3 = int( clusters.at(2)->GetParams().start_point.w / _w2cm );
    double startTime3 = clusters.at(2)->GetParams().start_point.t;
    unsigned char Pl3 = clusters.at(2)->GetParams().start_point.plane;


    //Get Intersections in pairs:
    //y and z indicate detector coordinate and numbers indicate planes
    //used to generate that intersection point
    double yS12, zS12, yS13, zS13, yS23, zS23;

    larutil::Geometry::GetME()->IntersectionPoint( startWire1, startWire2,
						   Pl1, Pl2,
						   yS12, zS12);

    larutil::Geometry::GetME()->IntersectionPoint( startWire1, startWire3,
						   Pl1, Pl3,
						   yS13, zS13);

    larutil::Geometry::GetME()->IntersectionPoint( startWire2, startWire3,
						   Pl2, Pl3,
						   yS23, zS23);

    if ( _verbose ){
      std::cout << "Wire Start Numbers: " << std::endl;
      std::cout << "\t" << startWire1 << std::endl;
      std::cout << "\t" << startWire2 << std::endl;
      std::cout << "\t" << startWire3 << std::endl;
      std::cout << std::endl;
    }

    if ( _verbose ){
      std::cout << "Intersection Pl1-Pl3: ( " << yS13 << ", " << zS13 << " )" << std::endl;  
      std::cout << "Intersection Pl1-Pl2: ( " << yS12 << ", " << zS12 << " )" << std::endl;  
      std::cout << "Intersection Pl2-Pl3: ( " << yS23 << ", " << zS23 << " )" << std::endl;  
    }

    //Parameter used for evaluation is area of triangle formed by the three intersection points
    double area = -1;
    if ( !_time ){
      area = Area2D( yS12, zS12, yS23, zS23, yS13, zS13 );
    }
    if ( _time ){
      area = Area3D( (yS12+yS13)/2. , (zS12+zS13)/2. , startTime1,
		     (yS13+yS23)/2. , (zS13+zS23)/2. , startTime3,
		     (yS12+yS23)/2. , (zS13+zS23)/2. , startTime2 );
    }
    
    if ( _verbose ) { std::cout << "Area of intersections triangle is: " << area << std::endl; }

    if ( area > _MaxArea )
      return -1;
    else
      return 1./area;

  }

  //------------------------------
  void CFAlgoStartPointMatch::Report()
  //------------------------------
  {

  }

  //---------------------------------------------------------------------------------------------------
  double CFAlgoStartPointMatch::Area2D( double Ax, double Ay, double Bx, double By, double Cx, double Cy ) {
  //---------------------------------------------------------------------------------------------------

    double a = (Ax*(By-Cy)+Bx*(Cy-Ay)+Cx*(Ay-By))*0.5;

    if ( a < 0 ) { a *= -1; }

    return a;

  }

  //---------------------------------------------------------------------------------------------------
  double CFAlgoStartPointMatch::Area3D( double Ax, double Ay, double Az,
					double Bx, double By, double Bz,
					double Cx, double Cy, double Cz ) {
  //---------------------------------------------------------------------------------------------------

    //Create vectors AB and AC
    Bx = Bx-Ax;
    By = By-Ay;
    Bz = Bz-Az;
    Cx = Cx-Ax;
    Cy = Cy-Ay;
    Cz = Cz-Az;

    return 0.5*sqrt( (By*Cz-Cz*By)*(By*Cz-Cz*By) + (Bz*Cx-Bx*Cz)*(Bz*Cx-Bx*Cz) + (Bx*Cy-By*Cx)*(Bx*Cy-By*Cx) );
  }

    
}
#endif
