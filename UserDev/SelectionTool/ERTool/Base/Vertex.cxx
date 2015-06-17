#ifndef ERTOOL_VERTEX_CXX
#define ERTOOL_VERTEX_CXX

#include "Vertex.h"
namespace ertool {

  Vertex::Vertex(double x, double y, double z) 
    : ::geoalgo::Vector(x,y,z)
  {}

  Vertex::Vertex(::geoalgo::Vector pos)
    : ::geoalgo::Vector(3)
  { pos.compat((*this)); for(size_t i=0; i<3; ++i) (*this)[i] = pos[i]; }

  
  void Vertex::Reset()
  {
    for(auto& v : (*this)) v = kINVALID_DOUBLE;
  }

}
#endif
