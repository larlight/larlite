#ifndef PROTOSHOWERALGCLUSTERPARAMS_CXX
#define PROTOSHOWERALGCLUSTERPARAMS_CXX

#include "ProtoShowerAlgClusterParams.h"
#include "TVector3.h"

namespace showerreco {

  ProtoShowerAlgClusterParams::ProtoShowerAlgClusterParams() {

    _params_alg = 0;
    _params3D_alg = 0;
    
  }

  void ProtoShowerAlgClusterParams::SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg) {
    
    if (_params_alg) {
      delete _params_alg;
    }
    _params_alg = _new_params_alg;
    
    return;
  }
  
  void ProtoShowerAlgClusterParams::SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg) {
    
    if (_params3D_alg) {
      delete _params3D_alg;
    }
    _params3D_alg = _new_params3D_alg;
    
    return;
  }

  void ProtoShowerAlgClusterParams::GenerateProtoShower(const std::vector<::larlite::cluster> &clus_v,
							const std::vector<std::vector<::larlite::hit> > &hit_v,
							const std::vector<::larlite::spacepoint> &sps_v,
							const std::vector<::larlite::vertex> &vtx_v,
							showerreco::ProtoShower & proto_shower)
  {


    // if no algo for calculating cluster params -> set up default ones
    if (!_params_alg) {
      _params_alg = new ::cluster::DefaultParamsAlg();
    }
    if (!_params3D_alg) {
      _params3D_alg = new ::cluster3D::Default3DParamsAlg();
    }

    // if no clusters for this PFParticle -> no 2D information
    proto_shower.hasCluster2D(true);
    if (clus_v.size() == 0)
      proto_shower.hasCluster2D(false);

    // resize _params according to number of clusters
    proto_shower._params.resize( clus_v.size() );

    // fill 2D information, if available
    for (size_t i=0; i < clus_v.size(); i++){

      _cru_helper.GenerateParams( hit_v[i], proto_shower._params.at( i ) );

      _params_alg->FillParams( proto_shower._params.at( i ) );

    }// for all input clusters

    // if there are spacepoints, fill 3D params as well
    if (sps_v.size() == 0)
      proto_shower.hasCluster3D(false);
    else{
      proto_shower.hasCluster3D(true);
      _cru3D_helper.GenerateParams3D( sps_v, proto_shower._params3D);
    }

    // fill vertex information
    if (vtx_v.size() == 0)
      proto_shower.hasVertex(false);
    else{
      for (auto const& vtx : vtx_v){
	double xyz[3];
	vtx.XYZ(xyz);
	proto_shower._vertexes.push_back(TVector3(xyz[0],xyz[1],xyz[2]));
      }
      proto_shower.hasVertex(true);
    }
      
    return;
  }
  

}// namespace

#endif
