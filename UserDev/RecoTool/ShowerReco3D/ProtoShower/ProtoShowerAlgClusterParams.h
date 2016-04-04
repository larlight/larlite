/**
 * \file ProtoShowerAlgClusterParams.h
 *
 * \ingroup ProtoShower
 * 
 * \brief Class def header for a class ProtoShowerAlgClusterParams
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGCLUSTERPARAMS_H
#define PROTOSHOWERALGCLUSTERPARAMS_H

#include <iostream>

#include "ShowerReco3D/Base/ProtoShowerAlgBase.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "Cluster3DRecoUtil/Base/CRU3DHelper.h"

#include "Cluster3DRecoUtil/Alg/Default3DParamsAlg.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

/**
   \class ProtoShowerAlgClusterParams
   User defined class ProtoShowerAlgClusterParams ... these comments are used to generate
   doxygen documentation!
 */

namespace showerreco {  

  class ProtoShowerAlgClusterParams : ProtoShowerAlgBase {
    
  public:
    
    /// Default constructor
    ProtoShowerAlgClusterParams();
    
    /// Default destructor
    ~ProtoShowerAlgClusterParams(){}

    void GenerateProtoShower(const std::vector<::larlite::cluster> &clus_v, 
			     const std::vector< std::vector<::larlite::hit> > &hit_v,
			     const std::vector<::larlite::spacepoint> &sps_v,
			     const std::vector<::larlite::vertex> &vtx,
			     showerreco::ProtoShower & proto_shower);
    

    void SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg);
    void SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg);
    
  private:

    ::cluster3D::Cluster3DParamsAlg * _params3D_alg;
    ::cluster::ClusterParamsAlg * _params_alg;
    
    // Use the CRU Helpers to build cluster params:
    ::cluster3D::CRU3DHelper _cru3D_helper;
    ::cluster::CRUHelper _cru_helper;
    
  };
  
}// namespace
    
#endif
/** @} */ // end of doxygen group 

