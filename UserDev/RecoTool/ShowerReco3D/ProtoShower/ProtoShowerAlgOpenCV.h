/**
 * \file ProtoShowerAlgOpenCV.h
 *
 * \ingroup ProtoShower
 * 
 * \brief Class def header for a class ProtoShowerAlgOpenCV
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
   \class ProtoShowerAlgOpenCV
   User defined class ProtoShowerAlgOpenCV ... these comments are used to generate
   doxygen documentation!
 */

namespace showerreco {  

  class ProtoShowerAlgOpenCV : ProtoShowerAlgBase {
    
  public:
    
    /// Default constructor
    ProtoShowerAlgOpenCV();
    
    /// Default destructor
    ~ProtoShowerAlgOpenCV(){}

    void GenerateProtoShower(const std::vector<::larlite::cluster> &clus_v, 
			     const std::vector< std::vector<::larlite::hit> > &hit_v,
			     const std::vector<::larlite::spacepoint> &sps_v,
			     const std::vector<::larlite::vertex> &vtx,
			     showerreco::ProtoShower & proto_shower);
    

  private:

  };
  
}// namespace
    
#endif
/** @} */ // end of doxygen group 

