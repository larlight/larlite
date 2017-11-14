#ifndef VERTEXISSTARTPOINT3D_H
#define VERTEXISSTARTPOINT3D_H

#include <iostream>
#include "ShowerRecoModuleBase.h"

namespace showerreco {
  class VertexIsStartPoint3D : ShowerRecoModuleBase {
  public:
    VertexIsStartPoint3D() {_name = "VertexIsStartPoint3D"; }
    ~VertexIsStartPoint3D() {}
    void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);
  };
}

#endif

