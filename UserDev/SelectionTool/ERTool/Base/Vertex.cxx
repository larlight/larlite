#ifndef ERTOOL_VERTEX_CXX
#define ERTOOL_VERTEX_CXX

#include "Vertex.h"
namespace ertool {

  Vertex::Vertex(double x, double y, double z) 
    : ::geoalgo::Vector(x,y,z)
  {}

  void Vertex::Reset()
  {
    for(auto& v : (*this)) v = kINVALID_DOUBLE;
  }

}
#endif
