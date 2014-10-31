#ifndef TWOLINEINTERSECTION_CXX
#define TWOLINEINTERSECTION_CXX

#include "TwoLineIntersection.h"

namespace geoalgo {
  
  std::vector<double> TwoLineIntersection::Intersection3D(std::vector<double> const& startp1, 
							  std::vector<double> const& dir1, 
							  std::vector<double> const& startp2, 
							  std::vector<double> const& dir2)
    
  {
    
    //Format of returned vector is (x,y,z,error)
    //where (x,y,z) are the coordinates of the best-intersection
    //(midpoint of shortest line connecting the two 3D input lines)
    //and error is the length^2 of the shortest line connecting the two
    //3D input lines (0 error means the 3D lines actually intersect)
    
    std::vector<double> result(4, std::numeric_limits<double>::max());
    
    ///////////////////////////////////////////////////////
    //see http://paulbourke.net/geometry/pointlineplane
    ///////////////////////////////////////////////////////
    
    //Let's spell everything out to match the URL above
    //First start point:      "p1"
    //p1 + dir1:              "p2"
    //Second start point:     "p3"
    //p3 + dir2:              "p4"
    
    double x1 = startp1.at(0);
    double y1 = startp1.at(1);
    double z1 = startp1.at(2);
    double x2 = x1 + dir1.at(0);
    double y2 = y1 + dir1.at(1);
    double z2 = z1 + dir1.at(2);
    double x3 = startp2.at(0);
    double y3 = startp2.at(1);
    double z3 = startp2.at(2);
    double x4 = x3 + dir2.at(0);
    double y4 = y3 + dir2.at(1);
    double z4 = z3 + dir2.at(2);
    
    double d1321 = (x1-x3)*(x2-x1) + (y1-y3)*(y2-y1) + (z1-z3)*(z2-z1);
    double d2121 = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1);
    double d4321 = (x4-x3)*(x2-x1) + (y4-y3)*(y2-y1) + (z4-z3)*(z2-z1);
    double d1343 = (x1-x3)*(x4-x3) + (y1-y3)*(y4-y3) + (z1-z3)*(z4-z3);
    double d4343 = (x4-x3)*(x4-x3) + (y4-y3)*(y4-y3) + (z4-z3)*(z4-z3);
    
    double mua = (d1343*d4321 - d1321*d4343) / (d2121*d4343 - d4321*d4321);
    double mub = (d1343 + mua*d4321) / d4343;
    
    //Now, the point "Pa" (x1,y1,z1) + mua*(x2-x1,y2-y1,z2-z1)
    //is a point on the first line, closest to the second line
    //Similarly,     "Pb" (x3,y3,z3) + mub*(x4-x3,y4-y3,z4-z3)
    //is a point on the second line, closest to the first line.
    
    //Let's call the midpoint of the 3d line connecting "Pa" and "Pb"
    //to be the "intersection" point.
    
    double Pax = x1 + mua*(x2-x1);
    double Pay = y1 + mua*(y2-y1);
    double Paz = z1 + mua*(z2-z1);
    double Pbx = x3 + mub*(x4-x3);
    double Pby = y3 + mub*(y4-y3);
    double Pbz = z3 + mub*(z4-z3);
    
    result.clear();
    //result x
    result.push_back((Pax+Pbx)/2);
    //result y
    result.push_back((Pay+Pby)/2);
    //result z
    result.push_back((Paz+Pbz)/2);
    //result error (length of connecting-line squared)
    result.push_back(pow(Pax-Pbx,2)+pow(Pay-Pby,2)+pow(Paz-Pbz,2));
    
    return result;
  }

}

#endif
