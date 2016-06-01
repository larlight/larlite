#ifndef FILTER3DDIR_CXX
#define FILTER3DDIR_CXX

#include "Filter3DDir.h"
#include <sstream>

namespace showerreco {

void Filter3DDir::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                    Shower_t& resultShower) {

  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasVertex()){
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing Vertex";
    throw ShowerRecoException(ss.str());
  }

  // get the 3D start point of the shower
  auto const& start3D = resultShower.fXYZStart;
  
  // get the 3D direction reconstructed by a previous algorithm
  auto const& dir3D   = resultShower.fDCosStart;

  // get the proto-shower 3D vertex
  auto const& vtx3D_v = proto_shower.vertexes();

  // make sure there is a single vertex only
  if (vtx3D_v.size() != 1){
    std::cout << "Number of vertices is " << vtx3D_v.size() << " is not one!" << std::endl;
    return;
  }
  auto const& vtx3D = vtx3D_v[0];

  // the vertex needs to be at least some distance from the start poitn for this to work
  double d = sqrt ( ( (start3D[0]-vtx3D[0]) * (start3D[0]-vtx3D[0]) ) +
		    ( (start3D[1]-vtx3D[1]) * (start3D[1]-vtx3D[1]) ) +
		    ( (start3D[2]-vtx3D[2]) * (start3D[2]-vtx3D[2]) ) );

  if (d < 5)
    return;

  // get 3D segment between start and vtx
  std::vector<double> seg3D = {0,0,0};
  seg3D[0] = start3D[0] - vtx3D[0];
  seg3D[1] = start3D[1] - vtx3D[1];
  seg3D[2] = start3D[2] - vtx3D[2];
  // normalize
  double mag = sqrt( seg3D[0]*seg3D[0] + seg3D[1]*seg3D[1] + seg3D[2]*seg3D[2] );
  seg3D[0] /= mag;
  seg3D[1] /= mag;
  seg3D[2] /= mag;

  // if dot product between 3D start -> vtx and reco'd 3DDir is high enough -> good shower
  // otherwise reject

  double dot = dir3D[0]*seg3D[0] + dir3D[1]*seg3D[1] + dir3D[2]*seg3D[2];
  //std::cout << "dot product is " << dot << std::endl;

  if ( fabs(dot) < _dot_cut){
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to vtx->srt and 3Ddir mismatch. dot product is " << dot;
    throw ShowerRecoException(ss.str());
  }  
  
  // This function takes the shower cluster set and computes the best fit 3D axis
  // and then assigns it to the shower

}


} //showerreco

#endif
