#ifndef VERTEXISTARTPOINT3D_CXX
#define VERTEXISTARTPOINT3D_CXX

#include "VertexIsStartPoint3D.h"
#include <sstream>

namespace showerreco {

  void VertexIsStartPoint3D::do_reconstruction( const ::protoshower::ProtoShower & proto_shower,
						Shower_t& resultShower)
  {

    // if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()) {
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
    }
    
    // get the proto-shower 3D vertex
    auto const& vtx3D_v = proto_shower.vertexes();
    
    // are there many vertices? if more than 1 this algo does not know what to do
    if (vtx3D_v.size() != 1){
      std::cout << "Number of vertices is " << vtx3D_v.size() << " is not one!" << std::endl;
      return;
    }

    // take the vtx for the neutrino interaction
    auto const& vtx = vtx3D_v.front();

    resultShower.fXYZStart = { vtx.X(), vtx.Y(), vtx.Z() };
  } // do_reconstruction

} //showerreco

#endif
