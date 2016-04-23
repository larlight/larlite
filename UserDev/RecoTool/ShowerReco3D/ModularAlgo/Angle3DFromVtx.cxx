#ifndef ANGLE3DFROMVERTEX_CXX
#define ANGLE3DFROMVERTEX_CXX

#include "Angle3DFromVtx.h"
#include <math.h>
#include <sstream>

namespace showerreco {
  
  void Angle3DFromVtx::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
					 Shower_t& resultShower) {

    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasVertex()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing Vertex";
      throw ShowerRecoException(ss.str());
    }
    
    // get the 3D start point of the shower
    auto const& start3D = resultShower.fXYZStart;

    // get the proto-shower 3D vertex
    auto const& vtx3D_v = proto_shower.vertexes();

    // are there many vertices? if more than 1 this algo does not know what to do
    if (vtx3D_v.size() != 1){
      std::cout << "Number of vertices is " << vtx3D_v.size() << " is not one!" << std::endl;
      return;
    }

    // take the vtx -> start point direction as the 3D direction
    auto const& vtx = vtx3D_v[0];

    std::vector<double> dir3D = {0,0,0};

    dir3D[0] = start3D[0] - vtx[0];
    dir3D[1] = start3D[1] - vtx[1];
    dir3D[2] = start3D[2] - vtx[2];

    // normalize
    double mag = sqrt( dir3D[0]*dir3D[0] + dir3D[1]*dir3D[1] + dir3D[2]*dir3D[2] );
    dir3D[0] /= mag;
    dir3D[1] /= mag;
    dir3D[2] /= mag;
    
    // projet to fDCosStart values
    resultShower.fDCosStart[0] = dir3D[0];
    resultShower.fDCosStart[1] = dir3D[1];
    resultShower.fDCosStart[2] = dir3D[2];


    return;
  }
  
  
} //showerreco

#endif
