#ifndef TWODIM_POLY2D_CXX
#define TWODIM_POLY2D_CXX

#include "Poly2D.h"
#include "LArUtil/GeometryHelper.h"

namespace twodimtools {

  Poly2D::Poly2D()
  { vertices.clear(); }
  
  //------------------------------------------------
  // returns slope of line uniting points p1 and p2
  float FindSlope( const std::pair<float, float> &p1,
		   const std::pair<float, float> &p2 )
  {
    float slope = (p2.second - p1.second) / (p2.first - p1.first);
    return slope;
  }
  
  //-------------------------------------------------------------------------
  // determines if 3 points are arranged in a clock-wire oder or not
  bool Clockwise(double Ax, double Ay, double Bx, double By, double Cx, double Cy)
  {
    return (Cy - Ay) * (Bx - Ax) > (By - Ay) * (Cx - Ax);
  }
  
  //------------------------------------------------------------
  // determine if two segments intersect
  bool SegmentOverlap(double Ax, double Ay, double Bx, double By,
		      double Cx, double Cy, double Dx, double Dy)
  {
    
    bool overlap = ( (Clockwise(Ax, Ay, Cx, Cy, Dx, Dy) != Clockwise(Bx, By, Cx, Cy, Dx, Dy))
		     and (Clockwise(Ax, Ay, Bx, By, Cx, Cy) != Clockwise(Ax, Ay, Bx, By, Dx, Dy)) );
    return overlap;
  }
  
  //---------------------------------------------------------------------------------
  // return intersection point for two segments
  std::pair<float, float> GetIntersection(double Ax, double Ay, double Bx, double By,
					  double Cx, double Cy, double Dx, double Dy)
  {
    
    //get equations for two lines
    // [Ax,Ay]<--->[Bx,By] : y = s1*x+c1
    // [Cx,Cy]<--->[Dx,Dy] : y = s2*x+c2
    double s1 = (By - Ay) / (Bx - Ax);
    double s2 = (Dy - Cy) / (Dx - Cx);
    double c1 = By - s1 * Bx;
    double c2 = Dy - s2 * Dx;
    
    double Xintersection = (c2 - c1) / (s2 - s1);
    double Yintersection = s1 * Xintersection + c1;
    std::pair<float, float> intersection;
    intersection = std::make_pair(Xintersection, Yintersection);
    
    return intersection;
  }
  
  //------------------------------------------------------------------
  // Poly2D constructor returning intersection of 2 polygons
  Poly2D::Poly2D(const Poly2D &poly1, const Poly2D &poly2)
  {
    
    //figure out if the two polygons overlap at all
    if ( !(poly1.PolyOverlap(poly2)) ) {
      std::vector< std::pair<float, float> > nullpoint;
      vertices = nullpoint;
      return;
    }
    
    //The overlap polygon is made up by:
    //1) all points of poly1 in poly2
    //2) all points of poly2 in poly1
    //3) all intersection points between segments
    
    //make a new set of points and add points
    //as listed above, if found.
    std::vector<std::pair<float, float> > IntersectionPoints;
    //1)
    for (unsigned int p1 = 0; p1 < poly1.Size(); p1++) {
      if ( poly2.PointInside( poly1.Point(p1) ) ) { IntersectionPoints.push_back( poly1.Point(p1) ); }
    }
    //2)
    for (unsigned int p2 = 0; p2 < poly2.Size(); p2++) {
      if ( poly1.PointInside( poly2.Point(p2) ) ) { IntersectionPoints.push_back( poly2.Point(p2) ); }
    }
    //3)
    //FIND SEGMENT INTERSECTIONS
    for (unsigned int i = 0; i < poly1.Size(); i++) {
      for (unsigned int j = 0; j < poly2.Size(); j++) {
	if (SegmentOverlap( poly1.Point(i).first, poly1.Point(i).second,
			    poly1.Point(i + 1).first, poly1.Point(i + 1).second,
			    poly2.Point(j).first, poly2.Point(j).second,
			    poly2.Point(j + 1).first, poly2.Point(j + 1).second) )
	  //segments overlap...add intersection to list
	  IntersectionPoints.push_back( GetIntersection( poly1.Point(i).first, poly1.Point(i).second,
							 poly1.Point(i + 1).first, poly1.Point(i + 1).second,
							 poly2.Point(j).first, poly2.Point(j).second,
							 poly2.Point(j + 1).first, poly2.Point(j + 1).second) );
      }//for all segments in poly2
    }//for all segments in poly1
    
    vertices = IntersectionPoints;
    return;
  }
  
  //---------------------------
  float Poly2D::Area() const
  {
    //how? here:
    //http://www.mathsisfun.com/geometry/area-irregular-polygons.html
    float area = 0;
    for (unsigned int i = 0; i < vertices.size(); i++) {
      if ( i < (vertices.size() - 1) )
	area += (((vertices.at(i)).second) + ((vertices.at(i + 1)).second)) * (((vertices.at(i)).first) - ((vertices.at(i + 1)).first)) * 0.5;
      if ( i == (vertices.size() - 1) )
	area += (((vertices.at(i)).second) + ((vertices.at(0)).second)) * (((vertices.at(i)).first) - ((vertices.at(0)).first)) * 0.5;
    }
    if (area < 0.0)
      area = -area;
    return area;
  }
  
  //--------------------------------
  float Poly2D::Perimeter() const
  {
    
    float perimeter = 0.;
    
    for (unsigned int i = 0; i < vertices.size(); i++) {
      if ( i < (vertices.size() - 1) )
	perimeter += ( (vertices.at(i).second - vertices.at(i + 1).second) *
		       (vertices.at(i).second - vertices.at(i + 1).second) +
		       (vertices.at(i).first - vertices.at(i + 1).first) *
		       (vertices.at(i).first - vertices.at(i + 1).first) );
      if ( i == (vertices.size() - 1) )
	perimeter += ( (vertices.at(i).second - vertices.at(0).second) *
		       (vertices.at(i).second - vertices.at(0).second) +
		       (vertices.at(i).first - vertices.at(0).first) *
		       (vertices.at(i).first - vertices.at(0).first) );
    }
    
    return sqrt(perimeter);
  }
  
  //------------------------------------------------------------------
  const std::pair<float, float>& Poly2D::Point(unsigned int p) const
  {
    //This function returns the vertex under consideration
    //as a std::pair<float,float> Returns vertex for argument
    //from 0 to N-1. For input N = number of sides then
    //the first vertex is returned
    if (p < vertices.size())
      return vertices.at(p);
    else if (p == vertices.size())
      return vertices.at(0);
    else {
      std::cout << "Out of bounds of Polygon!" << std::endl;
      return vertices.at(0);
    }
    
  }
  
  //------------------------------------------------------------------------
  // apply translation and rotation to a polygon
  std::pair<float, float> Poly2D::Project(const std::pair<float, float> &p,
					     float theta) const
  {
    
    std::pair<float, float> range(10000, 0);
    std::pair<float, float> ptmp;
    
    for (unsigned int i = 0; i < vertices.size(); i++) {
      //Translation
      //translating each vertex so that origin is on first vertex on polygon's edge being considered
      ptmp = std::make_pair(   (vertices.at(i)).first - p.first  ,   (vertices.at(i)).second - p.second   );
      //Rotation
      //instead of rotating each (x,y) edge (slow) just find nex x-position which gives us information
      //on the projection of that vertex on the line we are considering
      // +x direction is from vertex in consideration (vertex 'i' in loop) to next vertex
      //now find the x-coordinate of that vertex after it is rotated such that edge is now + x axis
      float xnew = (ptmp.first) * cos(theta) + (ptmp.second) * sin(theta);
      //finally calculate range of projection on x-axis: look at every x position and compare it to range
      if ( xnew < range.first )
	range.first = xnew;
      if ( xnew > range.second )
	range.second = xnew;
    }
    return range;
  }
  
  //---------------------------------------------------------------
  bool Poly2D::Overlap(float slope,
			  const Poly2D &poly2,
			  const std::pair<float, float> &origin) const
  {
    //translate and rotate both polygons
    float theta = tan(slope);
    //here we translate origin, rotate and find x-coordinates and find range of projection on edge line
    std::pair<float, float> range1 = this->Project(origin, theta);
    std::pair<float, float> range2 = poly2.Project(origin, theta);
    //std::cout << "range 1: " << range1.first << " " << range1.second << std::endl;
    //std::cout << "range 2: " << range2.first << " " << range2.second << std::endl;
    //if the two projections don't overlap --> no overlap!
    if ( ( ((range1.first <= range2.second) and ( range1.first >= range2.first )) or ((range1.second <= range2.second) and ( range1.second >= range2.first )) ) or ( ((range2.first <= range1.second) and ( range2.first >= range1.first )) or ((range2.second <= range1.second) and ( range2.second >= range1.first )) ) )
      return true;     //yes...they overlap
    else
      return false;    //no....they do not overlap
  }
  
  //-------------------------------------------------------
  bool Poly2D::PolyOverlap(const Poly2D &poly2) const
  {
    
    //start from first pair in vector then check all edges.
    //edges are (0,1), (1,2), ..., (N,N-1) each pair a pair
    //of vertexes
    for (unsigned int i = 0; i < this->Size(); i++) { //loop over first polygon's vertices
      //find projection line's slope
      //line: y=ax+b --- slope is a variable
      float slope;
      slope = FindSlope( this->Point(i) , this->Point(i + 1) );
      //if there is even one no-overlap
      //need to exit and return no overlap!
      if (! (this->Overlap( slope, poly2, this->Point(i) )) )
	return false;
    }//loop over first polygon vertices
    
    //do the exact same thing but reversing polygon role
    for (unsigned int i = 0; i < poly2.Size(); i++) { //loop over first polygon
      float slope;
      slope = FindSlope( poly2.Point(i) , poly2.Point(i + 1) );
      if (!(poly2.Overlap( slope, *this, poly2.Point(i) )) )
	return false;
    }//loop over second polygon vertices
    return true;
  }
  
  //---------------------------------------------------------------
  bool Poly2D::PolyOverlapSegments(const Poly2D &poly2) const
  {
    //if contained in one another then they also overlap:
    if ( (this->Contained(poly2)) or (poly2.Contained(*this)) ) {
      return true;
    }
    //loop over the two polygons checking wehther
    //two segments ever intersect
    for (unsigned int i = 0; i < this->Size(); i++) {
      for (unsigned int j = 0; j < poly2.Size(); j++) {
	if (SegmentOverlap( this->Point(i).first, this->Point(i).second,
			    this->Point(i + 1).first, this->Point(i + 1).second,
			    poly2.Point(j).first, poly2.Point(j).second,
			    poly2.Point(j + 1).first, poly2.Point(j + 1).second) ) {
	  return true;
	}
      }
    }
    return false;
  }
  
  //--------------------------------------------------------------------
  bool Poly2D::PointInside(const std::pair<float, float> &point) const
  {
    
    //any ray originating at point will cross polygon
    //even number of times if point outside
    //odd number of times if point inside
    int intersections = 0;
    for (unsigned int i = 0; i < this->Size(); i++) {
      if ( SegmentOverlap( this->Point(i).first, this->Point(i).second,
			   this->Point(i + 1).first, this->Point(i + 1).second,
			   10000.0, 10000.0,
			   point.first, point.second) )
	intersections += 1;
    }
    if ( (intersections % 2) == 0 )
      return false;
    else
      return true;
    
  }
  
  //-----------------------------------------------------
  bool Poly2D::Contained(const Poly2D &poly2) const
  {
    
    //loop over poly2 checking wehther
    //points of poly2 all inside poly1
    for (unsigned int i = 0; i < poly2.Size(); i++) {
      if ( !(this->PointInside( poly2.Point(i)) ) )
	return false;
    }
    
    return true;
    
  }
  
  //-------------------------------
  void Poly2D::UntanglePolygon()
  {
    
    //loop over edges
    for ( unsigned int i = 0; i < vertices.size() - 1; i++) {
      double Ax = vertices.at(i).first;
      double Ay = vertices.at(i).second;
      double Bx = vertices.at(i + 1).first;
      double By = vertices.at(i + 1).second;
      //loop over edges that have not been checked yet
      for (unsigned int j = i + 2; j < vertices.size() - 1; j++) {
	//avoid consecutive segments
	if ( vertices.at(i) == vertices.at(j + 1) )
	  continue;
	else {
	  double Cx = vertices.at(j).first;
	  double Cy = vertices.at(j).second;
	  double Dx = vertices.at(j + 1).first;
	  double Dy = vertices.at(j + 1).second;
	  
	  if ( SegmentOverlap( Ax, Ay, Bx, By, Cx, Cy, Dx, Dy ) ) {
	    std::pair<float, float> tmp = vertices.at(i + 1);
	    vertices.at(i + 1) = vertices.at(j);
	    vertices.at(j) = tmp;
	    //swapped polygon, now do recursion to make sure
	    this->UntanglePolygon();
	  }//if crossing
	}
      }//second loop
    }//first loop
    
  }
  
  ///Calculate the opening angle at the specified vertex:
  float Poly2D::InteriorAngle(unsigned int p) const {
    // Get the specified point and the two adjacent points
    if (p > vertices.size()) {
      return -9999999;
    }
    
    // std::cout << "Getting angle of point " << p << " at (" << vertices.at(p).first
    //           << ", " << vertices.at(p).second << ")" << std::endl;
    
    auto geoHelper = larutil::GeometryHelper::GetME();
    
    // Need at least 3 points:
    if (vertices.size() < 3)
      return 0.0;
    else {
      unsigned int next, prev;
      // Check if this is the very last point:
      next = (p + 1) % Size();
      prev = (p - 1 + Size()) % Size();
      // Now actually calculate the cosine of the angle with the dot product:
      larutil::Point2D point_prev, point_next, point_p;
      point_prev.w       = Point(prev).first; point_prev.t = Point(prev).second;
      point_next.w       = Point(next).first; point_next.t = Point(next).second;
      point_p.w          = Point(p).first;    point_p.t = Point(p).second;
      
      
      // Determine if this is an interior or exterior angle:
      if (vertices.size() == 3) {
	// All triangle angles are interior:
	return acos(geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev));
      }
      else {
	// Get the next point in the polygon
	std::vector<int> otherPoints;
	otherPoints.reserve(Size());
	for (int point = 0; point < Size(); point ++) {
	  if (point != p) {
	    otherPoints.push_back(point);
	  }
	}
	
	
	
	// std::cout << "\tPrev:   (" << vertices.at(prev).first << ", " << vertices.at(prev).second << ")" << std::endl;
	// std::cout << "\tNext:   (" << vertices.at(next).first << ", " << vertices.at(next).second << ")" << std::endl;
	
	// Determine if convex or concave by figuring out if it is inside or outside the sub-polygon without it
	// Basically using the algorithm from here http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
	// Need to know this in order to correctly calculate interior angle
	
	
	
	int hit_count = 0;
	for (unsigned int i = 0; i < otherPoints.size(); i++) {
	  int point = otherPoints[i];
	  int point2;
	  if (point == otherPoints.back()) {
	    point2 = otherPoints.front();
	  }
	  else {
	    point2 = otherPoints[i + 1];
	  }
	  if (SegmentOverlap(vertices.at(p).first, vertices.at(p).second,
			     -10, -10, //Compare it to an unphysical point to ensure it's not in the polygon
			     vertices.at(point).first,
			     vertices.at(point).second,
			     vertices.at(point2).first,
			     vertices.at(point2).second))
	    {
	      hit_count ++;
	    }
	}
	
	if (hit_count % 2 == 0) {
	  // Then it crossed an even number of points and is therefore not in the subpolygon.  Convex
	  // std::cout << "\tConvex, cosAngle is "
	  //           << geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev)
	  //           << ", returning "
	  //           <<  acos(geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev)) << std::endl;
	  return acos(geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev));
	}
	else
	  {
	    // std::cout << "\tConcave, cosAngle is  "
	    //           << geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev)
	    //           << ", returning "
	    //           <<  2*M_PI - acos(geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev)) << std::endl;
	    return 2 * M_PI - acos(geoHelper -> GetCosAngleBetweenLines(point_p, point_next, point_prev));
	  }
      }
    }
  }
  
  bool operator==(const Poly2D& lhs, const Poly2D& rhs) {
    return lhs.vertices == rhs.vertices;
  }
  
  bool operator!=(const Poly2D& lhs, const Poly2D& rhs) {
    return !(lhs == rhs);
  }
  
  std::ostream &operator<<(std::ostream &out, Poly2D poly) {
    
    if (poly.vertices.size( ) == 0) {
      out << "(NULL POLYGON)";
      return out;
    }
    
    for (unsigned int i = 0; i < poly.vertices.size(); ++i)
      {
	out << "(" << poly.vertices.at(i).first << ", " << poly.vertices.at(i).second << ")";
	if (i != poly.vertices.size() - 1 )
	  out << ", ";
      }
    
    return out;
  }

}// namespace

#endif
