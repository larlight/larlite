/**
 * \file ProtoShowerHelper.h
 *
 * \ingroup Base
 *
 * \brief Class def header for a class ProtoShowerHelper
 *
 * @author cadams
 */

/** \addtogroup Base

    @{*/
#ifndef PROTOSHOWERHELPER_H
#define PROTOSHOWERHELPER_H

#include <iostream>

#include "ShowerRecoTypes.h"

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/seed.h"
#include "DataFormat/vertex.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "Cluster3DRecoUtil/Base/CRU3DHelper.h"

#include "Cluster3DRecoUtil/Alg/Default3DParamsAlg.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

// Hack: convertor to cparms wire/tick
#include "Hack.h"

/**
   \class ProtoShowerHelper
   User defined class ProtoShowerHelper ... these comments are used to generate
   doxygen documentation!
*/
namespace showerreco {

  class ProtoShowerHelper {

  public:

    /// Default constructor
    ProtoShowerHelper();

    /// Default destructor
    ~ProtoShowerHelper();

    // From the storage manager, and with the pfpart producer name, generate
    // a vector of all the protoshowers in the event.
    void GenerateProtoShowers(::larlite::storage_manager* storage,
			      const std::string &pfpart_producer_name,
			      std::vector<::showerreco::ProtoShower> & proto_showers,
			      std::vector<unsigned int> showerLikePFParts=std::vector<unsigned int>(1,999999));

    void SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg);
    void SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg);

  private:

    ::cluster3D::Cluster3DParamsAlg * _params3D_alg;
    ::cluster::ClusterParamsAlg * _params_alg;

    // Use the CRU Helpers to build cluster params:
    ::cluster3D::CRU3DHelper _cru3D_helper;
    ::cluster::CRUHelper _cru_helper;

    Hack hack;
    
  };

} // showerreco


#endif
/** @} */ // end of doxygen group

