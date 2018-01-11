/**
 * \file Polygon.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief 2D polygon object
 *
 * @author kazuhiro & david caratelli
 */

/** \addtogroup ClusterRecoUtil

    @{*/

#ifndef TWODIM_POLY2D_H
#define TWODIM_POLY2D_H

#include <vector>
#include <utility>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>

//a polygon is a vector of std::pairs with first = x coordinate
//and second = y coordinate of that vertex
//access vertices with Point function. Points are:
//0   = first ordered vertex
//n-1 = last ordered vertex (n=size of polygon)
//n   = first vertex again
//>n  = invalid...return error message

namespace twodimtools {
  
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  //                BEGIN POLYGON CLASS               //
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  class Poly2D{
    
  public:
    
    /// default constructor
    Poly2D();
    /// default destructor
    ~Poly2D(){}
    /// constructor starting from list of edges for polygon
    Poly2D(const std::vector< std::pair<float,float> > &points) { vertices = points; }
    /// Create Intersection Polygon from 2 polygons
    Poly2D(const Poly2D &poly1, const Poly2D &poly2); 
    /// return number of edges in polygon
    unsigned int Size() const { return vertices.size(); } 
    /// return pair with information for a specific edge
    const std::pair<float,float>& Point(unsigned int p) const; 
    /// Project Polygon applying translation and rotation
    std::pair<float,float> Project(const std::pair<float,float> &p,float theta) const;
    /// Return Polygon Area
    float Area() const;
    /// return polygon perimeter
    float Perimeter() const;
    /// boolean: do these polygons overlap?
    bool Overlap(const Poly2D &poly2) const;
    /// boolean: do these polygons overlap?
    bool PolyOverlap(const Poly2D &poly2) const;
    /// boolean: is a point inside the polygon?
    bool PointInside(const std::pair<float,float> &point) const;
    /// check if poly2 is fully contained in poly1
    bool Contained(const Poly2D &poly2) const; 
    /// untangle polygon
    void UntanglePolygon();
    /// clear polygon's points
    void Clear() { vertices.clear(); }
    
    ///Calculate the opening angle at the specified vertex:
    float InteriorAngle(unsigned int p) const;
    
    friend bool operator==(const Poly2D& lhs, const Poly2D& rhs);
    friend bool operator!=(const Poly2D& lhs, const Poly2D& rhs);
    friend std::ostream &operator<<(std::ostream &out, Poly2D poly);     //output
    
    
  private:
    
    /// vector listing the polygon edges
    std::vector< std::pair<float,float> > vertices;
    
    /// utility function used by PolyOverlap to determine overlap
    bool Overlap(float slope, const Poly2D &poly2, const std::pair<float,float> &origin) const;
    
    /// utility function used in polygon overlap determination
    bool PolyOverlapSegments(const Poly2D &poly2) const;
    
  };
  
}// namespace


/** @} */ // end of doxygen group

#endif
